/*
 * ssoo/xalloc.98/xalloc.h
 *
 * Prototipos de las funciones pedidas
 *
 * NO MODIFICAR ESTE FICHERO
 */

#if defined(NOXALL)

#include <malloc.h>
#define xmalloc(s) malloc(s)
#define xrealloc(p, s) realloc(p, s)
#define xfree(p) free(p)
#define showHoles() showHoles()

#else

#include <stddef.h>
void *xmalloc(size_t size);
void *xrealloc(void * ptr, size_t size);
void xfree(void * ptr);
void showHoles();

#endif

