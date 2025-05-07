

#include <stdio.h>
#include <stdlib.h>

#include <linux/stddef.h>

#include <stdio.h>

void hexToBinary(unsigned int num) {
    // Iterate through each bit
    int i;

    for (i = sizeof(num) * 8 - 1; i >= 0; i--) {
        // Print the current bit (either 0 or 1)
        int bit = (num >> i) & 1;
        if ((i + 1) % 8 == 0 && i != (sizeof(num) * 8 - 1) && (i < 15)) {
            printf(" ");
        }
        if (i < 5)
            printf("%d", bit);
    }
    printf("\n");
}

int main() {
    // unsigned int hexValue;
    char hexString[20];
    int decimalNumber = 8000; 

    sprintf(hexString, "%x", decimalNumber);

    // Example: Taking a hex input from the user
    // printf("Enter a hexadecimal number (without 0x prefix): ");
    // scanf("%x", &hexValue);

    // printf("Binary equivalent: ");
    // hexToBinary((unsigned char)*hexString);
    hexToBinary(128);

    return 0;
}

// gcc -o main hex2binary.c;./main