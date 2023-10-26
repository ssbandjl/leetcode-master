#include <stdio.h>
#include <time.h>

#define UCS_PP_QUOTE(x)                   # x
#define UCT_IB_MD_NAME(_x)        "ib_" UCS_PP_QUOTE(_x)


#define CONTACT(x,y) x##y   //拼接
#define STR(x) #x   //转字符串

//二级
#define CONTACT2(x,y) CONTACT(x,y)
#define STR2(x) STR(x) 



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

#define FFL_FMT_NOW   CONTACT2(NOW, FFL_FMT)

#define DEBUG(fmt, ...) \
do { \
  NOW; \
  printf(fmt, ##__VA_ARGS__); \
  printf(" "FFL_FMT"\n", FFL); \
} while (0)

int main () {

   DEBUG("hello:%d", 100);
   
   return 0;
}