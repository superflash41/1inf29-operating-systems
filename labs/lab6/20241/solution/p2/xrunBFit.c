#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

// 20211866 - Saymon Estefano Nicho Manrique
// this codes simulates the use of the malloc function

void* xmalloc(size_t nbytes);
void xfree(void* ptr);
void showHoles();

int main(void) {
    unsigned int* pt1, *pt2, *pt3, *pt4;

    printf("=============================================== \n");
    printf("Demonstration of BEST-FIT Allocation: \n");
    printf("=============================================== \n");

    printf("1 initial allocation of 2048 bytes:\n");
    pt1 = (unsigned int*)xmalloc(2048);
    printf("Allocated 2048 bytes at %p\n", pt1);
    showHoles();

    printf("2 allocate 4096 bytes:\n");
    pt2 = (unsigned int*)xmalloc(4096);
    printf("Allocated 4096 bytes at %p\n", pt2);
    showHoles();

    printf("3 free the first 2048 bytes block:\n");
    xfree(pt1);
    showHoles();

    printf("4 allocate 1024 bytes (should fit into the 2048 byte hole):\n");
    pt3 = (unsigned int*)xmalloc(1024);
    printf("Allocated 1024 bytes at %p\n", pt3);
    showHoles();

    printf("5 allocate 512 bytes (best-fit should find the smallest suitable hole):\n");
    pt4 = (unsigned int*)xmalloc(512);
    printf("Allocated 512 bytes at %p\n", pt4);
    showHoles();

    printf("6 free the 4096 bytes block:\n");
    xfree(pt2);
    showHoles();

    printf("7 free all remaining blocks:\n");
    xfree(pt3);
    xfree(pt4);
    xfree(pt1);
    showHoles();

    printf("\n=============================================== \n");
    printf("Best-Fit Allocation Test Complete.\n");
    printf("=============================================== \n");
    printf("We can see how the holes are stored in a circular linked list,\n");
    printf("but the traversing of the list always starts from the beginning\n");
    printf("The algorithm works as follows: we always start from the beginning\n");
    printf("and we search for the space that is bigger.\n");
    printf("Then, we choose this hole and add the small hole to the list of holes.\n");
    printf("This is what makes the algorithm best-fit.\n");
    exit(0);
}
