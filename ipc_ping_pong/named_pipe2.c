#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int fd1 = 0;
    char *myfifo = "myfifo";
    char str1[80];
    char str2[80];

    mkfifo(myfifo, 0666);

    while (1)
    {
        fd1 = open(myfifo,O_RDONLY);
        read(fd1, str1, 80);

        printf("User1: %s\n", str1);
        close(fd1);

        fd1 = open(myfifo,O_WRONLY);
        sprintf(str2, "Pong");
        write(fd1, str2, strlen(str2)+1);
        close(fd1);
    }
    return 0;
}