#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hostname>", argv[0]);
        exit(1);
    }

    int status;
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);

    struct addrinfo *servinfo;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(argv[1], "80", &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    struct addrinfo *cur = servinfo;
    while (cur) {
        void *addr;
        char *ipver;
        char ipstr[INET6_ADDRSTRLEN];


        if (cur->ai_family == AF_INET) {
            struct sockaddr_in *temp = (struct sockaddr_in *)cur->ai_addr;
            addr = &(temp->sin_addr);
            ipver = "IPv4";
        } else if (cur->ai_family == AF_INET6) {
            struct sockaddr_in6 *temp = (struct sockaddr_in6 *)cur->ai_addr;
            addr = &(temp->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(cur->ai_family, addr, ipstr, sizeof ipstr);
        printf("%s: %s\n", ipver, ipstr);

        cur = cur->ai_next;
    }

    freeaddrinfo(servinfo);
}
