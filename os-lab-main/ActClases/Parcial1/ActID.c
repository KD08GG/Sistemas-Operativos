#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    printf("Mi ID = %d\n", getpid());
    printf("ID de mi padre = %d\n", getppid());

    return 0;
}