#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h>

const int MAX_SIZE = 128;
const int NAME_SIZE = 16; //max lenght of user names

const int MSG_SIZE_LONG  = 128;
const int MSG_SIZE_SHORT = 16;


enum SERVER_FLAGS
{
    NEW_CONNECT    = 1,
    DISCONNECT     = 2,
    CASUAL_MESSAGE = 3,
    SERVER_COMMAND = 4,
    ONLINE_LIST    = 5,
};

enum ERROR_LIST
{
    SUCCESS = 0,
    NO_PLACE_TO_ADD = -1,
    NO_USER_EXISTS = -2,
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

typedef struct user
{
    int   user_msgqid;  //user's queues ID
    char  user_name [NAME_SIZE];    //his name
    int   engaged_stat; //shows the place can be rewrite or no

} user_info_t;

typedef struct _users
{
    user_info_t* user;
    int   size;

} users_base_t;

int FindToAddUserPlace (users_base_t* base)
{
    for (int i = 0; i < MAX_SIZE; i ++)
    {
        if (!base -> user[i].engaged_stat) //check if its vacant place
        {
            return i;
        }
    }

    return NO_PLACE_TO_ADD;
}
int FindUserPlace (users_base_t* base, int message_qid, char* name)
{
   int i = 0;
   int pointer = 0;
   
    if (message_qid > 0)
    {
        while (i < base->size)
        {
            if (base->user[pointer].engaged_stat)
            {
                if (base->user[pointer].user_msgqid == message_qid)
                    return pointer;

                i ++;
                pointer ++;
            }
            else
                pointer ++;
        }
    }
    else if (name != NULL)
    {
        while (i < base->size)
        {
            if (base->user[pointer].engaged_stat)
            {
                if (!strcmp(base->user[pointer].user_name, name))
                    return pointer;

                i ++;
                pointer ++;
            }
            else
                pointer ++;
        }
    }
    else
        printf ("Incorrect input data to find user\n");

    return NO_USER_EXISTS; //later add specific error code
}
void AddUser (users_base_t* base, int add_place, status_update_t* message)
{
    base->user[add_place].user_msgqid = message->mesqid;
    base->user[add_place].engaged_stat = 1;
    strcpy (base->user[add_place].user_name, message->name);
    printf ("name of new user [%s] on the place [%d]\n", base->user[add_place].user_name, add_place);
    base->size ++;

    return;
}

void GreetNewUser (status_update_t* message)
{
    my_message_t message_to_send = {};

    message_to_send.type = SERVER_COMMAND;
    strcpy (message_to_send.sender_name, "SERVER");
    message_to_send.reciever_mesqid = message->mesqid;
    strcpy (message_to_send.message, "Welcome on board! You've been succsessfully connected!\n");

    msgsnd (message->mesqid, (const void*)&message_to_send, MSG_SIZE_LONG, 0);
}

void SendUserMessage (int message_qid, char* message)
{
    my_message_t message_to_send = {};

    message_to_send.type = SERVER_COMMAND;
    strcpy (message_to_send.sender_name, "SERVER");
    strcpy (message_to_send.message, message);

    msgsnd (message_qid, (const void*)&message_to_send, MSG_SIZE_LONG, 0);

}

int AddNewUsers (users_base_t* base, int message_qid)
{
    assert (base);
    status_update_t message = {}; // init message to recieve

    int recieve_stat = 1;
    int add_place = -1;

    while (recieve_stat > 0)
    {
        recieve_stat = msgrcv (message_qid, (void*)&message, MSG_SIZE_SHORT, NEW_CONNECT, IPC_NOWAIT);
        //printf ("recive_stat is %d\n", recieve_stat);

        if (recieve_stat > 0)
        {
            add_place = FindToAddUserPlace  (base); //linear search O(n), could use lists instead
            if (add_place < 0) //check wheather adding was successful or not
            {
                SendUserMessage (message.mesqid, "No place for you, body :(, try to reconnect later\n"); //let user know about connection failour
                printf ("No place for new user, max capacity of %d users was reached\n", MAX_SIZE);
                return NO_PLACE_TO_ADD;
            }
            printf ("add place was found [%d]\n", add_place);

            AddUser (base, add_place, &message);
            GreetNewUser (&message); // say hello and show him that he connected.

            printf ("user added\n");
        }
    }

    return SUCCESS;
}
int SendMessages (users_base_t* base, int message_qid)
{
    assert (base);

    my_message_t message = {};
    int recieve_stat = 1;

    while (recieve_stat > 0)
    {
        recieve_stat = msgrcv (message_qid, (void*)&message, MSG_SIZE_LONG, CASUAL_MESSAGE, IPC_NOWAIT);

        if (recieve_stat > 0)
        {
            int user_place = FindUserPlace (base, 0, message.reciever_name);
            printf ("reciever position is [%d]\n", user_place);

            if (user_place < 0)
            {
                printf ("User with name: %s, does not exist\n", message.reciever_name);
                int sender_place = FindUserPlace (base, message.sender_mesqid, NULL);
                SendUserMessage (base->user[sender_place].user_msgqid, "Unable to reach client, he/she is offline or doesnt exist");
            }

            msgsnd (base->user[user_place].user_msgqid, (const void*)&message, MSG_SIZE_LONG, 0);
        }
    }

    return SUCCESS;
}

int DisconnectUsers (users_base_t* base, int message_qid)
{
    assert (base);
    status_update_t message = {};

    int recieve_stat = 1;

    while (recieve_stat > 0)
    {
        recieve_stat = msgrcv (message_qid, (void*)&message, MSG_SIZE_SHORT, DISCONNECT, IPC_NOWAIT);
        if (recieve_stat > 0)
        {
            int place = FindUserPlace (base, message.mesqid, NULL);

            if (place < 0)
            {
                printf ("User, which is being disconected, does not exist\n");
                return NO_USER_EXISTS;
            }

            printf ("place of disconnecting user [%d]\n", place);

            base->user[place].engaged_stat = 0;
            base->size --;
            bzero (base->user[place].user_name, NAME_SIZE);

        }
    }

    return SUCCESS;

}

int ServerDead (users_base_t* base, int server_id)
{
    int i = 0;
    int pointer = 0;
    while (i < base->size)
    {
        if (base->user[pointer].engaged_stat)
        {
            base->user[pointer].engaged_stat = 0;
            bzero (base->user[pointer].user_name, NAME_SIZE);
            SendUserMessage (base->user[pointer].user_msgqid, "You have been disconnected from the server, try to reconnect later\n");
            i ++;
            pointer ++;
        }

        else 
            pointer ++;
    }

    msgctl (server_id, IPC_RMID, NULL);

    return SUCCESS;
}

void SendOnlineList (users_base_t* base, int message_qid)
{
    assert (base);

    my_message_t message = {};
    int recieve_stat = 1;

    while (recieve_stat > 0)
    {
        recieve_stat = msgrcv (message_qid, (void*)&message, MSG_SIZE_LONG, SERVER_COMMAND, IPC_NOWAIT);

        if (recieve_stat > 0)
        {
            if (!strcmp(message.message, "ONLINE"))
            {
                bzero (message.message, strlen("ONLINE"));
                int i = 0;
                int position = 0;
                int write_ptr = 0;
                fprintf (stderr, "%d\n", base->size);
                while (i < base->size)
                {
                    if (base->user[position].engaged_stat)
                    {

                        assert (position < MAX_SIZE);

                        sprintf (message.message + write_ptr, "%s\n", base->user[position].user_name);
                        write_ptr += strlen(base->user[position].user_name) + 1;
                        i ++;
                        position ++;
                    }
                    else 
                        position ++;
                }

                message.type = ONLINE_LIST;
                msgsnd(message.sender_mesqid, (const void*)&message, MSG_SIZE_LONG, 0);
            }
        }
    }
}

int main ()
{
    user_info_t users [MAX_SIZE] = {}; //users array

    users_base_t data_base = {};
    data_base.user = users; //replace user with users

    size_t seed = ftok ("./proga1.c", 0); //connect to the message queue
    umask (0);
    int server_qid= msgget (seed, IPC_CREAT | 0777);

    while (1)
    {
        int error = AddNewUsers (&data_base, server_qid); //add new users if appeared

        if (error != SUCCESS)
        {
            printf ("Error occured with error code %d\n", error);
            break;
        }

        SendOnlineList (&data_base, server_qid);   //send online list if somebody asked

        SendMessages (&data_base, server_qid);     //send all messages to clients

        DisconnectUsers (&data_base, server_qid);  //disconnect if somebody exited
    }

    
    ServerDead (&data_base, server_qid);
}