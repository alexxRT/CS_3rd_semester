#include <pthread.h>
#include <stdio.h>



int main ()
{
    long long int id = pthread_self ();
    printf ("This thread id is [%lu]\n", id);

    return 0;
}