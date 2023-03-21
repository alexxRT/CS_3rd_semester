
#include <sys/msg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

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
    printf ("key is %lu\n", key);

    umask (0);
    int msqid = msgget (key, 0777 | IPC_CREAT);    
  
    my_message message = {};

    for (int i = 0; i < 5; i ++)
    {
        message.type = EMPTY;
        int message_err = msgsnd (msqid, (const void*)&message, sizeof(my_message) - sizeof (long), 0);
        printf ("errno is %d\n", errno);

        printf ("%d\n", message_err);
        
    }

    message.type = END;
    msgsnd (msqid, (const void*)&message, sizeof(my_message) - sizeof (long), 0);

    return 0;
}