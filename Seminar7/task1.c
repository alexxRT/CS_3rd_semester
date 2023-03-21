#include "Semaphores.h"
#include <stdio.h>
#include <sys/shm.h>
#include <string.h>

int main ()
{
    int* S = (int*)SharedMemoryInit (sizeof(int));

    printf ("Hello, I am program 1\n");
    V (S);

    printf ("%d\n", *S);
    shmdt(S);
}
