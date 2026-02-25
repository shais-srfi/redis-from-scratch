#include "common.h"
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

int main() {

    // optain socket handle
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) { die("socket()"); }

    // connecting
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);                    // connecting to defined port in server.cpp
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) { die("connect()"); }

    // sending message
    char msg[] = "hello";
    write(fd, msg, strlen(msg));

    // receive message
    char rbuf[64] = {};
    ssize_t n = read(fd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) { die("read"); }
    printf("server says: %s\n", rbuf);
    close(fd);

    return 0;
}