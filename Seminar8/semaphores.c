#include "semaphores.h"

static const double SEM_FLAG = 1;

void Message_Verh (int msqid)
{
    my_message message = {};
    message.type = SEM_FLAG;

    msgsnd (msqid, (const void*)&message, 0, 0);

    return;
}

void Message_Prob (int msqid)
{
    my_message message = {};

    msgrcv (msqid, (void*)&message, 0, SEM_FLAG, 0);

    return;
}
