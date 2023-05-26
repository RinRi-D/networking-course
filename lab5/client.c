#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP_ADDR "127.0.0.1"
#define SERVER_PORT "2000"
#define SECRET_NUMBER 42

int main() {
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct sockaddr_storage client_addr;
    socklen_t addrlen = sizeof client_addr;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((status = getaddrinfo(SERVER_IP_ADDR, SERVER_PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    int s = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    int l = 1, r = SECRET_NUMBER+1;
    while (l < r) {
        int m = l + (r - l) / 2;
        char str[32];
        sprintf(str, "%d", m);
        sendto(s, str, 32, 0, servinfo->ai_addr, servinfo->ai_addrlen);

        char res[32];
        memset(res, 0, 32);
        if (recvfrom(s, res, 32, 0,
                    (struct sockaddr *)&client_addr, &addrlen) < 0) {
            printf("Failed to receive a message from client\n");
        }

        printf("res: %s\n", res);
        if (strcmp(res, "LESS\0") == 0)
            l = m;
        else if (strcmp(res, "MORE\0") == 0)
            r = m;
        else if (strcmp(res, "WIN\0") == 0) {
            close(s);
            freeaddrinfo(servinfo);
            printf("Secret Number: %s\n", str);
            exit(0);
        } else {
            fprintf(stderr, "unexpected error: %s\n", res);
            exit(1);
        }

    }

    close(s);
    freeaddrinfo(servinfo);
}
