#include <sys/msg.h>
#include <pthread.h>
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

const int EXIT = -1;

enum SERVER_FLAGS
{
    NEW_CONNECT    = 1,
    DISCONNECT     = 2,
    CASUAL_MESSAGE = 3,
    SERVER_COMMAND = 4,
    ONLINE_LIST    = 5,
    STOP_RECIEVING = 6,
    PRINT_USR_INFO = 7,
    INVALID_FORMAT = 8,
};

typedef struct _message
{
    long type;
    int  server_queue_id; //we always send to server
    int  sender_queue_id;
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

char* GetTime ()
{

time_t mytime = time(NULL);
char * time_str = ctime(&mytime);
time_str[strlen(time_str)-1] = '\0';
    
    return  time_str;
}

char* SkipWord (char* str)
{
    assert (str != NULL);
    int i = 0;

    while (str[i] != ' ' && str[i] != '\0')
        i ++;

    return (char*)str + i;
}

char* SkipSpaces (const char* str)
{
    assert (str != NULL);
    int i = 0;

    for (; str[i] != '\0'; i++)
    {
        if (str[i] != ' ')
        {
            assert (str [i] != ' ');
            return (char*)(str + i);
        }
    }
    
    return (char*)(str + i);
}

void Registration (int server_queue_id, int my_queue_id, char my_name[])
{
    printf ("Hello!\n");
    printf ("What's your name? Maximum 10 characters allowed\n");

    scanf ("%s", my_name);

    status_update_t message = {};
    strcpy (message.name, my_name);
    message.mesqid = my_queue_id;
    message.type = NEW_CONNECT;

    msgsnd (server_queue_id, (const void*)&message, MSG_SIZE_SHORT, 0);

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

        response_stat = msgrcv (my_queue_id, (void*)&response, MSG_SIZE_LONG, SERVER_COMMAND, IPC_NOWAIT);
    }
    printf ("\n");

    printf ("From %s: %s", response.sender_name, response.message);


}

void AskOnline (int server_queue_id, int my_queue_id)
{
    my_message_t message = {};

    message.sender_queue_id = my_queue_id;
    message.type = SERVER_COMMAND;
    strcpy (message.message, "ONLINE");

    msgsnd (my_queue_id, (const void*)&message, MSG_SIZE_LONG, 0);
}

void Disconnect (int server_queue_id, int my_queue_id)
{
    status_update_t message = {};
    message.type   = DISCONNECT;
    message.mesqid = my_queue_id;

    msgsnd (server_queue_id, (const void*)&message, MSG_SIZE_SHORT, 0);

    msgctl (my_queue_id, IPC_RMID, NULL);
}


void* RecieveMessages (void* args)
{
    int* my_queue_id = (int*)args;

    while (1)
    {
        my_message_t message = {};
        msgrcv (*my_queue_id, (void*)&message, MSG_SIZE_LONG, 0, 0);

        if (message.type == ONLINE_LIST)
        {
            printf ("\n------CLIENTS ONLINE AT %s-------\n\n", GetTime());
            printf ("%s", message.message);
            printf ("\n----------------ONLINE LIST ENDED-----------------\n\n");
        }
        else if (message.type == CASUAL_MESSAGE)
        {
            printf ("From %s: %s\n", message.sender_name, message.message);
        }
        else if (message.type == SERVER_COMMAND)
        {
            printf ("\nReply from the server!!!\n");
            printf ("From %s: %s\n", message.sender_name, message.message);
        }

        else if (message.type == STOP_RECIEVING)
        {
            fprintf (stderr, "Closing connection ...\n");
            sleep (1);
            break;
        }
        else if (message.type == PRINT_USR_INFO)
        {
            printf ("%s", message.message);
        }
        else 
        {
            printf ("UNKNOWN MESSAGE TYPE %ld\n", message.type);
            break;
        }
    }

    pthread_exit (NULL);
    return NULL;
}

void GetMessage (char message[])
{
    char c = 0;
    int position = 0;

    fflush (stdin);

    while ((c = getchar()) != '\n')
    {
        message[position] = c;
        position ++;
    }

    assert (position < MAX_SIZE && "Your message is too long, only 100 charachters allowed");
    message[position] = '\0';
}

int ParseMessage (my_message_t* message, char buffer[])
{
    char command [10] = {};
    sscanf (buffer, "%s", command);
   
    if (!strcmp(command, "to"))
    {
        buffer = SkipWord   (buffer);
        buffer = SkipSpaces (buffer);

        char rec_name [NAME_SIZE];
        sscanf (buffer, "%s", rec_name);
        strcpy (message->reciever_name, rec_name);

        buffer = SkipWord (buffer);

        strcpy (message->message, buffer); //copying message to send

        message->type = CASUAL_MESSAGE; 
        return CASUAL_MESSAGE;
    }

    else if (!strcmp(command, "online"))
    {
        message->type = SERVER_COMMAND;
        strcpy (message->message, "ONLINE");
        return SERVER_COMMAND;
    }

    else if (!strcmp(command, "exit"))
    {
        return EXIT;
    }
    else 
        return INVALID_FORMAT;
}
 
void Send (my_message_t* message, long msg_type)
{
    msgsnd (message->server_queue_id, (const void*)message, MSG_SIZE_LONG, 0);
}

void* SendMessages (void* args)
{
    my_message_t* message = (my_message_t*)args;
    char buffer [MAX_SIZE] = "";

    while (1)
    {
        GetMessage (buffer);

        int msg_type = ParseMessage (message, buffer); //msg_result indicates what type of message user sent
    
        if (msg_type == EXIT) //finish chating if user pressed exit
            break;

        if (msg_type == INVALID_FORMAT)
        {
            message->type = PRINT_USR_INFO;
            strcpy (message->message, "please check your message format, it's incorrect\nrepeat your input:\n");
            msgsnd (message->sender_queue_id, (const void*)message, MSG_SIZE_LONG, 0);

            continue;
        }

        Send (message, msg_type);
    }

    message->type = STOP_RECIEVING;
    msgsnd (message->sender_queue_id, (const void*)message, MSG_SIZE_LONG, 0);

    pthread_exit (NULL);
    return NULL;
}

int main ()
{
    size_t key_1 = ftok ("./proga1.c", 0); //connect to the server queue

    srand (time (NULL));
    int random_seed = rand();

    size_t key_2 = ftok ("./client.c", random_seed); //create my private queue

    umask (0);

    int server_id   = msgget (key_1, IPC_CREAT | 0777);
    int my_queue_id = msgget (key_2, IPC_CREAT | 0777);

    char my_name [NAME_SIZE] = "";
    Registration (server_id, my_queue_id, my_name);

    my_message_t message = {};              //init message to send
    strcpy (message.sender_name, my_name);
    message.sender_queue_id = my_queue_id;
    message.server_queue_id = server_id;

    pthread_t sending_id   = 0;
    pthread_t recieving_id = 0;

    pthread_create (&sending_id,   NULL, SendMessages,    &message);    //Create two threads: first for dealind with user's IO
    pthread_create (&recieving_id, NULL, RecieveMessages, &my_queue_id);//                    second for recieving anwers from server

    pthread_join (sending_id,   NULL);
    pthread_join (recieving_id, NULL);

    Disconnect (server_id, my_queue_id);

    return 0;
}