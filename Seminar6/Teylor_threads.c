#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/msg.h>
#include <pthread.h>
#include <math.h>

#define COMMAND_SIZE 10

typedef struct _set_of_args
{
    int number;
    int sign;
    double variable;
    double* buffer;
} ARGS_SET;


void* sin_calculate (void* arguments)
{
    ARGS_SET* member = (ARGS_SET*)arguments;
    double result = 1;

    for (int i = 1; i <= member->number; i ++)
        result = result * i;

    if (member->sign)
        result *= -1;

    int position = (member->number - 1) / 2;
    member->buffer[position] = pow(member->variable, member->number) / result;

    pthread_exit (NULL);

    return NULL;
}

void* cos_calculate (void* arguments)
{
    ARGS_SET* member = (ARGS_SET*)arguments;
    double result = 1;

    for (int i = 1; i <= member->number; i ++)
        result = result * i;

    if (member->sign)
        result *= -1;

    int position = (member->number) / 2;
    member->buffer[position] = pow(member->variable, member->number) / result;

    pthread_exit (NULL);

    return NULL;
}


int main (int argc, char* argv[])
{
    if (argc == 1)
    {
        printf ("Lack of arguments\n");
        return 0;
    }

    char command[COMMAND_SIZE] = "";

    printf ("Enter yor function name:\n");
    scanf ("%s", command);

    printf ("Enter the angle in deegres please:\n");
    
    double alpha = 0;
    scanf ("%lf", &alpha);
    
    double radians = alpha * (M_PI / 180);

    int num_of_members = atoi (argv[1]);

    double* members      = (double   *)calloc (num_of_members, sizeof(long long));

    if (!members)
    {
        printf ("TOO high precision, try put it less next time\n");
        return 0;
    }

    pthread_t* thread_id = (pthread_t*)calloc (num_of_members, sizeof(pthread_t));

    if (!thread_id)
    {
        free (members);

        printf ("TOO high precision, try put it less next time\n");
        return 0;
    }

    ARGS_SET*  args      = (ARGS_SET *)calloc (num_of_members, sizeof(ARGS_SET)); 

    if (!args)
    {
        free (members);
        free (thread_id);

        printf ("TOO high precision, try put it less next time\n");
        return 0;
    }

    if (!strcmp (command, "sin"))
    {
        for (int i = 0; i < num_of_members; i ++)
        {
            args[i].buffer   = members;
            args[i].number   = 2*i + 1;
            args[i].variable = radians;
            args[i].sign     = i % 2;

            pthread_create (thread_id + i, (const pthread_attr_t*)NULL, sin_calculate, args + i);
        }
    }
    else if (!strcmp (command, "cos"))
    {
        for (int i = 0; i < num_of_members; i ++)
        {
            args[i].buffer = members;
            args[i].number = 2*i;
            args[i].variable = radians;
            args[i].sign = i % 2;

            pthread_create (thread_id + i, (const pthread_attr_t*)NULL, cos_calculate, args + i);
        }
    }
    else 
    {
        printf ("Unknown function\n");
        return 0;
    }

    for (int i = 0; i < num_of_members; i ++) //join all proceses
        pthread_join (thread_id[i], NULL);


    double F_x = 0;

    for (int i = 0; i < num_of_members; i ++) // prints the result
        F_x += members[i];

    
    printf ("your %s(%.2lf) value is %lf\n", command, alpha, F_x);

    free (members);
    free (thread_id);
    free (args);

    return 0;
}