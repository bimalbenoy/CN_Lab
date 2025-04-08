#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

struct Link
{
    int hop, dest, wt;
};
int main()
{
    int client, server;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    server = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Socket binding failed\n");
        return 1;
    }

    if (listen(server, 5) == 0)
    {

        printf("Server is listening on port %d\n", PORT);
    }
    else
    {
        printf("Error\n");
        return 1;
    }

    client = accept(server, (struct sockaddr *)&client_addr, &addr_size);
    if (client < 0)
    {
        printf("Error\n");
        return 1;
    }
    else
    {
        printf("Client connected\n");
    }

    int H, L, S;
    struct Link *links = (struct Link *)malloc(L * sizeof(struct Link));
    int *dist = (int *)malloc(H * sizeof(int));

    recv(client, &H, sizeof(int), 0);
    recv(client, &L, sizeof(int), 0);
    recv(client, &S, sizeof(int), 0);
    recv(client, links, L * sizeof(struct Link), 0);

    for (int i = 0; i < H; i++)
    {
        dist[i] = INT_MAX;
    }
    dist[S] = 0;

    for (int i = 0; i < H - 1; i++)
    {
        for (int j = 0; j < L; j++)
        {
            int u = links[j].hop;
            int v = links[j].dest;
            int w = links[j].wt;
            if (dist[u] != INT_MAX && dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
            }
        }
    }
    int hascycle = 0;
    for (int j = 0; j < L; j++)
    {
        int u = links[j].hop;
        int v = links[j].dest;
        int w = links[j].wt;
        if (dist[u] != INT_MAX && dist[u] + w < dist[v])
        {
            hascycle = 1;
            break;
        }
    }

    send(client, &hascycle, sizeof(int), 0);
    send(client, dist, H * sizeof(int), 0);

    free(links);
    free(dist);
    close(client);
    close(server);
    return 0;
}