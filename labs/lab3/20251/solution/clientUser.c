#include <stdio.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// sn

#define SV_SOCK_PATH "/tmp/us_xfr"
#define BACKLOG 5

int main(int argc, char* argv[]) {
    struct sockaddr_un addr;
    int sfd, n;
    if ((sfd=socket(AF_UNIX, SOCK_STREAM, 0))==-1) perror("socket");
    // construct over address and make connection to server
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family=AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path)-1);
    if (connect(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un))==-1) perror("connect");
    // send data and receive results
    printf("Ingresa un numero entero: ");
    scanf("%d", &n);
    write(sfd, &n, sizeof(int));
    read(sfd, &n, sizeof(int));
    printf("Resultado: %d\n", n);
    if (close(sfd)==-1) perror("close");
    exit(EXIT_SUCCESS);
}