#include <stdio.h>
#include <string.h>
#include <time.h>

// #define INFO(msg, ...) \
//     fprintf(stderr, "info: %s:%d: ", __FILE__, __LINE__); \
//     fprintf(stderr, msg)

// #ifndef __FILENAME__
// #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
// #endif

// #define STRX(x) #x
// #define STR(x) STRX(x)

// #ifndef __FFL__
// #define __FFL__ __FUNCTION__ " " STR(__FILENAME__) ":" STR(__LINE__)
// #endif



#ifdef WIN32
#define TrimFilePath(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
#else //*nix
#define TrimFilePath(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#endif
 
#define dprint(fmt, ...)   \
    time_t timer; \
    char buffer[26]; \
    struct tm* tm_info; \
    timer = time(NULL); \
    tm_info = localtime(&timer); \
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info); \
    printf("[%s] "fmt" %s %s:%d\n", buffer, ##__VA_ARGS__, __FUNCTION__, TrimFilePath(__FILE__) ,__LINE__)

// gcc -o main ffl.c;./main
int main() {
    // time_t timer;
    // char buffer[26];
    // struct tm* tm_info;

    // timer = time(NULL);
    // tm_info = localtime(&timer);

    // strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    // puts(buffer);

    char a[120] = "jkdajgld";
    // printf("%s, %s\n", __FFL__, __FILENAME__);
    // printf("%#x\n", a);
    // INFO("hello:%s\n", a);
    dprint("%s", a);
    return 0;
}
