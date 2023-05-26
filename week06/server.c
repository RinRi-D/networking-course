#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP_ADDR "127.0.0.1"
#define SERVER_PORT 2001

char *svg_begin = "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1000\" height=\"1000\" style=\"shape-rendering:geometricPrecision;text-rendering:geometricPrecision;image-rendering:optimizeQuality;fill-rule:evenodd;clip-rule:evenodd\"><path fill=\"#7fbc00\" d=\"M803 335c7-1 13 2 18 9l92 164c7 10 6 19-2 28L795 639l-2 103c-2 7-7 11-14 12H213c-9-2-14-7-16-15l-1-37-21-29-3-11 23-191c3-10 10-15 20-15l179 13 409-134Zm-11 43c3 0 5 1 7 4l75 134-9 10a7347 7347 0 0 0-108 99l-1 86-2 4-259 2-257-1-2-1-3-26a468 468 0 0 0-22-36l18-149c0-6 2-9 7-9l165 11 391-128Z\" style=\"opacity:.974\"/><path fill=\"#7fbc00\" d=\"M675 453c11-3 19 2 23 13v201c-3 7-9 12-18 13-9-1-15-6-18-13V465c3-6 7-10 13-12Z\" style=\"opacity:.994\"/><path fill=\"#7fbc00\" d=\"M770 453c11-3 19 2 23 13v77c-7 12-16 16-29 9-4-2-6-6-7-9v-78c3-6 7-10 13-12Z\" style=\"opacity:.981\"/><path fill=\"#7fbc00\" d=\"M487 505c10-1 17 3 21 13v162c-3 8-9 12-18 13-9-1-15-5-18-13V518c3-8 7-12 15-13Z\" style=\"opacity:.991\"/><path fill=\"#7fbc00\" d=\"M581 516c10-2 18 3 22 13v108c-3 7-9 12-18 13-9-1-15-6-18-13V529c2-7 7-11 14-13Z\" style=\"opacity:.988\"/><path fill=\"#7fbc00\" d=\"M299 528c11-1 18 4 21 16v129c-3 12-11 17-24 15-7-2-11-7-13-13V542c3-8 8-13 16-14Z\" style=\"opacity:.963\"/><path fill=\"#7fbc00\" d=\"M390 562c12-3 20 2 24 14v84c-4 11-12 16-24 13-8-3-12-8-13-15v-80c1-8 5-14 13-16Z\" style=\"opacity:.968\"/>";
char *svg_text = "<text x=\"350\" y=\"840\" style=\"font-size:72px;fill:#000\">Hello, %s!</text>";
char *svg_end = "</svg>";

struct sockaddr_in server_addr, client_addr;
int welcoming_sock, connection_sock, client_struct_length = sizeof(client_addr);
char client_message[2000], server_message[2000];

// Sends a message to the accepted client through the connection socket.
int send_message(char *message)
{
    if (send(connection_sock, message, strlen(message), 0) < 0)
    {
        printf("Failed to send a message to client\n");
        return -1;
    }
    return 0;
}

int main(void)
{
    // Create TCP socket
    welcoming_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (welcoming_sock < 0)
    {
        printf("Failed to create a socket\n");
        return -1;
    }

    // Set IP and Port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
    server_addr.sin_port = htons(SERVER_PORT);

    int yes=1, status;

    /* when port is already bound to this program */
    if ((status = setsockopt(welcoming_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes)) == -1) {
        fprintf(stderr, "setsockopt error: %d\n", status);
        exit(1);
    }

    // Bind the socket to IP:Port
    if (bind(welcoming_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Failed to bind the socket\n");
        return -1;
    }

    // Listen for one incoming TCP connection
    if (listen(welcoming_sock, 1) < 0)
    {
        printf("Failed to listen to incoming connections\n");
        return -1;
    }

    printf("Server is listening on %s:%d\n", SERVER_IP_ADDR, SERVER_PORT);

    // Accept an incoming client connection
    connection_sock = accept(welcoming_sock, (struct sockaddr *)&client_addr, &client_struct_length);
    if (connection_sock < 0)
    {
        printf("Failed to accept the client connection\n");
        return -1;
    }

    // Receive the client's username
    if (recv(connection_sock, client_message, sizeof(client_message), 0) < 0)
    {
        printf("Failed to receive a message from client\n");
        return -1;
    }

    // Send a vector image to the client
    send_message(svg_begin);
    sprintf(server_message, svg_text, client_message);
    send_message(server_message);
    send_message(svg_end);

    // Close the sockets
    close(connection_sock);
    close(welcoming_sock);

    return 0;
}
