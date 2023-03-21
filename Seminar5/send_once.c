#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>



#define SIZE 100


int main () 
{

    size_t key = ftok("./task1.c", 0);

    umask (0);
    int shm_key = shmget (key, SIZE, 0777 | IPC_CREAT | IPC_EXCL);

    char* buffer = NULL;

    if (shm_key == -1)
    {
        shm_key = shmget (key, SIZE, 0777 | IPC_CREAT);
        buffer = (char*)shmat (shm_key, NULL, 0);
    }
    else
    {
        buffer = (char*)shmat (shm_key, NULL, 0); 
        bzero (buffer, SIZE);
    }

    strncpy (buffer, "HELLO WORLD", 12);

    shmdt(buffer);

    return 0;
}