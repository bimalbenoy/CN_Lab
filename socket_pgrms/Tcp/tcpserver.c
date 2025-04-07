#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define PORT 8080

int main()
{
    int server, client;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    char buffer[BUFFER_SIZE] = {0};

    server = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (listen(server, 5) == 0)
    {
        printf("Connection Successful\n");
    }
    else
    {
        printf("Connection Failed\n");
        return 1;
    }
    client = accept(server, (struct sockaddr *)&client_addr, &addr_size);
    printf("Client connected\n");
    printf("Enter the  fuckoff to stop communicating\n");
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        recv(client, buffer, BUFFER_SIZE, 0);
        printf("Message from server : %s\n", buffer);
        if (strcmp(buffer, "fuckoff") == 0)
        {
            break;
        }
        memset(buffer, 0, BUFFER_SIZE);
        printf("Enter the message to client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        send(client, buffer, strlen(buffer), 0);
        if (strcmp(buffer, "fuckoff") == 0)
        {
            break;
        }
    }

    close(client);
    close(server);
    return 0;
}