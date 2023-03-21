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

//необходим работающий с пайпом процесс, чтобы он жил
//непонятно сколько инфы и откуда пришло
//чтобы прочитать нужно сначала записать

//shared memory
//symophores                SVIPC system V ipc
//msg


int main () 
{

    size_t key = ftok("./task1.c", 8);

    umask (0);
    int shm_key = shmget (key, SIZE, 0777 | IPC_CREAT | IPC_EXCL);

    int* buffer = NULL;

    if (shm_key == -1)
    {
        shm_key = shmget(key, SIZE, 0777 | IPC_CREAT); 
        buffer = (int*)shmat (shm_key, NULL, 0); 
    }

    else 
    {
        buffer = (int*)shmat (shm_key, NULL, 0); 
        bzero ((char*)buffer, SIZE);
    }
    
    buffer[0]++;
    buffer[2]++;
    
    printf ("[%d %d %d]\n", buffer[0], buffer[1], buffer[2]);

    shmdt(buffer);

    return 0;
}