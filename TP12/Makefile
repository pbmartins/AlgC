# makefile para a fila com prioridade
CC = gcc-5
CFLAGS = -ansi -Wall

OBJM = pqueue.o
HEADERS = pqueue.h

OBJD = pqueue_dijkstra.o
HEADERSD = pqueue_dijkstra.h

OBJECTS = $(OBJM) testpqueue.o simpqueue.o teste1.o teste2.o
EXES = tpq spq teste1 teste2

all:	$(EXES)

tpq:	testpqueue.o $(OBJM)
	$(CC) $(CFLAGS) testpqueue.o $(OBJM) -o tpq

spq:	simpqueue.o $(OBJM)
	$(CC) $(CFLAGS) simpqueue.o $(OBJM) -o spq

teste1:	teste1.o $(OBJD)
	$(CC) $(CFLAGS) teste1.o $(OBJD) -o $@

teste2:	teste2.o $(OBJD)
	$(CC) $(CFLAGS) teste2.o $(OBJD) -o $@

$(OBJM):	$(HEADERS)

$(OBJD):	$(HEADERSD)

clean:
	rm -f $(OBJECTS) *~

cleanall:	clean
	rm -f $(EXES)
