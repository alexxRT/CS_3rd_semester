#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>


int main ()
{
    size_t p = fork ();

    if (p < 0) printf ("Can not create child proccess\n");
    if (p > 0) printf ("I am parent, my pid is [%d], my child id is [%lu]\n", getpid(), p);
    if (p == 0) printf ("I am child proccess, my id is [%d]; my parent id is [%d]\n", getpid(), getppid());

    while (1) // for synchronization
    {}

    return 0;

}