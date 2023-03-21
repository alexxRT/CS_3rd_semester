#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

typedef struct stat my_stat;

int main ()
{
    my_stat buf = {};
    stat ("./text.txt", &buf);
    size_t old_length = buf.st_size;

    int fd = open ("./text.txt", O_WRONLY);
    ftruncate (fd, strlen(" HELLO WORLD") + old_length);
    printf ("old len is %lu\n", old_length);

    lseek (fd, old_length, SEEK_SET);
    write (fd, " HELLO WORLD", strlen(" HELLO WORLD"));

    close (fd);

    return 0;
}