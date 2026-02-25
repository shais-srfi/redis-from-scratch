#include "common.h"

static int32_t query(int fd, const char *text) {
    // check if length is correct
    uint32_t len = (uint32_t)strlen(text);
    if (len > k_max_msg) {
        return -1;
    }

    // send request
    char wbuf[4 + k_max_msg];
    memcpy(wbuf, &len, 4);
    memcpy(&wbuf[4], text, len);
    if (int32_t err = write_all(fd, wbuf, 4 + len)) {
        return err;
    }

    // 4 byte header
    char rbuf[4 + k_max_msg];
    errno = 0;
    int32_t err = read_full(fd, rbuf, 4);
    if (err) {
        msg(errno == 0 ? "EOF" : "read() error");
        return err;
    }
    memcpy(&len, rbuf, 4);
    if (len > k_max_msg) {
        msg("too l  ong");
        return -1;
    }

    // reply body
    err = read_full(fd, &rbuf[4], len);
    if (err) {
        msg("read error()");
        return err;
    }
    printf("server says: %.*s\n", len, &rbuf[4]);
    return 0;
}

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

    // sending messages
    int32_t err = query(fd, "hello1");
    if (err) { goto L_DONE; }
    err = query(fd, "hello2");
    if (err) { goto L_DONE; }
L_DONE:
    close(fd);
    return 0;
}