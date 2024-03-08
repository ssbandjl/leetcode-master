#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <rdma/rdma_cma.h>

// 
#define TEST_NZ(x) do { if ( (x)) die("error: " #x " failed (returned non-zero)." ); } while (0)
#define TEST_Z(x)  do { if (!(x)) die("error: " #x " failed (returned zero/null)."); } while (0)

const int BUFFER_SIZE = 1024;

struct context {
  struct ibv_context *ctx;
  struct ibv_pd *pd;
  struct ibv_cq *cq;
  struct ibv_comp_channel *comp_channel;

  pthread_t cq_poller_thread;
};

struct connection {
  struct ibv_qp *qp;

  struct ibv_mr *recv_mr;
  struct ibv_mr *send_mr;

  char *recv_region;
  char *send_region;
};

static void die(const char *reason);

static void build_context(struct ibv_context *verbs);
static void build_qp_attr(struct ibv_qp_init_attr *qp_attr);
static void * poll_cq(void *);
static void post_receives(struct connection *conn);
static void register_memory(struct connection *conn);

static void on_completion(struct ibv_wc *wc);
static int on_connect_request(struct rdma_cm_id *id);
static int on_connection(void *context);
static int on_disconnect(struct rdma_cm_id *id);
static int on_event(struct rdma_cm_event *event);

static struct context *s_ctx = NULL;

// 入口
int main(int argc, char **argv)
{
#if _USE_IPV6
  struct sockaddr_in6 addr;
#else
  struct sockaddr_in addr;  //定义socket对象
#endif
  struct rdma_cm_event *event = NULL;  //定义rdma连接管理事件
  struct rdma_cm_id *listener = NULL;   //rdma连接管理id
  struct rdma_event_channel *ec = NULL; //rdma事件通道
  uint16_t port = 0;

  memset(&addr, 0, sizeof(addr));  // 设置地址为全0
#if _USE_IPV6
  addr.sin6_family = AF_INET6;
#else
  addr.sin_family = AF_INET;   //设置协议族
#endif

  TEST_Z(ec = rdma_create_event_channel());  // 打开一个通道, 异步事件通过事件通道上报给用户。
  TEST_NZ(rdma_create_id(ec, &listener, NULL, RDMA_PS_TCP));  //分配连接标识, 端口类型枚举:RDMA_PS_IB/RDMA_PS_UDP...
  TEST_NZ(rdma_bind_addr(listener, (struct sockaddr *)&addr));  // 将 RDMA 标识符绑定到源地址
  TEST_NZ(rdma_listen(listener, 10)); /* backlog=10 is arbitrary, 监听(等待)传入的连接请求*/  

  port = ntohs(rdma_get_src_port(listener));

  printf("listening on port %d.\n", port);

  // 循环阻塞并监听事件
  while (rdma_get_cm_event(ec, &event) == 0) {
    struct rdma_cm_event event_copy;

    memcpy(&event_copy, event, sizeof(*event));  //拷贝事件
    rdma_ack_cm_event(event); //确认并释放事件, 对于每个从event channel得到的事件，都要调用ack函数，否则会产生内存泄漏。这一步的ack是对应第5步的get。每一次get调用，都要有对应的ack调用
    // 处理事件
    if (on_event(&event_copy))  //如果有异常则中断循环, 走清理流程
      break;
  }

  rdma_destroy_id(listener);  //销毁/清理id
  rdma_destroy_event_channel(ec); //销毁/清理通道

  return 0;
}

void die(const char *reason)
{
  fprintf(stderr, "%s\n", reason);
  exit(EXIT_FAILURE);
}

// 构造上下文
void build_context(struct ibv_context *verbs)
{
  if (s_ctx) {
    if (s_ctx->ctx != verbs)
      die("cannot handle events in more than one context.");

    return;
  }

  s_ctx = (struct context *)malloc(sizeof(struct context));

  s_ctx->ctx = verbs;

  TEST_Z(s_ctx->pd = ibv_alloc_pd(s_ctx->ctx));  // ib_alloc_pd() 方法分配一个PD。 它将设备的指针作为参数, 注册后调用驱动程序回调时返回的对象
  TEST_Z(s_ctx->comp_channel = ibv_create_comp_channel(s_ctx->ctx));  // 创建完成通道
  TEST_Z(s_ctx->cq = ibv_create_cq(s_ctx->ctx, 10, NULL, s_ctx->comp_channel, 0)); /* cqe=10 is arbitrary(随意的) 创建完成队列*/
  TEST_NZ(ibv_req_notify_cq(s_ctx->cq, 0));  // 在 CQ 上请求完成通知。 当向 CQ 添加条目时，将向与 CQ 关联的完成通道添加一个事件。

  TEST_NZ(pthread_create(&s_ctx->cq_poller_thread, NULL, poll_cq, NULL));  //创建poll_cq(poll完成队列)线程
}

