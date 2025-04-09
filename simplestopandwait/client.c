// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define TIMEOUT 3

int main()
{
    int client;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};
    char packet[50];
    struct timeval tv = {TIMEOUT, 0};

    client = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    connect(client, (struct sockaddr *)&server_addr, sizeof(server_addr));
    setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv)); // If the client waits more than TIMEOUT seconds (e.g., 3 seconds) for an ACK and doesn't receive anything, read() will stop waiting and return.

    printf("Client: Connected to server.\n");

    int i = 1;

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        if (i > 5)
            break; // Stop after sending 5 packets

        sprintf(packet, "%d", i);
        printf("Client: Sending packet %d...\n", i);
        send(client, packet, strlen(packet), 0);

        int recv_size = recv(client, buffer, BUFFER_SIZE, 0);

        if (recv_size > 0 && strcmp(buffer, "ACK") == 0)
        {
            printf("Client: ACK received for packet %d\n\n", i);
            i++;
        }
        else
        {
            printf("Client: Timeout! Retransmitting packet %d...\n\n", i);
            // i remains the same, so same packet is resent
        }
    }

    printf("Client: All packets sent.\n");
    close(client);
    return 0;
}
