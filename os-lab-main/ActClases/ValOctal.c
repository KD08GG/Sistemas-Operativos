#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

// This function writes a value in the specified file
int main()
{
    // fd is the file descriptor
    int fd, n;
    float value = 47;

    fd = creat("datos.txt", 0777);

    n = write(fd, &value, sizeof(value));

    printf("%d\n", n);
    close(fd);
    return 0;
}