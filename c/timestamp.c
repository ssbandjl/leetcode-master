#include <stdio.h>
#include <time.h> 
#include <string.h>      


char * timestamp();

#define print_log(f_, ...) printf("%s ", timestamp()), printf((f_), ##__VA_ARGS__), printf("\n")



// gcc -o main timestamp.c;./main
int main(int argc, char* argv[]) {
    print_log("Hello");
    print_log("%s%d","mokumus",1996);


    time_t t = time(NULL);
    printf("time:%s\n", ctime(&t));

    return 0;
}

char * timestamp(){
    time_t now = time(NULL); 
    // char * time = asctime(gmtime(&now));
    char * time = ctime(&now);
    time[strlen(time)-1] = '\0';    // Remove \n
    return time;
}
