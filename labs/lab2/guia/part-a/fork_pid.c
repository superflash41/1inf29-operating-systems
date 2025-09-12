#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t child;
    if ((child = fork()))
        fprintf(stderr, "\nFork returned %d, I am the father with ID = %d\n", child, getpid());
    else
        fprintf(stderr, "\nFork returned %d, I am the child with ID = %d\n", child, getpid());
    return 0;
}
