#include <pthread.h>
#include <stdio.h>


void* PlusCounter (void* arg)
{
    int* counter = (int*)arg;
    (*counter)++;

    return NULL;
}


int main ()
{
    pthread_t new_thread = 0;
    int counter = 0;

    printf ("Hello i am thred 0, my id is %lu\n", pthread_self());

    int error = pthread_create (&new_thread, (const pthread_attr_t*)NULL, PlusCounter, &counter);
    
    if (error != 0)
    { 
    printf ("Thread wasn't created\n");
    return 0;
    }
    
    pthread_join (new_thread, NULL);

    printf ("Second thread id is %lu\n", new_thread);
    counter++;

    printf ("Counter after two threads execution [%d]\n", counter);
    

    return 0;
}