#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

void get_message (char message[])
{
    char c = 0;
    int possition = 0;
    while ((c = getchar()) != '\n')
    {
        message[possition] = c;
        possition ++;
    }
    message[possition] = '\0';
}

int main ()
{

    char response [MAX_SIZE];
    char message [MAX_SIZE];

    umask(0);
    const char* path = "./chat.txt";
    mkfifo (path, 0777);

    char c = 0;

    while (c != 'q')
    {
    printf ("\033[31mWait for your friend's response\033[0m\n");
    printf ("\n");

    int f_r = open (path, O_RDONLY);
    read (f_r, response, MAX_SIZE);

    printf ("\n------------------------------------------------------\n");
    printf ("From client1: %s\n", response);
    printf ("------------------------------------------------------\n\n");

    close (f_r);

    int f_w = open (path, O_WRONLY);

    printf ("\033[32mType your message: \033[0m");

    get_message (message);
    write (f_w, message, MAX_SIZE);
    close(f_w);

    #ifndef NO_DELAY
    {
        printf ("\n\n");
        printf ("Do you want to continue?\n");
        printf ("Press any key to continue, q to exit\n");
        scanf("%c", &c);
        fflush (stdin);
    }
    #endif

    }
    return 0;
}