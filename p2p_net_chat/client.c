#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>


#define MSG_SIZE 100
#define MAX_SIZE 100
#define CMD_SIZE 10


const char TEXT    = '1'; //messages types
const char SERVER  = '2';
const char REGISTR = '3';
const char EXIT    = '4';

static int PERSONAL_ID = -1;

const int  FLAG_INDX = MSG_SIZE - 1; //put flags in the end of the message

typedef struct sockaddr_in inet_socket; // just more convinient

typedef struct _user
{
    inet_socket user;
    int user_socket;
    int personal_id;
    int status; // offline/online stat indicator
}user;

typedef struct _DATA_BASE
{
    user* users; //array of users
    int num_of_users;
} DATA_BASE;

typedef struct args
{
    DATA_BASE* base;
    int my_socket;
    int server;
    int* my_personal_id;
}thread_args;

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

    assert (position < MSG_SIZE && "Your message is too long, only 100 charachters allowed");
    message[position] = '\0';
}


void* ListenServer (void* args)
{
    thread_args* to_recv = (thread_args*)args;
    char message[MSG_SIZE] = "";

    while (1)
    {
        int recv_bytes = recv(to_recv->server, message, MSG_SIZE, MSG_WAITALL);

        if (recv_bytes < 0)
        {
            printf ("error was %d\n", errno);
            break;
        }

       if (message[FLAG_INDX] == TEXT)
       {
            printf ("%s\n", message);
            bzero (message, MSG_SIZE);
       }

       else if (message[FLAG_INDX] == SERVER)
       {
            user recieved_struct = ((user*)message)[0]; //add to users base, contact list
            to_recv->base->users[to_recv->base->num_of_users] = recieved_struct;
            fprintf (stderr, "User with ID: [%d] added to your contact list\n", recieved_struct.personal_id);
            to_recv->base->num_of_users ++;

            bzero (message, MSG_SIZE);
       }
       else if (message[FLAG_INDX] == REGISTR)
       {
            PERSONAL_ID = ((int*)message)[0];
            //fprintf (stderr, "my id is %d\n", my_personal_id);
       }
    }

    pthread_exit (NULL);
    return NULL;
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

void* SendMessages (void* args)
{
    thread_args* to_send = (thread_args*)args;

    char message[MSG_SIZE] = "";
    char command[CMD_SIZE] = "";

    while (1)
    {
        GetMessage (message);

        sscanf (message, "%s", command);

        // if (!strncmp(command, "exit"))
        // {
        //     char message[MSG_SIZE] = "";
        //     message[FLAG_INDX] = EXIT;

        //     send (to_send->server, message, MSG_SIZE, 0);
        //     break;
        // }

        int user_indx = atoi (command);

        int start = 0;
        int end   = 0;
        sprintf (message, "%n%d%n", &start, PERSONAL_ID, &end);

        int num_of_spaces = end - start;

        for (int i = 0 + num_of_spaces; i < 3 - num_of_spaces; i ++)
            message[i] = ' ';

        inet_socket reciever_attr = {};

        for (int i = 0; i < to_send->base->num_of_users; i++)
        {
            if (user_indx == to_send->base->users[i].personal_id)
            {
                reciever_attr = to_send->base->users[i].user;
                break;
            }
        }

        int error = sendto (to_send->my_socket, message, MSG_SIZE, 0, (const struct sockaddr*)&reciever_attr, sizeof(struct sockaddr)); 
        if (error < 0)
        {
            printf ("error itself %d\n", error);
            perror ("message wasnt sent");
        }
        bzero (message, MSG_SIZE);
    }

    pthread_exit (NULL);
    return NULL;
}

void* RecieveMessages (void* args)
{
    int* my_socket = (int*)args;

    while (1)
    {
        char message[MSG_SIZE] = "";
        inet_socket recieved_socket;
        socklen_t recieved_size;
    
        recvfrom (*my_socket, message, MSG_SIZE, MSG_WAITALL,(struct sockaddr*)&recieved_socket, &recieved_size);

        // if (message[FLAG_INDX] == EXIT)
        //     break;

        printf ("%s\n", message);
    }
}

int main ()
{
    int my_socket = socket (AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in my_socket_atr = {};
    my_socket_atr.sin_family = AF_INET;
    my_socket_atr.sin_port = htons (10005);
    my_socket_atr.sin_addr.s_addr = INADDR_ANY;
    memset (my_socket_atr.sin_zero, 0, 8);

    int bind_err = bind (my_socket, (const struct sockaddr*)&my_socket_atr, sizeof(struct sockaddr));
    if (bind_err)
    {
        printf ("bind unsuccessfuly %d\n", errno);
        return 0;
    }

    struct sockaddr_in server_socket_atr = {};
    server_socket_atr.sin_family = AF_INET;
    server_socket_atr.sin_port   = htons (9005); 
    server_socket_atr.sin_addr.s_addr =  INADDR_ANY;
    memset (server_socket_atr.sin_zero, 0, 8);

    int server = socket (AF_INET, SOCK_STREAM, 0);

    int error = connect (server, (struct sockaddr*)&server_socket_atr, sizeof(struct sockaddr));

    if (error)
    {
        perror ("what");
        printf ("unable to connect, connection finished with error code %d\n", errno);
        return 0;
    }
    else 
        printf ("Connected to the server!!!\n");

    char message[MSG_SIZE] = "";
    strncpy (message, "10005!", strlen ("10005!"));
    send (server, message, MSG_SIZE, 0);
    bzero (message, MSG_SIZE);


    pthread_t sending_id   = 0;
    pthread_t recieving_id = 0;
    pthread_t listen_id    = 0;

    thread_args arguments_to_thread = {};

    user* users = (user*)calloc(MAX_SIZE, sizeof(user)); //init array to store users
    DATA_BASE base = {}; // made struct to store array and number of users
    base.users = users;
    base.num_of_users = 0;

    arguments_to_thread.base      = &base;
    arguments_to_thread.server    = server;
    arguments_to_thread.my_socket = my_socket;

    pthread_create (&sending_id,   NULL, SendMessages,    &arguments_to_thread);
    pthread_create (&listen_id ,   NULL, RecieveMessages, &my_socket);
    pthread_create (&recieving_id, NULL, ListenServer,    &arguments_to_thread);

    pthread_join (sending_id  , NULL);
    pthread_join (recieving_id, NULL);
    pthread_join (listen_id,    NULL);

    //free routine

    return 0;
}
