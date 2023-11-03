#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>



/* 打印当前时间, 函数名,文件名,行号 START */
#define FFL_FMT              "%s() %s:%d"
#define FFL                  __FUNCTION__,__FILE__,__LINE__
#define NOW \
    do { \
        time_t timer; \
        char buffer[26]; \
        struct tm* tm_info; \
        timer = time(NULL); \
        tm_info = localtime(&timer); \
        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info); \
        printf("%s ", buffer); \
	} while (0)
#define LOG(fmt, ...) \
do { \
  NOW; \
  printf(fmt, ##__VA_ARGS__); \
  printf(" "FFL_FMT"\n", FFL); \
} while (0)
/* 打印当前时间, 函数名,文件名,行号 END */



#define D_ALLOC(ptr, size)	D_ALLOC_CORE(ptr, size, 1)
#define D_ALLOC_PTR(ptr)	D_ALLOC(ptr, sizeof(*ptr))

#define D_ALLOC_CORE(ptr, size, count)                                                             \
	do {                                                                                       \
		(ptr) = (__typeof__(ptr))d_calloc((count), (size));                                \
		D_CHECK_ALLOC(calloc, true, ptr, #ptr, size, count, #count, 0);                    \
	} while (0)




void * d_calloc(size_t count, size_t size)
{
	void *ptr;
	ptr = calloc(count, size);
	return ptr;
}

#define D_CHECK_ALLOC(func, cond, ptr, name, size, count, cname,	\
			on_error)					\
	do {								\
		if ((cond) && (ptr) != NULL) {				\
			if ((count) <= 1)				\
				LOG("DEBUG "				\
					"alloc(" #func ") '" name	\
					"':%i at %p.",		\
					(int)(size), (ptr));		\
			else						\
				LOG("DEBUG "				\
					"alloc(" #func ") '" name	\
					"':%i * '" cname "':%i at %p.", \
					(int)(size), (int)(count), (ptr)); \
			break;						\
		}							\
		(void)(on_error);					\
		if ((count) >= 1)					\
			LOG("out of memory (tried to "		\
				#func " '" name "':%i)",		\
				(int)((size) * (count)));		\
		else							\
			LOG("out of memory (tried to "		\
				#func " '" name "':%i)",		\
				(int)(size));				\
	} while (0)

typedef struct daos_eq {
	int			eq_n_comp;
	int			eq_n_running;
	int			use_lock;
	int			is_polling;
	int			eq_fd;
} daos_eq_t;



int main () {
    struct daos_eq         *eq;
    D_ALLOC_PTR(eq);
	if (eq == NULL) {
        LOG("alloc eq failed");
	    return -1;
    }
    eq->eq_fd = 111;
    LOG("alloc eq success, eq:%p, eq->eq_fd:%d", eq, eq->eq_fd);
    return 0;
}