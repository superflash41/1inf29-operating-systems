#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define LOG 5

int main() {
    struct sockaddr_in claddr, svaddr;
    socklen_t svsz, clsz;
    int clfd, svfd = socket(AF_INET, SOCK_STREAM, 0);
    /* set up socket properties */
    svaddr.sin_family = AF_INET;
    svaddr.sin_addr.s_addr = htonl(INADDR_ANY); // any interface
    svaddr.sin_port = htons(9734);
    svsz = sizeof(svaddr);
    if(bind(svfd, (struct sockaddr*)&svaddr, svsz)) perror("bind");
    listen(svfd, LOG); // creates the connection and waits for clients 
    while (1) {
        char c;
        printf("Server waiting...\n");
        clsz = sizeof(claddr); // accept a connection
        clfd = accept(svfd, (struct sockaddr*)&claddr, &clsz);
        read(clfd, &c, sizeof(char));
        c++;
        write(clfd, &c, sizeof(char));
        close(clfd);
    }
    return EXIT_SUCCESS;
}

/* we notice the only difference is the set up of the socket properties,
* then it communicates in the same way as the UNIX stream socket */
