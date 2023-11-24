
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


typedef struct bulk_info {
    uint64_t addr;
    uint64_t len;
    uint32_t rkey;
} bulk_info_t;


typedef struct ec_io_desc {
    uint64_t ec_full_strip_num;
    uint64_t ec_bulk_start;
    uint64_t ec_parity_bulk_start;
} ec_io_desc_t;

typedef struct obj_wr_in {
    uint8_t bulk_sges_num;
    bulk_info_t *bulk_info;
    ec_io_desc_t *ec_info;
} obj_wr_in_t;

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
    int obj_off = 1024 * 6; // 写IO在对象内的偏移
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

    DEBUG("用户SGE个数:%d, 用户SGL总长:%d(%d)", user_sgl->sges_num, user_sgl_len, user_sgl_len/1024);

    // 对obj_off以8k为准, 向上对齐, 输入6K, 输出8K
    int align_off =  ALIGN_UP(obj_off, ALIGN_8K);
    int ec_start_off = align_off - obj_off; // 按8k对齐后, 在用户SGL中的偏移(EC满条带起始偏移)

    // 剩余长度
    int sgl_remain_len =  user_sgl_len - ec_start_off;
    DEBUG("用户SGL长度(%dK) - EC起始位置偏移(%dK) = 用户SGL剩余长度(%dK)", user_sgl_len/1024, ec_start_off/1024, sgl_remain_len/1024);

    // 剩下的长度大于满条带
    if (sgl_remain_len >= ec_full_strip_size)
    {
        /* 计算满条带个数和总长度 */
        int ec_full_strip_num = sgl_remain_len / ec_full_strip_size;
        int ec_full_strip_len = (sgl_remain_len / ec_full_strip_size) * ec_full_strip_size;
        DEBUG("EC满条带个数:%d, 长度:%dK", ec_full_strip_num, ec_full_strip_len/1024);
    } else {
        /* 非直通 */
    }

    return 0;
}