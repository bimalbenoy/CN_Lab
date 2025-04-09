// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int server, client;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE] = {0};
    socklen_t addr_size = sizeof(client_addr);
    int ack_chance = 40; // server ack sending chance

    srand(time(0)); // inorder to generate random numbers

    server = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server, 5);

    printf("Server: Waiting for connection...\n");
    client = accept(server, (struct sockaddr *)&client_addr, &addr_size);
    printf("Server: Connected.\n");

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int recv_size = recv(client, buffer, BUFFER_SIZE, 0);
        if (recv_size <= 0)
            break;

        printf("Server: Received packet %s\n", buffer);

        if (rand() % 100 < ack_chance) // rand() % 100 genrates random number between 0 to 99 if it is less than ack_chance ack is send
        {
            printf("Server: ACK sent\n\n");
            send(client, "ACK", 3, 0);
        }
        else
        {
            printf("Server: ACK lost\n\n");
        }
    }

    close(client);
    close(server);
    return 0;
}
