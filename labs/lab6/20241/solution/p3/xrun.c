#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

// 20211866 - Saymon Estefano Nicho Manrique
// this code simulates the use of the malloc, free, and realloc functions

void* xmalloc(size_t nbytes);
void xfree(void* ptr);
void* xrealloc(void* ptr, size_t size);
void showHoles();

int main(void) {
    unsigned int *pt1, *pt2, *pt3, *pt4;
    printf("=============================================== \n");
    printf("Demonstration of BEST-FIT Allocation and Reallocation: \n");
    printf("=============================================== \n");

    // Initial allocations
    printf("initial allocation of 2048 bytes:\n");
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

    printf("4 Allocate 1024 bytes (should fit into the 2048-byte hole):\n");
    pt3 = (unsigned int*)xmalloc(1024);
    printf("Allocated 1024 bytes at %p\n", pt3);
    showHoles();

    printf("5 allocate 512 bytes (best-fit should find the smallest suitable hole):\n");
    pt4 = (unsigned int*)xmalloc(512);
    printf("Allocated 512 bytes at %p\n", pt4);
    showHoles();

    // realloc by shrinking an allocated block
    printf("6 Shrink the 4096 bytes block to 2048 bytes using xrealloc:\n");
    pt2 = (unsigned int*)xrealloc(pt2, 2048);
    printf("Shrunk block at %p to 2048 bytes\n", pt2);
    showHoles();

    // realloc by expanding an allocated block
    printf("7 Expand the 1024 bytes block to 3000 bytes using xrealloc:\n");
    pt3 = (unsigned int*)xrealloc(pt3, 3000);
    printf("Expanded block at %p to 3000 bytes\n", pt3);
    showHoles();

    printf("9 Free all remaining blocks:\n");
    xfree(pt4);
    xfree(pt2);
    showHoles();
    printf("\n=============================================== \n");
    printf("Best-Fit Allocation and Reallocation Test Complete.\n");
    printf("=============================================== \n");
    return 0;
}
