
#include <sys/msg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_SIZE 100

const long HELLO = 1;
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

    message.type = HELLO;
    strcpy (message.mess, "Hello world");

    msgsnd (msqid, (const void*)&message, sizeof(my_message) - sizeof (long), 0);
      

    message.type = END;
    msgsnd (msqid, (const void*)&message, sizeof(my_message) - sizeof (long), 0);

    return 0;
}