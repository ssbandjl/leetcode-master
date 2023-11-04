
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define FFL_FMT              "%s() %s:%d"
#define FFL                  __FUNCTION__,__FILE__,__LINE__

#define NOW \
    do { \
        time_t timer; \
        char buffer[26]; \
        struct tm *tm_info; \
        timer = time(NULL); \
        tm_info = localtime(&timer); \
        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info); \
        printf("%s ", buffer); \
    } while (0)
#define DEBUG(fmt, ...) \
do { \
    NOW; \
    printf(fmt, ##__VA_ARGS__); \
    printf(" "FFL_FMT"\n", FFL); \
} while (0)

/* Unsigned.  */
typedef unsigned char		    uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		    uint32_t;
typedef unsigned long int	  uint64_t;
typedef unsigned long long  khint64_t;
/* 公共部分 END */

typedef struct zmem_sge {
    void *addr;
    uint32_t len;
    uint32_t offset;
} zmem_sge_t;
typedef struct zmem_sgl {
    zmem_sge_t buf[32];
    zmem_sge_t *sges;
    uint32_t sges_num;
} zmem_sgl_t;


#define SGE_NUM (5)
#define ALIGN_8K (8192)

/* 所以对A>1、B>1的整数A、B都有：UP(A/B)=int((A+B-1)/B) */
#define ALIGN_UP(n, align) (((n) + ((align) - 1)) / (align) * (align))


/* Macros for min/max.  */
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/** get remaining space in an iov, assuming that iov_len is used and
 * iov_buf_len is total in buf
 */
#define daos_iov_remaining(iov) ((iov).iov_buf_len > (iov).iov_len ? \
				(iov).iov_buf_len - (iov).iov_len : 0)

/** Move to next iov, it's caller's responsibility to ensure the idx boundary 
 * 计算SGL中下一个内存块的索引和偏移量
*/
#define daos_sgl_next_iov(iov_idx, iov_off)				\
	do {								\
		(iov_idx)++;						\
		(iov_off) = 0;						\
	} while (0)
/** Get the leftover space in an iov of sgl 
 * 计算SGL中该位置剩余的buf长度
*/
#define daos_iov_left(sgl, iov_idx, iov_off)				\
	((sgl)->sges[iov_idx].len - (iov_off))
/**
 * Move sgl forward from iov_idx/iov_off, with move_dist distance. It is
 * caller's responsibility to check the boundary.
 * 移动到SGL指定的位置, 移动距离(范围)为move_dist
 * 输入,sgl, move_dist
 * 输出: iov_idx, iov_off
 */
#define daos_sgl_move(sgl, iov_idx, iov_off, move_dist)			       \
	do {								       \
		uint64_t moved = 0, step, iov_left;			       \
		if ((move_dist) <= 0)					       \
			break;						       \
		while (moved < (move_dist)) {				       \
			iov_left = daos_iov_left(sgl, iov_idx, iov_off);       \
			step = MIN(iov_left, (move_dist) - moved);	       \
			(iov_off) += step;				       \
			moved += step;					       \
			if (daos_iov_left(sgl, iov_idx, iov_off) == 0)	       \
				daos_sgl_next_iov(iov_idx, iov_off);	       \
		}							       \
		assert(moved == (move_dist));				       \
	} while (0)

// void *daos_sgl_move_fun(zmem_sgl_t sgl, int iov_idx, int iov_off, int move_dist)			
// 		uint64_t moved = 0, step, iov_left;			       \
// 		if ((move_dist) <= 0)					       \
// 			break;						       \
// 		while (moved < (move_dist)) {				       \
// 			iov_left = daos_iov_left(sgl, iov_idx, iov_off);       \
// 			step = MIN(iov_left, (move_dist) - moved);	       \
// 			(iov_off) += step;				       \
// 			moved += step;					       \
// 			if (daos_iov_left(sgl, iov_idx, iov_off) == 0)	       \
// 				daos_sgl_next_iov(iov_idx, iov_off);	       \
// 		}							       \
// 		assert(moved == (move_dist));				       \

