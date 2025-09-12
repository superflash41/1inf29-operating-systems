#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

// 20211866 - Saymon Estefano Nicho Manrique
// this codes simulates the use of the malloc function

void* xmalloc(size_t nbytes);
void xfree(void * ptr);
void showHoles();

int main(void){
    unsigned int x = 0, unidad = 2048, base;
    unsigned int* pt;

    printf("=============================================== \n");
    printf("Demonstration of next fit: \n");
    printf("=============================================== \n");

    do {
        base=x;
        if((pt=(unsigned int*)xmalloc(base*unidad))) {
            fprintf(stdout,"We ask for %d bytes located on %p\n",base*unidad,pt);
            showHoles();
            printf("---------------------------------------- \n");
            if (x%2) {
                printf("We are freeing memory in this iteration:\n");
                xfree(pt);
                showHoles();
                printf("---------------------------------------- \n");
            }
        } else
            fprintf(stderr,"No hay suficiente memoria\n");
        x++;
    } while (x<=6);

    printf("We can see how the holes are stored in a circular list as addresses\n");
    printf("at the beginning point to each other (just 2 holes). This is the main property\n");
    printf("of next-fit algorithm.\n");
    printf("Then, we can appreciate how the algorithm starts from where it left the last\n");
    printf("iteration becuse of the order in which holes are printed. Also, when we get\n");
    printf("three holes is noticeable how the freeing is made also starting from where it left\n");
    printf("on previous iteration and also the value of freep pointer can show this.\n");
    printf("\n");
    printf("This demonstrates the use of xmalloc and xfree for the next-fit algorithm.\n");
    printf("\n");
    printf("NOTE: I include a function called showHoles(). It is declared in the xalloc.c file.\n");
    exit(0);                  
}


