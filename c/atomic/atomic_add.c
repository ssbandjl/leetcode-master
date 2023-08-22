#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>
 
atomic_int acnt;
int cnt;
 
int f(void* thr_data)
{
    int ret = 0;
    for(int n = 0; n < 10; ++n) {
        ret=atomic_fetch_add_explicit(&acnt, 1, memory_order_relaxed); // 原子的
        printf("ret:%d, cnt:%d\n", ret, cnt);
        ++cnt; // 未定义行为，实际上会失去一些更新
    }
    return 0;
}
 
int main(void)
{
    thrd_t thr[10];
    for(int n = 0; n < 10; ++n)
        thrd_create(&thr[n], f, NULL);
    for(int n = 0; n < 10; ++n)
        thrd_join(thr[n], NULL);
 
    printf("The atomic counter is %u\n", acnt);
    printf("The non-atomic counter is %u\n", cnt);
}
// ————————————————
// 版权声明：本文为CSDN博主「confirmwz」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
// 原文链接：https://blog.csdn.net/armlinuxww/article/details/103096636