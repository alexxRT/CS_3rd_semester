#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

#define MSG_SIZE 100
#define MAX_SIZE 100

const char TEXT    = '1'; //messages types
const char SERVER  = '2';
const char REGISTR = '3';

const int  FLAG_INDX = MSG_SIZE - 1;

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

int send_all (int socket, char* buffer, int message_len, int flags)
{
    int sent_bytes   = 0;
    int sent_overall = 0;

    while (sent_overall < message_len)
    {
        int sent_bytes = send (socket, buffer + sent_overall, MSG_SIZE, 0);

        if (sent_bytes == -1)
        {
            printf ("send finished with error code %d\n", errno);
            printf ("bytes losed while sending: %d\n", message_len - sent_overall);
            break;
        }

        sent_overall += sent_bytes;
    }

    return 0;
}

void SendAllNewConnect (DATA_BASE* base)
{
    assert (base != NULL);

    int num_of_users = base->num_of_users;
    char message [MSG_SIZE] = "";

    for (int i = 0; i < num_of_users - 1; i ++)
    {
        strncpy (message, "NEW CONNECT TO THE SERVER", MSG_SIZE);
        message[FLAG_INDX] = TEXT;
        send_all (base->users[i].user_socket, message, MSG_SIZE, 0);
        bzero (message, MSG_SIZE);

        memcpy (message, (const void*)&(base->users[num_of_users - 1]), sizeof(user));
        message[FLAG_INDX] = SERVER;
        send_all (base->users[i].user_socket, message, MSG_SIZE, 0);
        bzero (message, MSG_SIZE);
    }

    return;
}

void SendAddrsToClient (DATA_BASE* base)
{
    assert (base != NULL);

    int num_of_users = base->num_of_users;

    char message[MSG_SIZE] = "";
    strncpy (message, "WELCOME ON BOARD!!! NOW ON THE SERVER:", MSG_SIZE);
    message[FLAG_INDX] = TEXT;
    send_all (base->users[num_of_users - 1].user_socket, message, MSG_SIZE, 0);
    bzero (message, MSG_SIZE); 

    printf ("i sent greeting\n");

    for (int i = 0; i < num_of_users - 1; i ++)
    {
        memcpy (message, (const void*)&(base->users[i]), sizeof(user));
        message[FLAG_INDX] = SERVER;
        send_all (base->users[num_of_users - 1].user_socket, message, MSG_SIZE, 0);
        bzero (message, MSG_SIZE);
    }

    return;
}

void PrintUsersAttr (DATA_BASE* base)
{
    int num_of_users = base->num_of_users;
    printf ("now on server  [%d] users\n", base->num_of_users);

    for (int i = 0; i < num_of_users; i ++)
    {
        printf ("user socket is [%d]\n", base->users[i].user_socket);
        printf ("user port   is [%d]\n", base->users[i].user.sin_port); // which one to use when chating?
        printf ("after ntohs    [%d]\n", ntohs(base->users[i].user.sin_port));
        printf ("user addres is [%s]\n", inet_ntoa(base->users[i].user.sin_addr));

        //printf ("users sin_family is [%d]\n", base->users[i].user.sin_family);
    }
    printf ("\n\n");

    return;
}

void GetPortNumber (char buffer[], DATA_BASE* base)
{
    sscanf (buffer, "%hu", &(base->users[base->num_of_users].user.sin_port));

    return;
}

int main ()
{
    int server_socket = socket (AF_INET, SOCK_STREAM, 0);
    printf ("socket address is %d\n", server_socket);
    
    struct sockaddr_in socket_atr;

    socket_atr.sin_family = AF_INET;
    socket_atr.sin_port   = htons (9005);
    socket_atr.sin_addr.s_addr = INADDR_ANY;
    memset(socket_atr.sin_zero, 0, 8);

    int error = bind (server_socket, (const struct sockaddr*)&socket_atr, sizeof(struct sockaddr));
    
    if (error != 0)
    {
        printf ("bind unsuccessfully\n");
        printf ("error code was %d\n", errno);
    }
    else 
    {
        printf ("I BIND it\n");

        listen (server_socket, 1);

        user* users = (user*)calloc(MAX_SIZE, sizeof(user)); //init array to store users

        DATA_BASE base = {}; // made struct to store array and number of users
        base.users = users;
        base.num_of_users = 0;

        while (1)
        {
            //New connection
            struct sockaddr* user_info = (struct sockaddr*)&base.users[base.num_of_users].user;
            socklen_t user_size = sizeof (struct sockaddr);
            base.users[base.num_of_users].user_socket = accept (server_socket, user_info, &user_size);
            base.users[base.num_of_users].personal_id = base.num_of_users;

            char message[MSG_SIZE] = "";

            message [FLAG_INDX] = REGISTR;
            memcpy (message, &base.num_of_users, sizeof(int));
            send_all (base.users[base.num_of_users].user_socket, message, MSG_SIZE, 0);
            bzero (message, MSG_SIZE);

            //recieving binded users port
            recv (base.users[base.num_of_users].user_socket, message, MSG_SIZE, MSG_WAITALL);
            GetPortNumber (message, &base);
            base.users[base.num_of_users].user.sin_port = htons(base.users[base.num_of_users].user.sin_port);
            base.num_of_users++;

            printf ("connection OK\n");

            PrintUsersAttr    (&base);  //debug print for server side
            SendAllNewConnect (&base);  // all clients recieve address of new user
            SendAddrsToClient (&base);  // new client recieve list with online users
        }
    }
}