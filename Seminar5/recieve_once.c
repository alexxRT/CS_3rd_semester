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

    size_t key = ftok ("./task1.c", 0);

    umask (0);
    
    int shm_key = shmget (key, SIZE, 0);

    char* buffer = NULL;

    buffer = (char*)shmat (shm_key, NULL, 0); 

    printf ("%s\n", buffer);
    
    shmctl (shm_key, IPC_RMID, NULL);
    shmdt(buffer);

    return 0;
}