
/*
 * ssoo/xalloc.98/xalloc.c
 *
 * CONTENIDO INICIAL:
 *	Codigo correspondiente a la Seccion 8.7 del libro:
 *	"The C Programing Language", de B. Kernigham y D. Ritchie.
 *
 * Part of the code has been modified by: 20211866 - Saymon Estefano Nicho Manrique
 *
 */

#include <stdio.h>
#include <unistd.h>
#include "xalloc.h"

#define ul unsigned long

typedef long Align; // for alignment to long boundary

union header {   /* block header: */
	struct {
		union header *ptr; // next hole in the list
		size_t size;       // size of this hole based on a Header's size
	} s;
	Align x;               // forces the alignment of blocks
};

typedef union header Header;
static Header base;   /* empty list to get started */
static Header* freep = NULL;  /* start of the free list */
#define NALLOC 1024  /* minimum #units to request */

static Header* morecore(size_t nu) {
	if (nu < NALLOC) nu = NALLOC; // at least NALLOC
	char* cp = sbrk(nu * sizeof(Header)); // ask for more memory
	if (cp == (char*) -1) /* no space at all */
		return NULL;
	Header* up = (Header*)cp;
	up->s.size = nu;
	xfree((void*)(up+1));
    return freep;
}

/* xmalloc: general-purpose storage allocator */

void *xmalloc (size_t nbytes) {
	Header* p, *prevp;
	size_t nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;
	if (!(prevp=freep)) { // list is empty
        base.s.ptr = freep = prevp = &base; 
		base.s.size = 0;
	}
	for (p=prevp->s.ptr; ; prevp=p, p=p->s.ptr) {
		if (p->s.size >= nunits) {  /* big enough */
			if (p->s.size == nunits)  /* exactly */
				prevp->s.ptr = p->s.ptr; // remove the hole from the list
			else {  /* allocate tail end */
				p->s.size -= nunits;
				p += p->s.size;
				p->s.size = nunits;
			}
			freep = prevp; /* estrategia next-fit */
			return (void*)(p+1); // return the pointer to the data area
		}
		if (p == freep) /* wrapped around free list */
			if (!(p=morecore(nunits))) return NULL;  /* none left */
	}
}

void xfree(void* ap) {
	Header* bp, *p;
	bp = (Header*)ap - 1;  /* point to block header */
	for (p=freep; !(bp > p && bp < p->s.ptr); p=p->s.ptr)
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr)) break;
	if (bp + bp->s.size == p->s.ptr) {  /* join to upper nbr */
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	} else bp->s.ptr = p->s.ptr;
	if (p + p->s.size == bp) {         /* join to lower nbr */
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	} else p->s.ptr = bp;
	freep = p; /* estrategia next-fit */
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
