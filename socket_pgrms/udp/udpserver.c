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
    int server, client;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};

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

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int recv_size = recvfrom(server, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_size);
        if (recv_size < 0)
        {
            printf("Error\n");
            break;
        }
        printf("Message from client: %s \n", buffer);
        if (strcmp(buffer, "fuckoff") == 0)
        {
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        printf("Enter the message to client:");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        sendto(server, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, addr_size);
        if (strcmp(buffer, "fuckoff") == 0)
        {
            break;
        }
    }
    close(server);
    close(client);
}