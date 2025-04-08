#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5040
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
        perror("Socket connection failed");
        return 1;
    }

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        printf("Enter the File name:");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        send(client, buffer, strlen(buffer), 0);
        if (strcmp(buffer, "exit") == 0)
        {
            break;
        }

        while (1)
        {
            int rev_size = recv(client, buffer, BUFFER_SIZE, 0);
            if (rev_size < 0)
            {
                printf("Error in receiving file");
            }
            buffer[rev_size] = '\0';
            if (strncmp(buffer, "EOF", 3) == 0)
            {
                printf("\n");
                break;
            }
            printf("%s", buffer);
        }
    }
    close(client);
}