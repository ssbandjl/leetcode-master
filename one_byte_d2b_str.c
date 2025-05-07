#include <stdio.h>

void decimalToBinary(int n, char *binaryStr) {
    int i;
    for (i = 31; i >= 0; i--) {
        binaryStr[31 - i] = (n & (1 << i)) ? '1' : '0';
    }
    binaryStr[32] = '\0';  // Null-terminate the string
}

int main() {
    int number = 128;  // Example decimal number
    char binaryStr[33];  // 32 bits + 1 for null terminator

    decimalToBinary(number, binaryStr);

    printf("Binary representation of %d is: %s\n", number, binaryStr);

    return 0;
}