void build_qp_attr(struct ibv_qp_init_attr *qp_attr)
{
  memset(qp_attr, 0, sizeof(*qp_attr));

  qp_attr->send_cq = s_ctx->cq;
  qp_attr->recv_cq = s_ctx->cq;
  qp_attr->qp_type = IBV_QPT_RC;  // IB_QPT_RC: A Reliable Connected QP, 可靠的连接 QP

  qp_attr->cap.max_send_wr = 10;
  qp_attr->cap.max_recv_wr = 10;
  qp_attr->cap.max_send_sge = 1;
  qp_attr->cap.max_recv_sge = 1;
}

void * poll_cq(void *ctx)
{
  struct ibv_cq *cq;
  struct ibv_wc wc;  //  Work Completion工作完成对象

  while (1) {
    TEST_NZ(ibv_get_cq_event(s_ctx->comp_channel, &cq, &ctx));  // 获取下一个完成队列事件
    ibv_ack_cq_events(cq, 1);
    TEST_NZ(ibv_req_notify_cq(cq, 0)); 

    while (ibv_poll_cq(cq, 1, &wc))// ibv_poll_cq - 轮询 CQ 以获取工作完成情况
      on_completion(&wc);  //执行回调, 
  }

  return NULL;
}

void post_receives(struct connection *conn)
{
  struct ibv_recv_wr wr, *bad_wr = NULL;
  struct ibv_sge sge;  // scatter/gather 聚集/分散, 元素由 struct ib_sge 表示

  wr.wr_id = (uintptr_t)conn;
  wr.next = NULL;
  wr.sg_list = &sge;
  wr.num_sge = 1;

  sge.addr = (uintptr_t)conn->recv_region;
  sge.length = BUFFER_SIZE;
  sge.lkey = conn->recv_mr->lkey;

  /*ib_post_recv() 方法获取接收请求的链表并将它们添加到接收队列中未来的处理。 每个接收请求都被认为是未完成的，直到在其之后生成工作完成加工。 成功时它将返回 0 或带有失败原因的 errno 值。*/
  TEST_NZ(ibv_post_recv(conn->qp, &wr, &bad_wr));
}

void register_memory(struct connection *conn)
{
  conn->send_region = malloc(BUFFER_SIZE);
  conn->recv_region = malloc(BUFFER_SIZE);

  TEST_Z(conn->send_mr = ibv_reg_mr(
    s_ctx->pd,
    conn->send_region,
    BUFFER_SIZE,
    0));

  TEST_Z(conn->recv_mr = ibv_reg_mr(
    s_ctx->pd,
    conn->recv_region,
    BUFFER_SIZE,
    IBV_ACCESS_LOCAL_WRITE));
}

void on_completion(struct ibv_wc *wc)
{
  if (wc->status != IBV_WC_SUCCESS)
    die("on_completion: status is not IBV_WC_SUCCESS.");

  if (wc->opcode & IBV_WC_RECV) {  // 设置 IBV_WC_RECV 的值，以便消费者可以测试完成是否是通过测试接收（操作码和 IBV_WC_RECV), IB_WC_RECV：传入发送操作的接收请求已在接收方。
    struct connection *conn = (struct connection *)(uintptr_t)wc->wr_id;

    printf("received message: %s\n", conn->recv_region);

  } else if (wc->opcode == IBV_WC_SEND) {  // 发送操作在发送方完成。
    printf("send completed successfully.\n");
  }
}

