/*
 * ssoo/xalloc.98/xalloc.c
 *
 * CONTENIDO INICIAL:
 *	Codigo correspondiente a la Seccion 8.7 del libro:
 *	"The C Programing Language", de B. Kernigham y D. Ritchie.
 *
 * En este fichero se incluiran las rutinas pedidas 
 * Part of the code have been modified by: 20211866 - Saymon Estefano Nicho Manrique
 */

#include <stdio.h>
#include <unistd.h>
#include "xalloc.h"

#define ul unsigned long

typedef long Align; // for alignment to long boundary

union header {   /* block header */
	struct {
		union header *ptr; // next hole in the list
		size_t size;       // size of this hole based on a Header's size
	} s;
	Align x;               // forces the alignment of blocks
};

typedef union header Header;
static Header base;   /* empty list to get started */
static Header* freep = NULL;  /* will be used for pointing at the previous of the best */
#define NALLOC 1024  /* minimum #units to request */

static Header* morecore(size_t nu, Header* prevp) {
	printf("Calling morecore...\n");
	if (nu < NALLOC) nu = NALLOC; // at least NALLOC
	char* cp = sbrk(nu * sizeof(Header)); // ask for more memory
	if (cp == (char*)-1) return NULL;
	Header* up = (Header*)cp;
	up->s.size = nu;
	xfree((void*)(up+1));
    return prevp;
}

/* xmalloc: general-purpose storage allocator */
void *xmalloc (size_t nbytes) {
	int start = 0; // flag
	Header* p, *prevp, *best = NULL; // pointer best added for best-fit
	size_t nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;
	printf("Size (in units) needed: %ld\n", (ul)nunits);
	if (!freep) { // list is empty
        base.s.ptr = freep = prevp = &base; 
		base.s.size = 0;
		start = 1; // initialization of the flag
	}
	printf("Before malloc\n");
	showHoles();
	if (start && !(p=morecore(nunits, prevp))) return NULL;
	for (p=base.s.ptr; ; prevp=p, p=p->s.ptr) {
        if (p->s.size >= nunits) {
            if (!best) best = p; 
            else if (p->s.size >= best->s.size) {
                freep = prevp; // we save the previous of the new best
                best = p; // best gets updated
            }
        }
		if (p >= p->s.ptr && (best || !(p=morecore(nunits, prevp)))) break; 
	}
	if (best) { // we will show the best
	    printf("Best: %lx\n\n", (ul)best);
		if (best->s.size == nunits) {
			printf("Best has the exact space!\n");
			freep->s.ptr = best->s.ptr; // best is leaving the list
		} else {
			printf("Best is bigger than the desired space...\n");
			printf("Best: %lx - size: %ld - next: %lx\n", (ul)best, best->s.size, (ul)best->s.ptr);
			printf("Size (in units) of the new hole: %ld\n", best->s.size - nunits);
			best->s.size -= nunits;
            best += best->s.size;
            best->s.size = nunits;
		}
        printf("New allocated space ---- Address:%lx - size: %ld\n", (ul)best, best->s.size);
        printf("After malloc\n");
        showHoles();
        printf("Address of the new hole (no header):%lx\n", (ul)(best+1));
		return (void*)(best+1);
	}
    return NULL; // no hole available
}

/* xfree: put block ap in the free list */
void xfree(void* ap) {
	Header* bp, *p;
	bp = (Header*)ap - 1;  /* point to block header */
	for (p=&base; !(bp > p && bp < p->s.ptr); p=p->s.ptr)
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr)) break;
	if (bp + bp->s.size == p->s.ptr) {  /* join to upper nbr */
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	} else bp->s.ptr = p->s.ptr;
	if (p + p->s.size == bp) {         /* join to lower nbr */
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	} else p->s.ptr = bp;
}

void* xrealloc(void* ptr, size_t size) { return NULL; }

// additional function used to show the holes
void showHoles(){
    printf("Holes:\n");
    printf("Freep: %lx\n", (ul)freep);
    for(Header* p=freep->s.ptr; ; p=p->s.ptr){
        printf("Address: %lx - Size:%ld / ", (ul)p, (ul)p->s.size);
        printf("Next: %lx - Size:%ld\n", (ul)(p->s.ptr), (p->s.ptr)->s.size);
        if(p==freep) break;
    }
    printf("\n");
}
