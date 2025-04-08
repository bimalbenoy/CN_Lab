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

struct Link
{
    int hop, dest, wt;
};
int main()
{
    int client;
    struct sockaddr_in server_addr;

    client = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(client, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Connection Failed\n");
        return 1;
    }
    printf("Connection to server Successful\n");

    int H, L, S;
    printf("Enter number of nodes: ");
    scanf("%d", &H);
    printf("Enter number of links: ");
    scanf("%d", &L);
    printf("Enter source: ");
    scanf("%d", &S);

    struct Link *links = (struct Link *)malloc(L * sizeof(struct Link));

    for (int i = 0; i < L; i++)
    {
        printf("Link %d-Enter source, destination, weight: ", i + 1);
        scanf("%d%d%d", &links[i].hop, &links[i].dest, &links[i].wt);
    }
    // sending data
    send(client, &H, sizeof(int), 0);
    send(client, &L, sizeof(int), 0);
    send(client, &S, sizeof(int), 0);
    send(client, links, L * sizeof(struct Link), 0);

    int hasCycle;
    int *dist = (int *)malloc(H * sizeof(int));
    recv(client, &hasCycle, sizeof(int), 0);
    recv(client, dist, H * sizeof(int), 0);

    if (hasCycle)
    {
        printf("Graph contains cycle\n");
    }
    else
    {
        printf("\nHop\tDistance from Source\n");
        for (int i = 0; i < H; i++)
        {
            printf("%d\t%d\n", i, dist[i]);
        }
    }

    free(dist);
    free(links);

    close(client);

    return 0;
}