int on_connect_request(struct rdma_cm_id *id)
{
  struct ibv_qp_init_attr qp_attr;  // 结构 ibv_qp_init_attr 描述了新创建的 QP 的请求属性
  struct rdma_conn_param cm_params;  // 连接参数
  struct connection *conn;

  printf("received connection request.\n");

  build_context(id->verbs);
  build_qp_attr(&qp_attr);  // 设置连接属性

  /*ib_create_qp() 方法创建一个 QP。 它会在成功时返回一个指向新创建的 QP 的指针或 ERR_PTR() 指定失败的原因。*/
  TEST_NZ(rdma_create_qp(id, s_ctx->pd, &qp_attr));

  id->context = conn = (struct connection *)malloc(sizeof(struct connection));
  conn->qp = id->qp;

  register_memory(conn);  // 注册内存
  post_receives(conn);

  memset(&cm_params, 0, sizeof(cm_params));
  /*
  从侦听端调用以接受连接或数据报服务查找请求。与套接字接受例程不同，rdma_accept 不会在侦听的 rdma_cm_id 上调用。 相反，在调用 rdma_listen 之后，用户等待 RDMA_CM_EVENT_CONNECT_REQUEST 事件发生。 连接请求事件给用户一个新创建的 rdma_cm_id，类似于新的套接字，但 rdma_cm_id 绑定到特定的 RDMA 设备。 在新的 rdma_cm_id 上调用 rdma_accept。
  以下属性用于配置通信，并在接受连接或数据报通信请求时由 conn_param 参数指定。 用户应使用连接请求事件中报告的 rdma_conn_param 值来确定接受时这些字段的适当值。 用户可以直接引用连接事件中的rdma_conn_param结构，也可以引用自己的结构。 如果引用了来自事件的 rdma_conn_param 结构，则在此调用返回之前不得确认该事件。
  */
  TEST_NZ(rdma_accept(id, &cm_params));  // rdma_accept - 调用以接受连接请求。参考: https://linux.die.net/man/3/rdma_accept

  return 0;
}

int on_connection(void *context)
{
  struct connection *conn = (struct connection *)context;
  struct ibv_send_wr wr, *bad_wr = NULL;  // 发送工作请求
  struct ibv_sge sge;

  snprintf(conn->send_region, BUFFER_SIZE, "message from passive/server side with pid %d", getpid());

  printf("connected. posting send...\n");

  memset(&wr, 0, sizeof(wr));

  wr.opcode = IBV_WR_SEND;
  wr.sg_list = &sge;
  wr.num_sge = 1;
  wr.send_flags = IBV_SEND_SIGNALED;

  sge.addr = (uintptr_t)conn->send_region;
  sge.length = BUFFER_SIZE;
  sge.lkey = conn->send_mr->lkey;

  TEST_NZ(ibv_post_send(conn->qp, &wr, &bad_wr));  //提交工作请求到队列, 如果设置了 IBV_SEND_INLINE 标志，则可以在调用结束后, 立即重用数据缓冲区

  return 0;
}

int on_disconnect(struct rdma_cm_id *id)
{
  struct connection *conn = (struct connection *)id->context;

  printf("peer disconnected.\n");

  rdma_destroy_qp(id);

  ibv_dereg_mr(conn->send_mr);
  ibv_dereg_mr(conn->recv_mr);

  free(conn->send_region);
  free(conn->recv_region);

  free(conn);

  rdma_destroy_id(id);

  return 0;
}

int on_event(struct rdma_cm_event *event)
{
  int r = 0;

  /*
  这个调用就是作用在第一步创建的event channel上面，要从event channel中获取一个事件。这是个阻塞调用，只有有事件时才会返回。在一切正常的情况下，函数返回时会得到一个 RDMA_CM_EVENT_CONNECT_REQUEST事件，也就是说，有客户端发起连接了。
  在事件的参数里面，会有一个新的rdma_cm_id传入。这点和socket是不同的，socket只有在accept后才有新的socket fd创建
  */
  if (event->event == RDMA_CM_EVENT_CONNECT_REQUEST)  // 1.连接请求
    r = on_connect_request(event->id);
  else if (event->event == RDMA_CM_EVENT_ESTABLISHED)  //2. 连接建立
    r = on_connection(event->id->context);
  else if (event->event == RDMA_CM_EVENT_DISCONNECTED)  //连接断开
    r = on_disconnect(event->id);
  else
    die("on_event: unknown event.");

  return r;
}

