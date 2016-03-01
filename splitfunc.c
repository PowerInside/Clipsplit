#include <unistd.h>
#include <stdlib.h>
#include "savesplit.h"

void buffering(int fd, char **buffer, unsigned int bufsize) {
    /* Reads from fd to dynamically sized buffer */
    ssize_t nbytes;
    const unsigned int initial_bufsize = bufsize;

    for (;;) {
        nbytes = read(fd, *buffer + (bufsize - initial_bufsize), bufsize);
        if (nbytes != bufsize)
            break;
        bufsize += initial_bufsize;
        *buffer = realloc(*buffer, bufsize);
    }
    *buffer = realloc(*buffer, bufsize - initial_bufsize + nbytes + 1);
    (*buffer)[bufsize + nbytes] = '\0';
}
