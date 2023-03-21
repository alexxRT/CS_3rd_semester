#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define MAX_SIZE 100


typedef struct stat my_stat;

int main ()
{

    my_stat buf = {};
    int error = stat ("file.txt", &buf);

    assert (error == 0);

    int file = open ("file.txt", O_RDWR);
    
    size_t file_size = buf.st_size;

    char* maped_mem = (char*)mmap (NULL, file_size, PROT_WRITE, MAP_SHARED, file, SEEK_SET);

    printf ("old info in file is %s\n", maped_mem);
    strncpy (maped_mem, "LESHA", file_size);
    printf ("check your file, new info is %s\n", maped_mem);

    munmap (maped_mem, file_size);

    close (file);

    return 0;
}