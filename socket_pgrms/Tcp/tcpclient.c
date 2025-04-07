#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int client;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};

    client = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Connection Failed\n");
        return 1;
    }
    printf("Connection to server Successful\n");
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        printf("Enter the message to server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        send(client, buffer, strlen(buffer), 0);
        if (strcmp(buffer, "fuckoff") == 0)
        {
            break;
        }
        memset(buffer, 0, BUFFER_SIZE);
        recv(client, buffer, BUFFER_SIZE, 0);
        printf("Message from server : %s\n", buffer);
        if (strcmp(buffer, "fuckoff") == 0)
        {
            break;
        }
    }
    close(client);
    return 0;
}