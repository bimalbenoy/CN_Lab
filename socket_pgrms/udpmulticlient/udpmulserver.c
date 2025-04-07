#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_client 2

int main()
{
    int server, client;
    struct sockaddr_in server_addr, client_addr[MAX_client];
    socklen_t addr_size = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};
    int client_count = 0;

    server = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Socket binding failed\n");
        return 1;
    }
    printf("UDP Server listening on port %d...\n", PORT);
    printf("Enter 'fuckoff' to stop communicating\n");
    while (client_count < MAX_client)
    {
        memset(buffer, 0, BUFFER_SIZE);
        recvfrom(server, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr[client_count], &addr_size);
        printf("client %d is connected: %s\n", client_count + 1, buffer);
        client_count++;
    }

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        recvfrom(server, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr[0], &addr_size);
        printf("Client 1 message: %s\n", buffer);

        sendto(server, buffer, BUFFER_SIZE, 0,
               (struct sockaddr *)&client_addr[1], addr_size);

        if (strcmp(buffer, "fuckoff") == 0)
            break;

        memset(buffer, 0, BUFFER_SIZE);
        recvfrom(server, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr[1], &addr_size);
        printf("Client 2 message: %s\n", buffer);

        sendto(server, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr[0], addr_size);

        if (strcmp(buffer, "fuckoff") == 0)
            break;
    }
    close(server);
    close(client);
}