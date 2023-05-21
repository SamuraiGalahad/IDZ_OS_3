#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <string.h>   
#include <unistd.h>    

#define MAXPENDING 5

typedef struct 
{
    int client_id;
    int shop;
} Message;

void send_to_seller(Message message, char *argv[])
{
    int sock;                        
    struct sockaddr_in loc_serv;
    unsigned short echoServPort1, echoServPort2;     
    char *servIP;   
    
    servIP = argv[2];        
    echoServPort1 = atoi(argv[3]);
    echoServPort2 = atoi(argv[4]);

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&loc_serv, 0, sizeof(loc_serv));

    loc_serv.sin_family = AF_INET;    
    loc_serv.sin_addr.s_addr = inet_addr(servIP);

    if (message.shop % 2 == 1)
    {
        printf("All information sent to 1st seller!");
        loc_serv.sin_port = htons(echoServPort1); 
    } else {
        printf("All information sent to 2st seller!");
        loc_serv.sin_port  = htons(echoServPort2); 
    }

    connect(sock, (struct sockaddr *) &loc_serv, sizeof(loc_serv));
 
    send(sock, &message, sizeof(message), 0);
    close(sock);
}

int main(int argc, char *argv[])
{
    int servSock;                   
    int clntSock;                   
    struct sockaddr_in serv, clnt;
    unsigned short echoServPort;   
    unsigned int clntLen;   

    echoServPort = atoi(argv[1]);

    servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&serv, 0, sizeof(serv));  
    serv.sin_family = AF_INET;               
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(echoServPort);    

    bind(servSock, (struct sockaddr *) &serv, sizeof(serv));
    listen(servSock, MAXPENDING);

    for (;;)
    {
        clntLen = sizeof(clnt);

        clntSock = accept(servSock, (struct sockaddr *) &clnt, &clntLen);

        printf("Handling client %s\n", inet_ntoa(clnt.sin_addr));

        Message message;
        int recvMsgSize = recv(clntSock, &message, sizeof(message), 0);
        while (recvMsgSize > 0)  
        { 
            printf("Got product with id=%d\n", message.shop);  
            send(clntSock, &message, recvMsgSize, 0);
            recvMsgSize = recv(clntSock, &message, sizeof(message), 0);
            send_to_seller(message, argv);
        }

    close(clntSock);
    }
}