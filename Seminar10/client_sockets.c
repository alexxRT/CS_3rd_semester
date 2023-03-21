#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>


#define MSG_SIZE 100

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

int main ()
{
    int client_socket = socket (AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in socket_atr;

    socket_atr.sin_family = AF_INET;
    socket_atr.sin_port   = htons (9001); 
    socket_atr.sin_addr.s_addr =  inet_addr ("192.168.0.133");
    memset (socket_atr.sin_zero, 0, 8);

    int error = connect (client_socket, (struct sockaddr*)&socket_atr, sizeof(struct sockaddr));

    if (error)
    {
        printf ("unable to connect, connection finished with error code %d\n", errno);
        return 0;
    }
    else 
        printf ("Connected to the server!!!\n");

    char message[MSG_SIZE] = "";

    strncpy (message, "HELLO!!! I connected, finally", 30);
    send_all (client_socket, message, MSG_SIZE, 0);

    bzero (message, MSG_SIZE);
    recv (client_socket, message, MSG_SIZE, MSG_WAITALL);
    printf ("response from server %s\n", message);

    sleep (3);
    close (client_socket);

    return 0;
}