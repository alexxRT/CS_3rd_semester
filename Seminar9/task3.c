#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char* argv[])
{
    for (int i = 0; i < argc; i ++)
    {
        printf ("%s\n", argv[i]);
    }
    
    if (argc < 2)
    {
        printf ("lack of arguments for %s\n", argv[0]);
        return 0;
    }

    if (!strcmp ("-s", argv[1]))
    {
        if (argc < 4)
            printf ("lack of args for %s %s\n", argv[0], argv[1]);
        else
            symlink (argv[2], argv[3]);
    }
    else 
    {
        if (argc < 3)
            printf ("lack of args for %s\n", argv[0]);
        else
            link  (argv[1], argv[2]);
    }

    return 0;
}