#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10

void* tfun(void* tid) {
    long id = (long)tid;
    fprintf(stderr, "hello world! greetings from thread TID[%ld]\n", id);
    pthread_exit(NULL);
}

int main(void) {
    pthread_t threads[N];
    int status;
    for (long i=0; i<N; i++) {
        printf("main here! creating thread %ld\n", i);
        status = pthread_create(&threads[i], NULL, tfun, (void*)i);
        if (status) {
            printf("oops, pthread create returned error code %d\n", status);
            exit(-1);
        }
    }
    exit(0);
}
