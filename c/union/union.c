#include <stdio.h>
#include <stdint.h>

typedef union
{
    struct
    {
        uint8_t a;
        uint8_t b;
        uint8_t c;
        uint8_t d;
    };
    uint32_t x;
} somenewtype;

typedef union
{
    uint32_t* p;
    uint8_t* q;
} somepointer;

int main(int argc, char** argv)
{
    uint32_t r;
    uint8_t s;
    somenewtype z;
    somepointer p;
    r = 0x11223344; 
    s = 0x11;
    z.x = 0x11223344;
    p.p = &r;
    p.q = &s;
    printf("%x%x%x%x\n", z.d, z.c, z.b, z.a);
    printf("%x %x\n", *(p.p), *(p.q));
}