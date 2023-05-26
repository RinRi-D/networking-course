#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hostname>", argv[0]);
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(argv[1], "443", &hints, &servinfo)) != 0) {
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

    SSL_load_error_strings();
    SSL_library_init();
    SSL_CTX *ssl_ctx = SSL_CTX_new(SSLv23_client_method());


    SSL *conn = SSL_new(ssl_ctx);
    SSL_set_fd(conn, s);

    int err;
    if ((err = SSL_connect(conn)) != 1) {
        fprintf(stderr, "SSL_connect error: %d\n", err);
        exit(1);
    }

    char buf[10000];
    fillHttpReq(buf, argv[1], 10000);
    printf("%s\n", buf);
    // TODO: send throw a loop
    SSL_write(conn, buf, 10000);
    memset(buf, 0, 10000);
    SSL_read(conn, buf, 10000);
    printf("%s\n", buf);
    close(s);
    freeaddrinfo(servinfo);
}
