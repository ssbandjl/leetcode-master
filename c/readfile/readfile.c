#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
		// 主动断开QP
    char *file_name = "/tmp/control";
		FILE *fp = NULL;
		char control_str[1024] = {'\0'};

		if (access(file_name, F_OK) == 0) {
      printf("find file\n");
			fp = fopen(file_name, "r");
			if (fp) {
        printf("open file\n");
				if(fgets(control_str, 1024, fp)) {
          printf("file content:%s\n", control_str);
					if (strncmp(control_str, "1", 1) == 0)
            printf("disconnect qp\n");
				};
			};
		};
  return 0;
}