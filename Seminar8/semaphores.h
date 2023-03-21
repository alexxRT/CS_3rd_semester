#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <sys/msg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

typedef struct message 
{
    long type;
} my_message;

void Message_Prob (int msqid);
void Message_Verh (int msqid);

#endif