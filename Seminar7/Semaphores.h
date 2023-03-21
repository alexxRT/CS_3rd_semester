#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/sem.h>

typedef struct sembuf my_sebuf;

int GetSemId (int size);

void Verh (my_sebuf* array, size_t semid);

void Prob (my_sebuf* array, size_t semid);

void ArrayDestroy (my_sebuf* array, int num_of_elems);

char* SharedMemoryInit (int size);

void P (int* S);

void V (int * S);

#endif