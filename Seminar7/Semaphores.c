#include "Semaphores.h"

int GetSemId (int size)  //Dejkstra semophores with semophores array
{
    size_t key = ftok ("./task3.c", 0);
    size_t sem_id = semget (key, size, 0777 | IPC_CREAT);

    return sem_id;
}

void Prob (my_sebuf* array, size_t semid) 
{
    array[0].sem_op = -1;
    array[0].sem_flg = 0;

    semop (semid, array, 1);

    return;
}

void Verh (my_sebuf* array, size_t semid)
{
    array[0].sem_op = 1;
    array[0].sem_flg = 0;

    semop (semid, array, 1);

    return;
}

void ArrayDestroy (my_sebuf* array, int num_of_elems)
{
    bzero (array, num_of_elems * sizeof(my_sebuf));
    free (array);
}

// ------------------------------------------------------------------------------//

char* SharedMemoryInit (int size)   // Dejkstra semophores, using shared memory
{
    size_t key = ftok("./task1.c", 0);

    umask (0);
    int shm_key = shmget (key, size, 0777 | IPC_CREAT | IPC_EXCL);

    char* buffer = NULL;

    if (shm_key == -1)
    {
        shm_key = shmget(key, size, 0777 | IPC_CREAT); 
        buffer = (char*)shmat (shm_key, NULL, 0); 
    }

    else 
    {
        buffer = (char*)shmat (shm_key, NULL, 0); 
        bzero (buffer, size);
    }

    assert (buffer != NULL);
    return buffer;
}

void P (int* S) 
{
    while (*S == 0){};
    (*S) --;

    return;
}

void V (int * S)
{
    (*S) ++;

    return;
}

//---------------------------------------------------------------------------------------//