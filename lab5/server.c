#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP_ADDR "127.0.0.1"
#define SERVER_PORT 2000
#define SECRET_NUMBER 42

struct sockaddr_in server_addr, client_addr;
int socket_desc, client_struct_length = sizeof(client_addr);
char client_message[2000];

// Function to send a message to the client
int send_message(char *message)
{
    if (sendto(socket_desc, message, strlen(message), 0,
               (struct sockaddr *)&client_addr, client_struct_length) < 0)
    {
        printf("Failed to send a message to client\n");
        return -1;
    }
    return 0;
}

int main(void)
{
    // Create UDP socket
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_desc < 0)
    {
        printf("Failed to create a socket\n");
        return -1;
    }

    // Set IP and Port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket to IP:Port
    if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Failed to bind the socket\n");
        return -1;
    }

    printf("Server is listening on %s:%d\n", SERVER_IP_ADDR, SERVER_PORT);

    // Generate a random number between 1 and 42
    srand(time(NULL));
    int target = rand() % SECRET_NUMBER + 1;

    for (int i = 0; i < SECRET_NUMBER; i++)
    {
        // Receive a message from client
        memset(client_message, '\0', sizeof(client_message));
        if (recvfrom(socket_desc, client_message, sizeof(client_message), 0,
                     (struct sockaddr *)&client_addr, &client_struct_length) < 0)
        {
            printf("Failed to receive a message from client\n");
            return -1;
        }

        // Print the received message
        printf("%s:%i: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), client_message);

        // If message is not a number b/w 1 and SECRET_NUMBER send an error and continue
        int n = atoi(client_message);
        if (n <= 0 || n > SECRET_NUMBER)
        {
            if (send_message("ERROR\0"))
                return -1;
            continue;
        }

        // If a correct number was guessed. Send WIN and terminate.
        if (n == target)
        {
            if (send_message("WIN\0"))
                return -1;
            close(socket_desc);
            return 0;
        }

        // If a wrong number was guessed, send a hint
        if (n < target) {
            printf("LESS %d %d\n", n, target);
        } else if (n > target){
            printf("MORE %d %d\n", n, target);
        } else {
            printf("EQUAL %d %d\n", n, target);
        }

        if (send_message(n < target ? "LESS\0" : "MORE\0"))
            return -1;
    }

    if (send_message("LOSE\0"))
        return -1;
    close(socket_desc);

    return 0;
}
