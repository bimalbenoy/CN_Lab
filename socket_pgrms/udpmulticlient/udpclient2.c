#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int client;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(server_addr);
    char buffer[BUFFER_SIZE] = {0};

    client = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    printf("UDP Client connected on port %d...\n", PORT);
    printf("Enter 'fuckoff' to stop communicating\n");

    sendto(client, "client 2 is here", strlen("client 2 is here"), 0, (struct sockaddr *)&server_addr, addr_size);

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int recv_size = recvfrom(client, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_size);
        if (recv_size < 0)
        {
            printf("Error\n");
            break;
        }
        printf("Message from client1: %s \n", buffer);
        if (strcmp(buffer, "fuckoff") == 0)
        {
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        printf("Enter the message to client1:");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        sendto(client, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, addr_size);
        if (strcmp(buffer, "fuckoff") == 0)
        {
            break;
        }
    }

    close(client);
}