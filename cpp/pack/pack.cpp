// pragma_directives_pack.cpp
#include <stddef.h>
#include <stdio.h>


/*
1111 1100 1111 1111   = 16B
*/
struct S {
   int i;   // size 4
   short j;   // size 2
   double k;   // size 8
};


/*
1111 1111 1111 11   = 14B
*/
#pragma pack(2)
struct T {
   int i;
   short j;
   double k;
};

int main() {
   printf("%zu ", offsetof(S, i));
   printf("%zu ", offsetof(S, j));
   printf("%zu ", offsetof(S, k));
   printf("%zu\n", sizeof(S));

   printf("%zu ", offsetof(T, i));
   printf("%zu ", offsetof(T, j));
   printf("%zu ", offsetof(T, k));
   printf("%zu\n", sizeof(T));
}
