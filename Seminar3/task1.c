#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main ()
{
    umask (0);

    int f_descriptor = open ("./test.txt", O_WRONLY | O_CREAT, 0777);
    const char* message = "Hello world";

    write (f_descriptor, message, 12);

    close (f_descriptor);

    return 0;
}