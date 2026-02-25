#pragma once

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}
