/*  This program, server5.c, begins in similar vein to our last server,
    with the notable addition of create threads.
    The variables and the procedure of creating and naming a socket are the same.  */
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char ret_buf[32768];
char * error_return = "<HTML>\n<BODY>File not found\n</BODY>\n</HTML>";

char * read_file(char * buf, int num_buf) {
    int i;
    char *cp, *cp2;
    FILE *f;
    cp = buf + 5;
    cp2 = strstr(cp, " HTTP");
    if (cp2 != NULL) *cp2 = '\0';    
    // fetch file:
    f = fopen(cp, "r");
    if (f == NULL) return error_return;
    i = fread(ret_buf, 1, 32768, f);    
    if (i == 0) { fclose(f); return error_return; }
    ret_buf[i] = '\0';
    fclose(f);
return ret_buf;
}



void *thread_func(void * tsocket) {
	long  c_sockfd = (long)tsocket;
	char buffer[4000],*pbuf;
	int n;
	
    n=read(c_sockfd, buffer, sizeof(buffer));     
    buffer[n]='\0';
    pbuf=read_file(buffer,4000);
    write(c_sockfd,pbuf,strlen(pbuf));
    close(c_sockfd);
    pthread_exit(NULL);    
}	

int main()
{
    long server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, (socklen_t)server_len);

/*  Create a connection queue, ignore child exit details and wait for clients.  */

    listen(server_sockfd, 5);    

    while(1) {
		 pthread_t thread_id;        
        /*printf("server waiting\n");*/

/*  Accept connection.  */

        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, 
            (struct sockaddr *)&client_address, (socklen_t *)&client_len);

/*  Fork to create a process for this client and perform a test to see
    whether we're the parent or the child.  */

        int ret = pthread_create(&thread_id,NULL, thread_func, (void *)client_sockfd);
        if (ret != 0) { printf("Error from pthread: %d\n", ret); exit(1); }          
    }
}

