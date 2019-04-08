########################################################
CC = gcc
CFLAGS = -g -Wall -pedantic
LDLIBS = -lrt -pthread
EJS = ejercicio2 ejercicio2_solved ejercicio3_productor ejercicio3_consumidor
########################################################
OBJECTS2 = ejercicio2.o
OBJECTS2S = ejercicio2_solved.o
OBJECTS3P = ejercicio3_productor.o queue.o
OBJECTS3C = ejercicio3_consumidor.o queue.o
########################################################

all : $(EJS)

clean:
	rm -rf *.o $(EJS)

### EJS

ejercicio2: $(OBJECTS2)
	$(CC) $(CFLAGS) -o ejercicio2 $(OBJECTS2) $(LDLIBS)

ejercicio2.o: ejercicio2.c
	$(CC) $(CFLAGS) -c ejercicio2.c

ejercicio2_solved: $(OBJECTS2S)
	$(CC) $(CFLAGS) -o ejercicio2_solved $(OBJECTS2S) $(LDLIBS)

ejercicio2_solved.o: ejercicio2_solved.c
	$(CC) $(CFLAGS) -c ejercicio2_solved.c

ejercicio3_productor: $(OBJECTS3P)
	$(CC) $(CFLAGS) -o ejercicio3_productor $(OBJECTS3P) $(LDLIBS)

ejercicio3_productor.o: ejercicio3_productor.c queue.h
	$(CC) $(CFLAGS) -c ejercicio3_productor.c

ejercicio3_consumidor: $(OBJECTS3C)
	$(CC) $(CFLAGS) -o ejercicio3_consumidor $(OBJECTS3C) $(LDLIBS)

ejercicio3_consumidor.o: ejercicio3_consumidor.c queue.h
	$(CC) $(CFLAGS) -c ejercicio3_consumidor.c

### TAD

queue.o: queue.c queue.h types.h
	$(CC) $(CFLAGS) -c queue.c