// gcc -o main example.c;./main
int main(){
    printf("\n");
    zmem_sge_t user_sge[SGE_NUM];
    zmem_sgl_t *user_sgl;
    int obj_off = 1024 * 6; // 写对象, 从对象内偏移6K处写
    int ec_cell_size = 1024 * 64;
    int ec_full_strip_size = 1024 * 64 * 4; // ec 4+2
    int i = 0;
    int buf_len_arr[SGE_NUM] = {1024, 1024*2, 1024*10, 1024*2666, 1024*3};
    char *buf = NULL;

    for (i = 0; i < SGE_NUM; i++){
        buf = (void *)malloc(buf_len_arr[i]);
        if (buf == NULL){
            DEBUG("malloc buf failed");
            return -1;
        }
        user_sge[i].addr = buf;
        user_sge[i].len = buf_len_arr[i];
        user_sge[i].offset = 0;
    }


    user_sgl = (zmem_sgl_t *)malloc(sizeof(zmem_sgl_t));
    user_sgl->sges = user_sge;
    user_sgl->sges_num = SGE_NUM;
    /* 计算sgl总长度 */
    int user_sgl_len = 0;
    for(i = 0; i < SGE_NUM; i++)
    {
        user_sgl_len += user_sgl->sges[i].len;
    }

    DEBUG("用户SGE个数 user_sges_num:%d, 用户SGL总长 user_sgl_len:%d(%d)", user_sgl->sges_num, user_sgl_len, user_sgl_len/1024);
    // 对obj_off以8k为准, 向上对齐
    int align_off =  ALIGN_UP(obj_off, ALIGN_8K);
    int align_off_diff = align_off - obj_off; // 按8k对齐后, 在用户SGL中的偏移(EC满条带起始地址)

    // 剩余长度
    int sgl_remain_len =  user_sgl_len - align_off_diff;
    DEBUG("user_sgl_len(%d) - 左对齐后相对用户SGE的偏移align_off_diff(%d) = 对齐后SGL剩余长度sgl_remain_len(%d)", user_sgl_len/1024, align_off_diff/1024, sgl_remain_len/1024);

    
    // 判断剩下的长度是否大于满条带
    if (sgl_remain_len >= ec_full_strip_size)
    {
        // 计算ec_sge的长度和起始位置
        int sge_len_sum = 0;
        int ec_sge_num = 0;
        void *ec_sge_start_addr;
        int ec_sge_start_len;
        int ec_start_sge;
        int ec_start_sge_idx = 0;
        for (i = 0; i < SGE_NUM; i++)
        {
            /* if sum = off, then break, record sge_idx, ec_first_sge_addr, ec_first_sge_len */
            if (sge_len_sum >= align_off_diff)
            {
                ec_sge_start_len = sge_len_sum - user_sgl->sges[i-1].len;
                ec_sge_start_addr = user_sgl->sges[i-1].addr + (user_sgl->sges[i-1].len - ec_sge_start_len);
                ec_start_sge_idx = i-1;
                DEBUG("EC相对用户的起始SGE ec start in user_sge[%d], EC起始SGE长度 ec_sge_len:%d, 用户SGE地址 user_sge_addr:%p, ECS_SGE起始地址 ec_sge_addr:%p", 
                    i-1, ec_sge_start_len, user_sgl->sges[i-1].addr, ec_sge_start_addr);
                break;
            }
            sge_len_sum += user_sgl->sges[i].len;
        }

        /* 计算满条带个数和总长度 */
        int ec_full_strip_num = sgl_remain_len / ec_full_strip_size;
        int ec_full_strip_len = (sgl_remain_len / ec_full_strip_size) * ec_full_strip_size;
        DEBUG("左对齐后, 剩余SGL中包含的满条带个数:%d", ec_full_strip_num);
        DEBUG("满条带总长度:%d", ec_full_strip_len/1024);

        /* 
        先计算出, 左对齐后, 剩余长度包含多少个满条带(满条带总长度(下面需要移动的距离=2560(10个满条带)))
        需求: 移动到SGL的SGE[1], 偏移为2K, 移动的距离为满条带位置()
         */
        int iov_idx = 0;
        int iov_off = 0;
        int move_dist = 1024 * 2;
        daos_sgl_move(user_sgl, iov_idx, iov_off, move_dist);
        DEBUG("start iov_idx:%d, iov_off:%d, move_dist:%d", iov_idx, iov_off, move_dist);

        move_dist = 1024 * 2560;
        daos_sgl_move(user_sgl, iov_idx, iov_off, move_dist);
        DEBUG("end iov_idx:%d, iov_off:%d, move_dist:%d", iov_idx, iov_off/1024, move_dist);



        /* 倒序, 找尾部 */
        int user_sgl_delete_ec_full_remain_size = sgl_remain_len % ec_full_strip_size;
        DEBUG("用户SGL去除满条带后, 右边剩余的长度 user_sgl_delete_ec_full_remain_size:%d(%d)", user_sgl_delete_ec_full_remain_size, user_sgl_delete_ec_full_remain_size / 1024);
        int ec_end_sge_right_len = 0;
        int ec_end_sge_idx = 0;
        int ec_end_sge_len = 0;
        sge_len_sum = 0;
        for (i = (SGE_NUM - 1); i > 0; i--)
        {
            if (sge_len_sum >= user_sgl_delete_ec_full_remain_size){
                ec_end_sge_len = (user_sgl->sges[i+1].len - ec_end_sge_right_len);
                ec_end_sge_idx = i+1;
                DEBUG("ec end in user_sge[%d], ec_end_sge_right_len:%d(%d), ec_end_sge_len:%d(%d)", 
                    i+1, ec_end_sge_right_len, ec_end_sge_right_len/1024, ec_end_sge_len, ec_end_sge_len/1024);
                break;
            }
            sge_len_sum += user_sgl->sges[i].len;
            ec_end_sge_right_len = user_sgl_delete_ec_full_remain_size - user_sgl->sges[i+1].len;
        }
        int ec_sge_nums = ec_end_sge_idx + 1 - ec_start_sge_idx;
        DEBUG("ec in user_sge[%d-%d], nums:%d", ec_start_sge_idx, ec_end_sge_idx, ec_sge_nums);

        /* 构造ec_sgl */
        zmem_sgl_t *ec_sgl;
        ec_sgl = (zmem_sgl_t *)malloc(sizeof(zmem_sgl_t) * ec_sge_nums);
        zmem_sge_t ec_sge[ec_sge_nums];
        int ec_sge_idx = 0;
        for (i = ec_start_sge_idx; ec_sge_idx < ec_sge_nums; i++)
        {
            if (i == ec_start_sge_idx)
            {
                 ec_sge[ec_sge_idx].addr = ec_sge_start_addr;
                 ec_sge[ec_sge_idx].len = ec_sge_start_len;
            } else if (i == ec_end_sge_idx) {
                ec_sge[ec_sge_idx].addr = user_sgl->sges[i].addr;
                ec_sge[ec_sge_idx].len = ec_end_sge_len;
            } else {
                ec_sge[ec_sge_idx].addr = user_sgl->sges[i].addr;
                ec_sge[ec_sge_idx].len = user_sgl->sges[i].len;
            }
            ec_sge[ec_sge_idx].offset = 0;
            ec_sge_idx++;
        }
        ec_sgl->sges = ec_sge;
        ec_sgl->sges_num = ec_sge_nums;

        /* 打印结果 */
        for (i=0; i < SGE_NUM; i++)
        {
            DEBUG("user_sge[%d], addr:%p, len:%d(%d)", i, user_sge[i].addr, user_sge[i].len, user_sge[i].len / 1024);
        }
        DEBUG("-----------------------");
        for (i=0; i < ec_sge_nums; i++)
        {
            DEBUG("ec_sge[%d], addr:%p, len:%d(%d)", i, ec_sge[i].addr, ec_sge[i].len, ec_sge[i].len / 1024);
        }
    }



    return 0;
}

