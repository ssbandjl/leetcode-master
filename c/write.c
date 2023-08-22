#include <stdio.h>
#include <stdlib.h>

int main() {
    char sentence[1000] = "hello,world";

    // creating file pointer to work with files
    FILE *fptr;
    // opening file in writing mode
    fptr = fopen("program.txt", "w");
    // exiting program 
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }
    // printf("Enter a sentence:\n");
    // fgets(sentence, sizeof(sentence), stdin);

    fprintf(fptr, "%s", sentence);
    fclose(fptr);
    return 0;
}

/*
tee<<EOF>write.c
#include <stdio.h>
#include <stdlib.h>

int main() {
    char sentence[1000] = "hello,world";

    // creating file pointer to work with files
    FILE *fptr;
    // opening file in writing mode
    fptr = fopen("program.txt", "w");
    // exiting program 
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }
    // printf("Enter a sentence:\n");
    // fgets(sentence, sizeof(sentence), stdin);

    fprintf(fptr, "%s", sentence);
    fclose(fptr);
    return 0;
}
EOF

gcc -o write write.c && ./write

*/