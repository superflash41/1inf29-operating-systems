OBJS = xrun.o xalloc.o
BINARY = xrun
CFLAGS = -Wall -lm

xrun: $(OBJS)
	gcc -o $(BINARY) $(OBJS) $(CFLAGS)

xrun.o: xrun.c
	gcc -c xrun.c

xalloc.o: xalloc.c xalloc.h
	gcc -c xalloc.c

clean:
	rm -f $(BINARY) $(OBJS)
