
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
#define AIGN_8K (8192)

/* */
# define ALIGN_UP(n, align) (((n) + ((align) - 1)) / (align) * (align))

/* 
所以对A>1、B>1的整数A、B都有：
UP(A/B)=int((A+B-1)/B)
 */


// gcc -o main example.c;./main
int main(){
    zmem_sge_t sge[SGE_NUM];
    zmem_sgl_t *usgl;
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
        sge[i].addr = buf;
        sge[i].len = buf_len_arr[i];
        sge[i].offset = 0;
    }


    usgl = (zmem_sgl_t *)malloc(sizeof(zmem_sgl_t));
    usgl->sges = sge;
    usgl->sges_num = SGE_NUM;
    /* 计算sgl总长度 */
    int usgl_len = 0;
    for(i = 0; i < SGE_NUM; i++)
    {
        usgl_len += usgl->sges[i].len;
    }

    DEBUG("user_sges_num:%d, usgl_len:%d", usgl->sges_num, usgl_len);
    // 对obj_off以8k为准, 向上对齐
    int align_off =  ALIGN_UP(obj_off, AIGN_8K);
    int align_off_diff = align_off - obj_off; // 按8k对齐后, 在用户SGL中的偏移(EC满条带起始地址)

    // 剩余长度
    int sgl_remain_len =  usgl_len - align_off_diff;
    DEBUG("usgl_len(%d) - align_off_diff(%d) = sgl_remain_len(%d)", usgl_len, align_off_diff,sgl_remain_len);

    
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
                ec_sge_start_len = sge_len_sum - usgl->sges[i-1].len;
                ec_sge_start_addr = usgl->sges[i-1].addr + (usgl->sges[i-1].len - ec_sge_start_len);
                ec_start_sge_idx = i-1;
                DEBUG("ec start in user_sge[%d], ec_sge_len:%d, user_sge_addr:%p, ec_sge_addr:%p", 
                    i-1, ec_sge_start_len, usgl->sges[i-1].addr, ec_sge_start_addr);
                break;
            }
            sge_len_sum += usgl->sges[i].len;
        }

        /* 倒序, 找尾部 */
        int user_sgl_delete_ec_full_remain_size = sgl_remain_len % ec_full_strip_size;
        DEBUG("remain:%d(%d)", user_sgl_delete_ec_full_remain_size, user_sgl_delete_ec_full_remain_size / 1024);
        int ec_end_sge_right_len = 0;
        int ec_end_sge_idx = 0;
        int ec_end_sge_len = 0;
        sge_len_sum = 0;
        for (i = (SGE_NUM - 1); i > 0; i--)
        {
            if (sge_len_sum >= user_sgl_delete_ec_full_remain_size){
                ec_end_sge_len = (usgl->sges[i+1].len - ec_end_sge_right_len);
                ec_end_sge_idx = i+1;
                DEBUG("ec end in user_sge[%d], ec_end_sge_right_len:%d(%d), ec_end_sge_len:%d(%d)", 
                    i+1, ec_end_sge_right_len, ec_end_sge_right_len/1024, ec_end_sge_len, ec_end_sge_len/1024);
                break;
            }
            sge_len_sum += usgl->sges[i].len;
            ec_end_sge_right_len = user_sgl_delete_ec_full_remain_size - usgl->sges[i+1].len;
        }
        int ec_sge_nums = ec_end_sge_idx + 1 - ec_start_sge_idx;
        DEBUG("ec[%d-%d], nums:%d", ec_start_sge_idx, ec_end_sge_idx, ec_sge_nums);
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
                ec_sge[ec_sge_idx].addr = usgl->sges[i].addr;
                ec_sge[ec_sge_idx].len = ec_end_sge_len;
            } else {
                ec_sge[ec_sge_idx].addr = usgl->sges[i].addr;
                ec_sge[ec_sge_idx].len = usgl->sges[i].len;
            }
            ec_sge[ec_sge_idx].offset = 0;
            ec_sge_idx++;
        }
        ec_sgl->sges = ec_sge;
        ec_sgl->sges_num = ec_sge_nums;

        /* 打印结果 */
        for (i=0; i < SGE_NUM; i++)
        {
            DEBUG("user_sge[%d], addr:%p, len:%d(%d)", i, sge[i].addr, sge[i].len, sge[i].len / 1024);
        }
        for (i=0; i < ec_sge_nums; i++)
        {
            DEBUG("ec_sge[%d], addr:%p, len:%d(%d)", i, ec_sge[i].addr, ec_sge[i].len, ec_sge[i].len / 1024);
        }
    }




    return 0;
}

/*  log:
root@ubuntu22:~/project/dev/leetcode/leetcode-master# ./main.sh 
2023-11-04 23:08:51 user_sges_num:5, usgl_len:2746368 main() main.c:93
2023-11-04 23:08:51 usgl_len(2746368) - align_off_diff(2048) = sgl_remain_len(2744320) main() main.c:100
2023-11-04 23:08:51 ec start in user_sge[1], ec_sge_len:1024, user_sge_addr:0x557ca96156b0, ec_sge_addr:0x557ca9615ab0 main() main.c:121
2023-11-04 23:08:51 remain:122880(120) main() main.c:130
2023-11-04 23:08:51 ec end in user_sge[3], ec_end_sge_right_len:119808(117), ec_end_sge_len:2610176(2549) main() main.c:140
2023-11-04 23:08:51 ec[1-3], nums:3 main() main.c:148
2023-11-04 23:08:51 user_sge[0], addr:0x557ca96152a0, len:1024(1) main() main.c:175
2023-11-04 23:08:51 user_sge[1], addr:0x557ca96156b0, len:2048(2) main() main.c:175
2023-11-04 23:08:51 user_sge[2], addr:0x557ca9615ec0, len:10240(10) main() main.c:175
2023-11-04 23:08:51 user_sge[3], addr:0x7fb8e0365010, len:2729984(2666) main() main.c:175
2023-11-04 23:08:51 user_sge[4], addr:0x557ca96186d0, len:3072(3) main() main.c:175
2023-11-04 23:08:51 ec_sge[0], addr:0x557ca9615ab0, len:1024(1) main() main.c:179
2023-11-04 23:08:51 ec_sge[1], addr:0x557ca9615ec0, len:10240(10) main() main.c:179
2023-11-04 23:08:51 ec_sge[2], addr:0x7fb8e0365010, len:2610176(2549) main() main.c:179

user_sgl:
1K, 2K, 10K, 2666K, 3K

ec_sgl:
1K, 10K, 2549K
 */