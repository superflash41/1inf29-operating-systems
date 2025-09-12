OBJS = xrunNFit.o xalloc.o
BINARY = xrun
CFLAGS = -Wall -lm

xrun: $(OBJS)
	gcc -o $(BINARY) $(OBJS) $(CFLAGS)

xrun.o: xrunNFit.c
	gcc -c xrunNFit.c

xalloc.o: xalloc.c xalloc.h
	gcc -c xalloc.c

clean:
	rm -f $(BINARY) $(OBJS)
