#include <stdio.h>

void decimalToBinary(unsigned char decimal) {
    // Iterate through each bit of the 1-byte number
    for (int i = 7; i >= 0; i--) {
        // Print the bit value (either 0 or 1)
        printf("%d", (decimal >> i) & 1);
    }
    printf("\n");
}


// gcc -o main one_byte_d2b.c;./main
int main() {
    unsigned char decimal = 128;  // Example: Maximum value for a 1-byte number

    // printf("Decimal: %d\n", decimal);
    // printf("Binary: ");
    decimalToBinary(decimal);

    // Test with another example
    // decimal = 65;  // ASCII value for 'A'
    // printf("Decimal: %d\n", decimal);
    // printf("Binary: ");
    // decimalToBinary(decimal);

    return 0;
}