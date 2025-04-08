#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int client;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE], ack[BUFFER_SIZE];

    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(client, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection Failed");
        return 1;
    }

    printf("Connected to server\n");

    int current_frame = 1, total_frame = 5;

    while (current_frame <= total_frame)
    {
        int acknowledged = 0;
        while (!acknowledged)
        {
            sprintf(buffer, "frame%d", current_frame);
            send(client, buffer, strlen(buffer), 0);
            printf("Sent Frame %d\n", current_frame);

            memset(ack, 0, sizeof(ack));
            recv(client, ack, sizeof(ack), 0);

            if (strncmp(ack, "NAK", 3) == 0)
            {
                printf("Received NAK for frame %d. Resending...\n", current_frame);
                sleep(1);
                continue;
            }

            int ack_num = atoi(ack);
            if (ack_num == current_frame + 1)
            {
                printf("ACK received for frame %d\n", current_frame);
                current_frame++;
                acknowledged = 1;
            }
            else
            {
                printf("Unexpected ACK. Resending frame %d...\n", current_frame);
            }

            sleep(1);
        }
    }

    close(client);
    printf("All frames sent successfully. Connection closed.\n");
    return 0;
}
