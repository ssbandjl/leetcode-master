#include <stdio.h>

struct animal_struct {
    unsigned long cat;   /* 4B */
    unsigned short pig;  /* 2B */
    char dog;            /* 1B */
    char forx;           /* 1B */
};



// gcc -o Wpadded main example.c;./main
int main(){
    
    printf("sizeof animal:%ld\n", sizeof(struct animal_struct)); // sizeof animal:24, gcc -o main -Wpadded main.c
    return 0;
}