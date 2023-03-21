
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

    struct sockaddr_in to_socket_attr = {};

    to_socket_attr.sin_family      = AF_LOCAL;
    to_socket_attr.sin_addr.s_addr = INADDR_LOOPBACK; //inet_addr ("10.55.139.137");
    to_socket_attr.sin_port        = htons (9002);
    memset (to_socket_attr.sin_zero, 0, 8);

    char message [MSG_SIZE] = "";
    strncpy(message, "Hello", 6); 

    sendto (my_socket, message, MSG_SIZE, 0, (struct sockaddr*)&to_socket_attr, sizeof(struct sockaddr));
    printf ("message sent!\n");

    return 0;
}