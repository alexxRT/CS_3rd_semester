#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main ()
{
    umask (0);

    int f_descriptor = open ("./test.txt", O_RDONLY, 0777);
    char message[100];

    read (f_descriptor, message, 12);
    printf ("%s\n", message);

    close (f_descriptor);

    return 0;
}