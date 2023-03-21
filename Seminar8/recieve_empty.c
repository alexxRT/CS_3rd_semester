// ftok first
// msgget
#include <sys/msg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_SIZE 100

const long EMPTY = 1;
const long END   = 2;


typedef struct message 
{
    long type;
    char mess[MAX_SIZE];
} my_message;

int main ()

{
    size_t key = ftok ("./send_empty.c", 0);
    int message_counter = 0;

    umask (0);
    int msqid = msgget (key, IPC_CREAT | 0777);

    my_message message = {};
    int error = msgrcv (msqid, (void*)&message, sizeof(my_message) - sizeof (long), 0, 0);

    do 
    {
        msgrcv (msqid, (void*)&message, sizeof(my_message) - sizeof (long), 0, 0);
        message_counter ++;
    }
    while (message.type != END);

    printf ("MESSAGES RECIEVED %d/5\n", message_counter);

    msgctl (msqid, IPC_RMID, NULL);
   
    return 0;
}