#include <stdio.h>
#include <stdlib.h>


#define DTS_CFG_MAX 256
__attribute__ ((__format__(__printf__, 2, 3)))
static inline void
dts_create_config(char buf[DTS_CFG_MAX], const char *format, ...)
{
	va_list	ap;
	int	count;

	va_start(ap, format);
	count = vsnprintf(buf, DTS_CFG_MAX, format, ap);
	va_end(ap);

	if (count >= DTS_CFG_MAX)
		buf[DTS_CFG_MAX - 1] = 0;
}

static int run_single_class_tests(const char *cfg)
{
	char test_name[DTS_CFG_MAX];

	dts_create_config(test_name, "IO single oclass tests %s", cfg);
	printf("Running %s\n", test_name);
}

// gcc -o main main.c;./main
int main(){

    run_single_class_tests();
    return 0;
}

