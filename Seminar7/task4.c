#include "Semaphores.h"

#define SIZE 100


int main ()
{
    my_sebuf* array = (my_sebuf*)calloc (SIZE, sizeof (my_sebuf));
    array[0].sem_num = 1;

    size_t semid = GetSemId (SIZE);

    Prob (array, semid);
    printf ("I am proga 2\n");

    ArrayDestroy (array, SIZE);

    return 0;
}
