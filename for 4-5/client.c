#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h>    
#include <string.h>    
#include <unistd.h> 
#include <sys/types.h>
#include <fcntl.h>

#define BUFSIZE 2048

void error_handler(char *msg) {
    perror(msg);
     exit(1);
}

struct client
{
    int client_id;
    int* shopping_list;
};

typedef struct 
{
    int client_id;
    int shop;
} Message;

struct sockaddr_in server_addr;
int sock;

int shopping_lists[3][4] = {
    {1, 2, 3, 4},
    {1, 2, 1, 2},
    {3, 4, 3, 4}
};

void client_process(struct client cl)
{
    int server_len, send_len;
    char int_buffer[sizeof(int)];

    server_len = sizeof(server_addr);

    // Отправляем список покупок на сервера
    printf("Client now going to the store!\n");

    for (int i = 0; i < 4; i++) {
        Message message;
        message.client_id = cl.client_id;
        message.shop = cl.shopping_list[i];

        if (send(sock, &message, sizeof(message), 0) < 0) {
            error_handler("Bad request!");
        }

        printf("Sent shopping list to server.\n");
        sleep(3);
    }
    printf("%d bought all! Thanks!\n", cl.client_id);
}

int creating_clients(int n) {
    if (n == 0) {
        return 0;
    }
    if (fork() == 0) {
        struct client cl;
        cl.shopping_list = shopping_lists[n - 1];
        cl.client_id = n;
        client_process(cl);
        return 0;
    }
    return creating_clients(n - 1);
}

int main(int argc, char *argv[])
{                                    
    char *echoString;                
    unsigned int echoStringLen;      
    int bytesRcvd, totalBytesRcvd;

    if (argc != 3) {
        printf("Usage: %s <server_ip> <port>\n", argv[0]);
        exit(1);
    }   

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&server_addr, 0, sizeof(server_addr));  
    server_addr.sin_family      = AF_INET;    
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);  
    server_addr.sin_port        = htons(atoi(argv[2])); 



    connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));

    creating_clients(3);

    exit(0);
}