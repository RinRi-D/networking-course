#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

// TODO: fix over buf overflow
void fillHttpReq(char *buf, char *host, size_t size) {
    char *req = "GET / HTTP/1.1\r\nHost: \0";
    memcpy(buf, req, strlen(req));
    memcpy(buf + strlen(buf), host, strlen(host));
    memcpy(buf + strlen(buf), "\r\n\r\n", strlen("\r\n\r\n"));
}

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
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(argv[1], (argc > 2 ? argv[2] : "80"), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    int s = socket(servinfo->ai_family, servinfo->ai_socktype,
                   servinfo->ai_protocol);
    printf("%d\n", s);

    int connection;
    if ((connection = connect(s, servinfo->ai_addr, servinfo->ai_addrlen)) ==
        -1) {
        fprintf(stderr, "connection error: %d\n", connection);
        exit(1);
    }

    char buf[10000];
    fillHttpReq(buf, argv[1], 10000);
    printf("%s\n", buf);
    // TODO: send throw a loop
    send(s, buf, 10000, 0);
    memset(buf, 0, 10000);
    recv(s, buf, 10000, 0);
    printf("%s\n", buf);
    close(s);
    freeaddrinfo(servinfo);
}
