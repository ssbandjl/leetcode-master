#include<dlog.h>
#define TAG "MY_APP"

int main(void)
{
   int integer = 21;
   char string[] = "test dlog";

   dlog_print(DLOG_DEBUG, TAG, "debug message");
   dlog_print(DLOG_INFO, TAG, "info message");
   dlog_print(DLOG_WARN, TAG, "warning message");
   dlog_print(DLOG_ERROR, TAG, "error message");
   dlog_print(DLOG_INFO, TAG, "%s, %d", string, integer);

   return 0;
}
