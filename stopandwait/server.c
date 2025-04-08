#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
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
    char buffer[BUFFER_SIZE], ack[BUFFER_SIZE];
    int current_frame = 1;
    int total_frame = 5;

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        return 1;
    }

    if (listen(server, 5) == 0)
        printf("Server is listening on port %d\n", PORT);
    else
    {
        perror("Listen failed");
        return 1;
    }

    client = accept(server, (struct sockaddr *)&client_addr, &addr_size);
    if (client < 0)
    {
        perror("Accept failed");
        return 1;
    }

    printf("Client connected\n");
    srand(time(NULL));

    while (total_frame > 0)
    {
        memset(buffer, 0, sizeof(buffer));
        recv(client, buffer, sizeof(buffer), 0);

        if (strncmp(buffer, "frame", 5) != 0)
        {
            printf("Invalid message received\n");
            break;
        }

        int frame_no = atoi(&buffer[5]);
        if (frame_no == current_frame)
        {
            printf("Frame %d received\n", frame_no);

            int drop = rand() % 2;
            if (drop == 0)
            {
                printf("Simulating ACK loss for frame %d\n", frame_no);
                strcpy(ack, "NAK");
                send(client, ack, strlen(ack), 0);
                continue; // Ask client to resend
            }

            current_frame++;
            total_frame--;

            sprintf(ack, "%d", current_frame);
            send(client, ack, strlen(ack), 0);
            printf("ACK sent for frame %d\n", frame_no);
        }
        else
        {
            printf("Duplicate/out-of-order frame: %s.\n", buffer);
            strcpy(ack, "NAK");
            send(client, ack, strlen(ack), 0);
        }
    }

    close(client);
    close(server);
    printf("All frames received. Server shutting down.\n");
    return 0;
}
