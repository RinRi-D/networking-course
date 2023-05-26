#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int func() {
    return 0;
}

int main(int argc, char **argv) {
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo("127.0.0.1", "2001", &hints, &servinfo)) != 0) {
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
    printf ("%d\n", connection);

    char buf[1000], svg_start[10000], svg_text[10000], svg_end[10000];
    memset(buf, 0, 1000);
    sprintf(buf, "Amirlan");
    int bytes = send(s, buf, 1000, 0);
    printf("bytes sent %d\n", bytes);
    memset(buf, 0, 1000);
    memset(svg_start, 0, 10000);
    memset(svg_text, 0, 10000);
    memset(svg_end, 0, 10000);
    recv(s, svg_start, 10000, 0);
    recv(s, svg_text, 10000, 0);
    recv(s, svg_end, 10000, 0);

    FILE * of = fopen("Image.svg", "w");
    fwrite(svg_start, 1, strlen(svg_start), of);
    fwrite(svg_text, 1, strlen(svg_text), of);
    fwrite(svg_end, 1, strlen(svg_end), of);
    close(s);
    freeaddrinfo(servinfo);
}
