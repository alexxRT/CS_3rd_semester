#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>

typedef struct _message
{
    char* message_itself;
    size_t size;
} new_message;

void print_message_safe (new_message* message)
{
    assert (message != NULL);
    assert (message->message_itself != NULL);

    size_t len = message->size;

    for (int i = 0; i < len; i++)
    {
        printf ("%c", message->message_itself[i]);
    }
    printf ("\n");
}

int main ()
{
    int pipes[2];
    pipe (pipes);

    int f_w = pipes [1];
    int f_r = pipes [0];

    int pid = fork ();

    if (pid < 0) printf ("Child proccess was'n created");

    if (pid > 0)
    {
        new_message message = {};
        message.message_itself = "Hello child, I am your parent";
        message.size = strlen (message.message_itself); 

        write (f_w, &message, sizeof(new_message));

        close (f_w);
        close (f_r);
    }

    if (pid == 0)
    {
        new_message recieved_mes = {};

        read  (f_r, &recieved_mes, sizeof(new_message));
        print_message_safe (&recieved_mes);

        close (f_w);
        close (f_r);
    }
    
    close (f_w);
    close (f_r);

    return 0;
}