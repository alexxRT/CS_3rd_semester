
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MSG_SIZE 100


int main ()
{
    int my_socket = socket(AF_LOCAL, SOCK_DGRAM, 0);

    struct sockaddr_in my_socket_attr = {};

    my_socket_attr.sin_family      = AF_LOCAL; //= AF_INET;
    my_socket_attr.sin_addr.s_addr = INADDR_LOOPBACK;
    my_socket_attr.sin_port        = htons (9005);
    memset (my_socket_attr.sin_zero, 0, 8);

    int error = bind (my_socket, (struct sockaddr*)&my_socket_attr, sizeof(struct sockaddr));
    if (error)
    {
        printf ("error was %d\n", errno);
        return 0;
    }

    char message[MSG_SIZE];

    struct sockaddr_in from_socket_attr = {};
    socklen_t from_len = 0;

    while (1)
    {
        int bytes_rcvd= recvfrom (my_socket, message, MSG_SIZE, 0, (struct sockaddr*)&from_socket_attr, &from_len);
      
        printf ("from len is %d\n", from_len);
        printf ("dgrm recieved is [%s]\n", message);
        printf ("ip addres who sent is [%u]\n", from_socket_attr.sin_addr.s_addr);

        bzero    (message, MSG_SIZE);
    }

    return 0;

}