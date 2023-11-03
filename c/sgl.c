#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>


#define BUF_LEN 16


#define FFL_FMT              "%s() %s:%d"
#define FFL                  __FUNCTION__,__FILE__,__LINE__
#define NOW \
  do  { \
    time_t timer; \
    char buffer[26]; \
    struct tm* tm_info; \
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


static inline int generate_test_string(char *str, int size)
{
    char *tmp_str;
    int i;

    tmp_str = calloc(1, size);

    for (i = 0; i < (size - 1); ++i) {
        tmp_str[i] = 'A' + (i % 26);
    }

    memcpy(str, tmp_str, size);

    free(tmp_str);
    return 0;
}


/** iovec for memory buffer */
typedef struct {
	/** buffer address */
	void		*iov_buf;
	/** buffer length */
	size_t		iov_buf_len;
	/** data length */
	size_t		iov_len;
} d_iov_t;

typedef struct {
	uint32_t	sg_nr;
	uint32_t	sg_nr_out;
	d_iov_t		*sg_iovs;
} d_sg_list_t;


static inline void
d_iov_set(d_iov_t *iov, void *buf, size_t size)
{
	iov->iov_buf = buf;
	iov->iov_len = iov->iov_buf_len = size;
}




// gcc -o main example.c;./main
int main(){
    char  *buf = NULL;
    char  *buf2 = NULL;
    d_iov_t	iov[2];
	d_sg_list_t	sgl;



    buf  = malloc(BUF_LEN);
    buf2 = malloc(BUF_LEN);
    generate_test_string(buf, BUF_LEN);
    generate_test_string(buf2, BUF_LEN);
    DEBUG("buf :%p, buf :%s", buf, buf);
    DEBUG("buf2:%p, buf2:%s", buf2, buf2);

    d_iov_set(&iov[0], buf, BUF_LEN);
    d_iov_set(&iov[1], buf2, BUF_LEN);

    sgl.sg_nr = 2;
	sgl.sg_iovs = iov;
    DEBUG("sgl:%p", &sgl);

}
