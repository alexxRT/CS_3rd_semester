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
#include <assert.h>

#define SIZE 1000

enum GET_MESSAGE
{
    END = -1,
    READ = 1,
    CONTINUE = 0
};

int get_message (char message[])
{
    char c = 0;
    int possition = 0;
    while ((c = getchar()) != '\n')
    {
        message[possition] = c;
        possition ++;
    }
    message[possition] = '\0';

    if (!strcmp (message, "Q"))
        return END;
    if (!strcmp (message, "SEE"))
        return READ;
    else 
        return CONTINUE;
}

void recieve_message (char* buffer, int* ptr_to_read)
{
    int i = *ptr_to_read;

    while (buffer[i] != '\0')
    {
        printf ("From client 2: ");
        while (buffer[i] != '*')
        {
            printf ("%c", buffer[i]);
            i ++;
        }

        i ++;
        printf ("\n");
    }

    *ptr_to_read = i;

    return;
}

void  send_message (char* buffer, char* message, int* ptr_to_write)
{

    memcpy (buffer + *ptr_to_write, message, strlen(message));
    memcpy (buffer + *ptr_to_write + strlen(message), "*", 1);

    *ptr_to_write += strlen (message) + 1;

    return;
}


int main ()
{
    size_t key1 = ftok ("./task1.c", 0);
    size_t key2 = ftok ("./task2.c", 0);

    umask (0);

    int shm_key1 = shmget (key1, SIZE, 0777 | IPC_CREAT);
    int shm_key2 = shmget (key2, SIZE, 0777 | IPC_CREAT);

    char* buffer1 = NULL;
    char* buffer2 = NULL;

    int ptr_to_read  = 0;
    int ptr_to_write = 0;

    buffer1 = (char*)shmat (shm_key1, NULL, 0); 
    buffer2 = (char*)shmat (shm_key2, NULL, 0);

    recieve_message (buffer2, &ptr_to_read);

    while (1)
    {
    char message[SIZE] = "";
    int mess_indx = get_message (message);

    if (mess_indx == END) 
        break;

    if (mess_indx != READ)
        send_message (buffer1, message, &ptr_to_write);

    recieve_message (buffer2, &ptr_to_read);
    }

    bzero (buffer1, SIZE);
    bzero (buffer2, SIZE);

    shmdt  (buffer2);
    shmdt  (buffer1);
}