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
    const char* message_itself;
    char ID;
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

void init_message (new_message* message, const char* string, char ID)
{
    assert (message != NULL);
    assert (string  != NULL);

    message->message_itself = string;
    message->size = strlen (message->message_itself); 
    message->ID = ID;

    return;
}

int main ()
{
    int pipes[2];
    pipe (pipes);

    int f_w = pipes [1];
    int f_r = pipes [0];

    printf ("\n");

    int pid1 = fork ();
    if (pid1 < 0) printf ("First child proccess was'n created\n");


    int pid2 = fork ();
    if (pid2 < 0) printf ("Second child proccess was'n created\n");
    
    if (pid1!= 0 && pid2!= 0)
    {
    new_message message = {};

    init_message (&message, "Hello maaaan, Whats upp??", 'A');

    write (f_w, &message, sizeof (new_message));

    close (f_w);
    close (f_r);
    }

    else if (pid1 == 0 && pid2 != 0)
    {
    new_message message = {};

    init_message (&message, "Check my new profile", 'B');

    write (f_w, &message, sizeof (new_message));

    close (f_w);
    close (f_r);

    }

    else if (pid1 != 0 && pid2 == 0)
    {
    new_message message = {};

    init_message (&message, "Finaly, here is your last message", 'C');

    write (f_w, &message, sizeof (new_message));

    close (f_w);
    close (f_r);
    }

    else 
    {
        new_message message_recieved = {};
                
        int num_of_read_messages = 0;
        while (num_of_read_messages < 3)
        {
            int read_bytes = read (f_r, &message_recieved, sizeof (new_message));

            if (read_bytes > 0)
            {
                printf ("\n----------------------------------------------------\n");
                print_message_safe (&message_recieved);
                printf ("Message was recieved from procces [%c]\n", message_recieved.ID);
                printf ("----------------------------------------------------\n");

                num_of_read_messages ++;
            }
        }

        close (f_w);
        close (f_r);
    }
    
    close (f_w);
    close (f_r);

    return 0;
}