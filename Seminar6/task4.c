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
#include <pthread.h>



#define SIZE 100

//необходим работающий с пайпом процесс, чтобы он жил
//непонятно сколько инфы и откуда пришло
//чтобы прочитать нужно сначала записать

//shared memory
//symophores                SVIPC system V ipc
//msg

void* PlusCounter (void* arg)
{
    int* counter = (int*)arg;
    (*counter)++;

    return NULL;
}

int main () 
{

    size_t key = ftok("./task1.c", 0);

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

    printf ("%p\n", buffer);
    
    buffer[0]++;
    buffer[2]++;
    
    int counter = 0;
    int thread_id[5];

    for (int i = 0; i < 5; i ++)
    {
        pthread_create (&thread_id[i], (const pthread_attr_t*)NULL, PlusCounter, &counter);
    }

    for (int i = 0; i < 5; i ++)
    {
        printf ("%d\n", thread_id[i]);
    }

    for (int i = 0; i < 5; i ++)
    {
        pthread_join (thread_id[i], NULL);
    }

    buffer[3] += counter + 1;
    buffer[5] += counter + 1;

    
    printf ("[%d %d %d %d %d %d]\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);

    shmdt(buffer);

    return 0;
}