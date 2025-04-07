#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 2

int main()
{
    int server, client[MAX_CLIENTS];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};
    int client_count = 0;

    server = socket(AF_INET, SOCK_STREAM, 0);

    if (server < 0)
    {
        printf("Socket creation failed\n");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Socket binding failed\n");
        return 1;
    }

    if (listen(server, MAX_CLIENTS) == 0)
    {
        printf("Server is listenig to  port\n");
    }
    else
    {
        printf("Server is not listenig to  port\n");
        return 1;
    }
    while (client_count < MAX_CLIENTS)
    {
        client[client_count] = accept(server, (struct sockaddr *)&client_addr, &addr_size);
        if (client[client_count] < 0)
        {
            printf("Client connection failed\n");
            return 1;
        }
        client_count++;
    }
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int recv_size = recv(client[0], buffer, BUFFER_SIZE, 0);
        if (recv_size <= 0)
        {
            printf("Client disconnected\n");
            break;
        }
        printf("Client 1 message:%s\n", buffer);

        send(client[1], buffer, BUFFER_SIZE, 0);
        if (strcmp(buffer, "fuckoff") == 0)
        {
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        recv_size = recv(client[1], buffer, BUFFER_SIZE, 0);
        if (recv_size <= 0)
        {
            printf("Client disconnected\n");
            break;
        }
        printf("Client 2 message:%s\n", buffer);

        send(client[0], buffer, BUFFER_SIZE, 0);
        if (strcmp(buffer, "fuckoff") == 0)
        {
            break;
        }
    }
    printf("Closing connections...\n");
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        close(client[i]);
    }
    close(server);
    return 0;
}