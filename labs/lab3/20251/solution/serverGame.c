#include <stdio.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// sn

#define SV_SOCK_PATH "/tmp/us_xfr"
#define BACKLOG 5

int main(int argc, char* argv[]) {
    struct sockaddr_un addr;
    int sfd, cfd, n;
    if ((sfd=socket(AF_UNIX, SOCK_STREAM, 0))==-1) perror("socket");
    // construct over address and make this a listening socket
    if (remove(SV_SOCK_PATH)==-1 && errno!=ENOENT) perror("remove");
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family=AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path)-1);
    if (bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un))==-1) perror("bind");
    if (listen(sfd, BACKLOG)==-1) perror("listen");
    // handle connections
    for (;;) {
        if ((cfd=accept(sfd, NULL, NULL))==-1) perror("accept");
        read(cfd, &n, sizeof(int));
        n = (((n+7)*2)-6)/2 - n; // the operation
        write(cfd, &n, sizeof(int));
        if (close(cfd)==-1) perror("close");
    }
}