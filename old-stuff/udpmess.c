#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char **argv) {
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <hostname> <port (optional)>", argv[0]);
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((status = getaddrinfo(argv[1], (argc > 2 ? argv[2] : "80"), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    int s = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    char str[10] = "Hi Mom!";
    sendto(s, str, 10, 0, servinfo->ai_addr, servinfo->ai_addrlen);
}
