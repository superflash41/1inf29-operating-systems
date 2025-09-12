#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SV_PATH "/tmp/sck"
#define BFSZ 100

int main(void) { /* an example of UNIX stream socket client */
    struct sockaddr_un addr;
    ssize_t nr;
    char buf[BFSZ];
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd==-1) perror("socket");
    // construct sv address and make the connection
    memset(&addr, 0, sizeof(struct sockaddr_un)); // clear the space for the socket structure
    addr.sun_family = AF_UNIX; // manual set up of the socket's type
    strncpy(addr.sun_path, SV_PATH, sizeof(addr.sun_path)-1); // manual set up of the path
    if (connect(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un))==-1)
        perror("connect");
    while ((nr=read(STDIN_FILENO, buf, BFSZ)))
        if (write(sfd, buf, nr)!=nr) perror("write"); // copy stdin to socket
    if (nr==-1) perror("read");
    exit(EXIT_SUCCESS); // closes the socket of the cl, sv sees EOF
}

/* for the sv: socket() -> connect() -> close() */
