

/* 公共部分 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// gcc -o main main.c;./main
int main(){
    char *char_ptr = NULL;
    free(char_ptr);
    free(char_ptr);
    printf("double free tes\n");
    return 0;
}