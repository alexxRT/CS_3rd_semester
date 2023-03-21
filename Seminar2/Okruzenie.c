#include <stdio.h>
#include <strings.h>


int main (int argc, char* argv[], char* env[])
{
    int i = 0;

    printf ("\n\n---------------------\n\n");

    while (env[i] != NULL)
    {
        printf ("%s\n", env[i]);
        i++;
    }

    printf ("\n\n------------------------\n\n");

    return 0;
}