/*  log:
2023-11-05 01:57:58 用户SGE个数 user_sges_num:5, 用户SGL总长 user_sgl_len:2746368(2682) main() main.c:149
2023-11-05 01:57:58 user_sgl_len(2682) - 左对齐后相对用户SGE的偏移align_off_diff(2) = 对齐后SGL剩余长度sgl_remain_len(2680) main() main.c:156
2023-11-05 01:57:58 EC相对用户的起始SGE ec start in user_sge[1], EC起始SGE长度 ec_sge_len:1024, 用户SGE地址 user_sge_addr:0x557710de2ac0, ECS_SGE起始地址 ec_sge_addr:0x557710de2ec0 main() main.c:177
2023-11-05 01:57:58 左对齐后, 剩余SGL中包含的满条带个数:10 main() main.c:187
2023-11-05 01:57:58 满条带总长度:2560 main() main.c:188
2023-11-05 01:57:58 start iov_idx:1, iov_off:1024, move_dist:2048 main() main.c:198
2023-11-05 01:57:58 end iov_idx:3, iov_off:2549, move_dist:2621440 main() main.c:202
2023-11-05 01:57:58 用户SGL去除满条带后, 右边剩余的长度 user_sgl_delete_ec_full_remain_size:122880(120) main() main.c:208
2023-11-05 01:57:58 ec end in user_sge[3], ec_end_sge_right_len:119808(117), ec_end_sge_len:2610176(2549) main() main.c:218
2023-11-05 01:57:58 ec in user_sge[1-3], nums:3 main() main.c:226
2023-11-05 01:57:58 user_sge[0], addr:0x557710de26b0, len:1024(1) main() main.c:255
2023-11-05 01:57:58 user_sge[1], addr:0x557710de2ac0, len:2048(2) main() main.c:255
2023-11-05 01:57:58 user_sge[2], addr:0x557710de32d0, len:10240(10) main() main.c:255
2023-11-05 01:57:58 user_sge[3], addr:0x7f6566965010, len:2729984(2666) main() main.c:255
2023-11-05 01:57:58 user_sge[4], addr:0x557710de5ae0, len:3072(3) main() main.c:255
2023-11-05 01:57:58 ----------------------- main() main.c:257
2023-11-05 01:57:58 ec_sge[0], addr:0x557710de2ec0, len:1024(1) main() main.c:260
2023-11-05 01:57:58 ec_sge[1], addr:0x557710de32d0, len:10240(10) main() main.c:260
2023-11-05 01:57:58 ec_sge[2], addr:0x7f6566965010, len:2610176(2549) main() main.c:260

user_sgl:
1K, 2K, 10K, 2666K, 3K

ec_sgl:
1K, 10K, 2549K
 */