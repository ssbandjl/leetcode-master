#include <stdio.h>
#include <arpa/inet.h>
#include <stdint.h>

// Implementing htonll manually
uint64_t htonll(uint64_t value) {
    if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) {
        // Swap bytes manually if the system is little-endian
        return ((value & 0x00000000000000FF) << 56) |
               ((value & 0x000000000000FF00) << 40) |
               ((value & 0x0000000000FF0000) << 24) |
               ((value & 0x00000000FF000000) << 8)  |
               ((value & 0x000000FF00000000) >> 8)  |
               ((value & 0x0000FF0000000000) >> 24) |
               ((value & 0x00FF000000000000) >> 40) |
               ((value & 0xFF00000000000000) >> 56);
    } else {
        return value;  // No conversion needed if already big-endian
    }
}

int main() {
    uint64_t little_endian_64 = 0x1234567890ABCDEF; // Example little-endian 64-bit number

    uint64_t big_endian_64 = htonll(little_endian_64);

    printf("Original 64-bit little-endian: 0x%lx\n", little_endian_64);
    printf("Converted 64-bit big-endian: 0x%lx\n", big_endian_64);

    return 0;
}


// int main()
// {
//  	unsigned long a = 0x1122334455667788;
// 	printf("%lx, %lx\n", a&0xFF, (a >> 8)&0xFF);

// 	printf("%lx, %x\n", htonll(a) & 0xFF, (a >> 8)&0xFF);
// 	/*  Write C code in this online editor and run it. */
 
//    return 0;
// }

// gcc -o main switch_big_and_little_endian.c;./main