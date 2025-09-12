#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define SV_PATH "/tmp/sck"
#define BFSZ 100
#define LOG 5

int main(void) { /* an example of UNIX stream socket server */
    struct sockaddr_un addr;
    ssize_t nr;
    char buf[BFSZ];
    int cfd, sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd==-1) perror("socket");
    // construct sv address and bind socket to it
    if (remove(SV_PATH)==-1 && errno != ENOENT) perror("remove");
    memset(&addr, 0, sizeof(struct sockaddr_un)); // clear the space for the socket structure
    addr.sun_family = AF_UNIX; // manual set up of the socket's type
    strncpy(addr.sun_path, SV_PATH, sizeof(addr.sun_path)-1); // manual set up of the path
    if (bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un))==-1) perror("bind");
    if (listen(sfd, LOG)==-1) perror("listen"); // ready to start accepting
    while (1) { // handle clients iteratively
        if ((cfd=accept(sfd, NULL, NULL))==-1)
            perror("accept"); // we do not care about the new socket
        while ((nr=read(cfd, buf, BFSZ)))
            if (write(STDOUT_FILENO, buf, nr)!=nr) perror("write");
        if (nr==-1) perror("read");
        if (close(cfd)==-1) perror("close");
    }
    return 0;
}

/* for the sv: socket() -> bind() -> listen() -> accept() -> close() */
