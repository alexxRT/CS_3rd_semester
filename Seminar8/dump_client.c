#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h>
#include <time.h>

const int NAME_SIZE = 11;
const int MAX_SIZE  = 100;

const int MSG_SIZE_LONG  = 130;
const int MSG_SIZE_SHORT = 15;


enum SERVER_FLAGS
{
    NEW_CONNECT = 1,
    DISCONNECT = 2,
    CASUAL_MESSAGE = 3,
    SERVER_MESSAGE = 4,
};

typedef struct _message
{
    long type;
    int  reciever_mesqid;
    int  sender_mesqid;
    char reciever_name [NAME_SIZE];
    char sender_name   [NAME_SIZE];
    char message       [MAX_SIZE];
} my_message_t;


typedef struct _status_message
{
    long type;
    int mesqid;
    char name [NAME_SIZE];

} status_update_t;

void Registration (int server_id, int my_buffer_id)
{
    printf ("Hello!\n");
    printf ("What's your name? Maximum 10 characters allowed\n");
    char my_name [NAME_SIZE];

    scanf ("%s", my_name);

    status_update_t message = {};
    strcpy (message.name, my_name);
    message.mesqid = my_buffer_id;
    message.type = NEW_CONNECT;

    msgsnd (server_id, (const void*)&message, MSG_SIZE_SHORT, 0);

    int response_stat = -1;
    my_message_t response = {};

    printf ("Now you will be connected to the server\n");

    while (response_stat == -1)
    {
        printf ("\rconnecting    ");
        fflush(stdout);
        sleep (1);

        printf ("\rconnecting .  ");
        fflush(stdout);
        sleep (1);

        printf ("\rconnecting .. ");
        fflush(stdout);
        sleep (1);

        printf ("\rconnecting ...");
        fflush(stdout);
        sleep (1);

        response_stat = msgrcv (my_buffer_id, (void*)&response, MSG_SIZE_LONG, SERVER_MESSAGE, IPC_NOWAIT);
    }
    printf ("\n");

    printf ("From %s: %s", response.sender_name, response.message);
}

void AskOnline (int server_id, int my_buffer_id)
{
    my_message_t message = {};

    message.sender_mesqid = my_buffer_id;
    message.type = SERVER_MESSAGE;
    strcpy (message.message, "ONLINE");

    msgsnd (server_id, (const void*)&message, MSG_SIZE_LONG, 0);
}

void Disconnect (int server_id, int my_buffer_id)
{
    status_update_t message = {};
    message.type   = DISCONNECT;
    message.mesqid = my_buffer_id;

    msgsnd (server_id, (const void*)&message, MSG_SIZE_SHORT, 0);

    msgctl (my_buffer_id, IPC_RMID, NULL);
}

int main ()
{
    size_t key_1 = ftok ("./proga1.c", 0); //connect to the server queue

    srand (time (NULL));
    int random_seed = rand();

    size_t key_2 = ftok ("./client.c", random_seed); //create my private queue

    umask (0);

    int server_id = msgget (key_1, IPC_CREAT | 0777);
    int my_buffer_id = msgget (key_2, IPC_CREAT | 0777);

    // time_t mytime = time(NULL);
    // char * time_str = ctime(&mytime);
    // time_str[strlen(time_str)-1] = '\0';
    // printf("Current Time : %s\n", time_str);

    Registration (server_id, my_buffer_id);

    char c = 0;

    while (c != 'q'){c = getchar();};


    Disconnect (server_id, my_buffer_id);

    return 0;
}