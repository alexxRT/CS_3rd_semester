#include <unistd.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
        size_t p = fork ();
        
        if (p == 0)
        {
            if (argc > 1) execvp (argv[1] , argv + 1); //childe one changes context
        }
        if (p > 0)
        {
            for (int i = 0; i < 10000; i ++) //parent proccess counts to 10000
            {
                printf ("%d\n", i);
            }
        }
        return 0;
}