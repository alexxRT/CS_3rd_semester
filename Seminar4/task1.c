#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main ()
{
    umask (0);
    const char* path = "./text.txt";
    int pid = fork ();

    if (pid < 0) printf ("ERROR\n");
    
    else if (pid > 0)
    {
        mkfifo (path, 0777);

        int f_w = open (path, O_WRONLY);
        write (f_w, "Hello world", 14);

        close (f_w);
    }
    else
    {
        mkfifo (path, 0777);

        int f_r = open (path, O_RDONLY);
        char* buffer = (char*)calloc(14, sizeof(char));
        int num_of_bytes = read (f_r, buffer, 14);

        int message = 0;

        while (!message)
        {
            if (num_of_bytes > 0)
            {
            printf ("%s\n", buffer);
            message ++;
            }

        }
        
        close (f_r);
        free (buffer);
    }
    

    return 0;
}
