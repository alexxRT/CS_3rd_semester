#include "semaphores.h"


int main ()
{
    size_t key = ftok ("./semaphores_with_msg.c", 0);

    umask (0);
    int msqid = msgget (key, IPC_CREAT | 0777);

    Message_Prob (msqid);

    printf ("proga 1 executed\n");
    msgctl (msqid, IPC_RMID, NULL);

    return 0;
}