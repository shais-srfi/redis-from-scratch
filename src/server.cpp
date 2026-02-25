#include "common.h"

static int32_t one_request(int connfd) {
    // reads 4 byte header in stream
    char rbuf[4 + k_max_msg];
    errno = 0;
    int32_t err = read_full(connfd, rbuf, 4);
    if (err) {
        msg(errno == 0 ? "EOF" : "read() error ");
        return err;
    }
    uint32_t len = 0;
    memcpy(&len, rbuf, 4);
    if (len > k_max_msg) {
        msg("too long");
        return -1;
    }

    // request body
    err = read_full(connfd, &rbuf[4], len);
    if (err) {
        msg("read() error");
        return err;
    }

    // outputting client
    printf("client says: %.*s\n", len, &rbuf[4]);

    // replying to client
    const char reply[] = "world";
    char wbuf[4 + sizeof(reply)];
    len = (uint32_t)strlen(reply);
    memcpy(wbuf, &len, 4);
    memcpy(&wbuf[4], reply, len);
    return write_all(connfd, wbuf, 4 + len);
}

int main() {

    // obtain socket handle
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) { die("socket()"); }

    // set socket options
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    // bind to address
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);        // this is the port
    addr.sin_addr.s_addr = htonl(0);    // wildcard ip 0.0.0.0
    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) { die("bind()"); }

    // listen
    rv = listen(fd, SOMAXCONN);
    if (rv) { die("listen()"); }

    // accept connection
    while (true) {
        // accept
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
        if (connfd < 0) { continue; }   // error

        // serves one client at a time
        while (true) {
            int32_t err = one_request(connfd);
            if (err) { break; }
        }
        close(connfd);
    }

    return 0;
}