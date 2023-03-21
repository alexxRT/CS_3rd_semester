#include <unistd.h>
#include <stdio.h>


int main ()
{
    size_t pid = getpid ();
    size_t ppid = getppid ();


    printf ("proccess id is [%lu], parent id is [%lu]\n", pid, ppid);

    return 0;

}
