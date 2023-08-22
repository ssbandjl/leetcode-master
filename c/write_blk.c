#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
       char buff[128] = {0};
       int fd = open("write.txt", O_CREAT|O_RDWR);
       write(fd, "pilgrimtao is cool", 18);
       pread(fd, buff, 128, 0);
       printf("%s\n", buff);
       close(fd);
       return 0;
}
