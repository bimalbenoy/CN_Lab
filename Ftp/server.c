#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5040
#define BUFFER_SIZE 1024

int main()
{
    int server, client;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    server = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Socket binding failed");
        return 1;
    }

    if (listen(server, 5) == 0)
    {
        printf("Server is listening on port %d\n", PORT);
    }
    else
    {
        perror("Socket listening failed");
        return 1;
    }
    client = accept(server, (struct sockaddr *)&client_addr, &addr_size);
    printf("Client connected\n");
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int recv_size = recv(client, buffer, BUFFER_SIZE, 0);
        if (recv_size < 0)
        {
            break;
        }
        if (strcmp(buffer, "exit") == 0)
        {
            break;
        }
        printf("the file name is %s\n", buffer);

        FILE *f = fopen(buffer, "r");
        if (f == NULL)
        {
            char *err = "File not found";
            send(client, err, strlen(err), 0);
            continue;
        }
        while (fgets(buffer, BUFFER_SIZE, f) != NULL)
        {
            send(client, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            usleep(1000);
        }
        strncpy(buffer, "EOF", 3);
        send(client, buffer, 3, 0);
        fclose(f);
    }
    close(client);
    close(server);
    return 0;
}