#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#define FFL_FMT              "%s %s:%d"
#define FFL                  __FUNCTION__,__FILE__,__LINE__


void d_vlog(const char *fmt, va_list ap)
{
  fprintf(stdout, "%s "FFL_FMT" \n", fmt, FFL);
}

static inline void d_log(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	d_vlog(fmt, ap);
	va_end(ap);
}



#define PRINT(fmt, ...)				    \
	do {                            \
	  printf(fmt##FFL_FMT, ##__VA_ARGS__, FFL);	    \
    fprintf(stdout, fmt##FFL_FMT, fmt, FFL); \
	} while (0)


// gcc -o main main.c;./main
int main(){
    char path[4096];
    char line[4096];
    char *ret;
    int ret_length;
    FILE *fp;
    char *allowed_rest;
    char *token;
    int token_count;
    int i;
    int pid = getpid();

    snprintf(path, sizeof(path), "/proc/%d/status", pid);   /* 获取pid对应的/proc/pid/status路径 */

    fp = fopen(path, "r");          
    if (NULL == fp)                                             /* 检查文件是否存在*/    
    {
        printf("failed to open /proc/%d/status\n");
        return NULL;
    } 

    while(fgets(line, sizeof(line), fp) != NULL)            //解析Cpus_allowed_list
    {
        if (sscanf(line, "Cpus_allowed: %[^\n]", line) == 1)
        {
            puts(line);
            break;
        }
    }
    fclose(fp);

    if(NULL == line)
    {
        printf("/proc/%d/status has no item [Cpus_allowed]\n", pid);
        return NULL;
    }    
    
    token_count = 0;
    // allowed_rest = line;
    allowed_rest = NULL;
    token = strtok_r(line, ",", &allowed_rest);   
    printf("1token: %s\n", token);
    while (NULL != token)
    {   
        token_count++;
        token = strtok_r(NULL, ",", &allowed_rest);
        printf("1token: %s\n", token);
    }
    printf("token count %d\n", token_count);

    ret_length = token_count*8;
    ret = (char *)calloc(1, ret_length + 1);        /* 分配对应长度的内存 */
    ret[0] = 0;

    printf("ret length %d\n", ret_length);

    allowed_rest = NULL;
    token = strtok_r(line, ",", &allowed_rest);   
    i = 1;
    strncat(ret, token, 8);
    printf("token: %s, i=%d\n", token, i);
    // while (i < token_count)
    // {   
    //     i++;
    //     token = strtok_r(NULL, ",", &allowed_rest);
    //     printf("token: %s, i=%d\n", token, i);
    //     strncat(ret, token, 8);
    // }
    while(NULL != token)
    {
        i++;
        token = strtok_r(NULL, ",", &allowed_rest);
        printf("token: %s, i=%d\n", token, i);
    }

    ret[ret_length] = 0;

    printf("%d has cpu_list %s\n", pid, ret);
    return ret;



  return 0;
}