#include "Semaphores.h"
#include <stdio.h>
#include <sys/shm.h>

int main ()
{
    int* S = (int*)SharedMemoryInit(sizeof(int));

    P(S);
    printf ("Hello, procces 1 finaly executed\n");

    printf ("%d\n", *S);
    shmdt(S);
}