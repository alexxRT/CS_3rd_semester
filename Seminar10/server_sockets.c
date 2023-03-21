// сокет конечная точка сетевого взаимодействия 
//индетифицируется дескриптором

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


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
    int server_socket = socket (AF_INET, SOCK_STREAM, 0);
    printf ("socket address is %d\n", server_socket);
    
    struct sockaddr_in socket_atr;

    socket_atr.sin_family = AF_INET;
    socket_atr.sin_port   = htons (9001);
    socket_atr.sin_addr.s_addr = INADDR_ANY;
    memset(socket_atr.sin_zero, 0, 8);

    int error = bind (server_socket, (const struct sockaddr*)&socket_atr, sizeof(struct sockaddr));
    
    if (error = 1)
    {
        printf ("bind unsuccessfully\n");
        printf ("error code was %d\n", errno);
    }
    else 
    {
        printf ("I BIND it\n");

        listen (server_socket, 1);

        int client_socket = -1;
        struct sockaddr_in client_addr = {};
        int client_addr_len = sizeof (struct sockaddr_in);

        while ((client_socket = accept (server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&client_addr_len)) > 0)
        {
            printf ("connection OK\n");

            char message [MSG_SIZE] = ""; //init buffer to comunicate with client

            recv   (client_socket, message, MSG_SIZE, MSG_WAITALL);
            printf ("message from the client %s\n", message);

            bzero (message, MSG_SIZE);
            strncpy (message, "Hello from server!!!", 21);
            int error = send_all (client_socket, message, MSG_SIZE, 0);
        }
    }











    printf ("\n\n\n----------------LIST OF POSSIBLE BIND ERRORS---------------\n\n\n");
    printf ("error on the line %d, code is %d\n", __LINE__,  EAFNOSUPPORT);
    printf ("error on the line %d, code is %d\n", __LINE__,  EBADF       );
    printf ("error on the line %d, code is %d\n", __LINE__,  EDESTADDRREQ);
    printf ("error on the line %d, code is %d\n", __LINE__,  EFAULT      );
    printf ("error on the line %d, code is %d\n", __LINE__,  EINVAL      );
    printf ("error on the line %d, code is %d\n", __LINE__,  ENOTSOCK    );
    printf ("error on the line %d, code is %d\n", __LINE__,  EOPNOTSUPP  );
    printf ("error on the line %d, code is %d\n", __LINE__,  EACCES      );
    printf ("error on the line %d, code is %d\n", __LINE__,  EEXIST      );
    printf ("error on the line %d, code is %d\n", __LINE__,  EIO         );
    printf ("error on the line %d, code is %d\n", __LINE__,  EISDIR      );
    printf ("error on the line %d, code is %d\n", __LINE__,  ELOOP       );
    printf ("error on the line %d, code is %d\n", __LINE__,  ENAMETOOLONG);
    printf ("error on the line %d, code is %d\n", __LINE__,  ENOENT      );
    printf ("error on the line %d, code is %d\n", __LINE__,  ENOTDIR     );
    printf ("error on the line %d, code is %d\n", __LINE__,  EROFS);
    printf ("\n\n----------------------------END OF LIST-------------------------\n\n");


    return 0;
}