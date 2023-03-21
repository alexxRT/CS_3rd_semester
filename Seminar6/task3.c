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
    pthread_t thread1 = 0;
    pthread_t thread2 = 0;
    pthread_t thread3 = 0;
    pthread_t thread4 = 0;

    int counter = 0;

    printf ("Hello i am thred 0, my id is %lu\n", pthread_self());

    pthread_create (&thread1, (const pthread_attr_t*)NULL, PlusCounter, &counter);
    pthread_create (&thread2, (const pthread_attr_t*)NULL, PlusCounter, &counter);
    pthread_create (&thread3, (const pthread_attr_t*)NULL, PlusCounter, &counter);
    pthread_create (&thread4, (const pthread_attr_t*)NULL, PlusCounter, &counter);

    pthread_join (thread4, NULL);
    pthread_join (thread3, NULL);
    pthread_join (thread2, NULL);
    pthread_join (thread1, NULL);
    
    counter++;

    printf ("Counter after all threads execution [%d]\n", counter);
    

    return 0;
}