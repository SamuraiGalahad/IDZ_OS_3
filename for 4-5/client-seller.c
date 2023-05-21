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


int main(int argc, char *argv[])
{
    int servSock;                   
    int clntSock;                   
    struct sockaddr_in serv, clnt; 
    unsigned int clntLen;

    servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&serv, 0, sizeof(serv));  
    serv.sin_family = AF_INET;               
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(atoi(argv[1]));    

    bind(servSock, (struct sockaddr *) &serv, sizeof(serv));
    listen(servSock, MAXPENDING);

    for (;;)
    {
        clntLen = sizeof(clnt);
        clntSock = accept(servSock, (struct sockaddr *) &clnt, &clntLen);
        Message message;  
        int recvMsgSize = recv(clntSock, &message, sizeof(message), 0);

        printf("Good shop: %d\n", message.shop);
        sleep(1);

        close(clntSock);
    }
}
