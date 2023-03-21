#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct stat my_stat;

int main ()
{
    my_stat buf = {};
    int error = stat ("../Seminar8/proga1.c", &buf);

    if (error)
        printf ("attributes were not gotten\n");
    else
    { 
        printf  ("Your file attributes are\n");
        printf  ("ID of device containing file   %llu\n", buf.st_dev                   );
        printf  ("Mode of file                   %llu\n", buf.st_mode                  );
        printf  ("Number of hard links           %llu\n", buf.st_nlink                 );
        printf  ("File serial number             %llu\n", buf.st_ino                   );
        printf  ("User ID of the file            %llu\n", buf.st_uid                   );
        printf  ("Group ID of the file           %llu\n", buf.st_gid                   );
        printf  ("Device ID                      %llu\n", buf.st_rdev                  );
        printf  ("time of last access            %llu\n", buf.st_atimespec.tv_nsec     );
        printf  ("time of last data modification %llu\n", buf.st_mtimespec.tv_nsec     );
        printf  ("time of last status change     %llu\n", buf.st_ctimespec.tv_nsec     );
        printf  ("time of file creation(birth)   %llu\n", buf.st_birthtimespec.tv_nsec );
        printf  ("file size, in bytes            %llu\n", buf.st_size                  );
        printf  ("blocks allocated for file      %llu\n", buf.st_blocks                );
        printf  ("optimal blocksize for I/O      %llu\n", buf.st_blksize               );
        printf  ("user defined flags for file    %llu\n", buf.st_flags                 );
        printf  ("file generation number         %llu\n", buf.st_gen                   );
    }
    return 0;
}