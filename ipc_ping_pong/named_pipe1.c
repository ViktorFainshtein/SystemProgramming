#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main()
{
    int fd = 0;
    char *myfifo = "myfifo";
    char arr1[80];
    char arr2[80];

    mkfifo(myfifo, 0666);

    while (1)
    {
        fd = open(myfifo, O_WRONLY);

        sprintf(arr2, "Ping");

        write(fd, arr2, strlen(arr2)+1);
        close(fd);

        fd = open(myfifo, O_RDONLY);

        read(fd, arr1, sizeof(arr1));

        printf("User2: %s\n", arr1);
        close(fd);
    }

    return 0;
}