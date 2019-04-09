########################################################
CC = gcc
CFLAGS = -g -Wall -pedantic
LDLIBS = -lrt -pthread
EJS = ejercicio2 ejercicio2_solved ejercicio3_productor ejercicio3_consumidor ejercicio4a ejercicio3_productorB ejercicio3_consumidorB
########################################################
OBJECTS2 = ejercicio2.o
OBJECTS2S = ejercicio2_solved.o
OBJECTS3P = ejercicio3_productor.o queue.o
OBJECTS3C = ejercicio3_consumidor.o queue.o
OBJECTS3PB = ejercicio3_productorB.o queue.o
OBJECTS3CB = ejercicio3_consumidorB.o queue.o
OBJECTS4A = ejercicio4a.o
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


ejercicio3_productorB: $(OBJECTS3PB)
	$(CC) $(CFLAGS) -o ejercicio3_productorB $(OBJECTS3PB) $(LDLIBS)

ejercicio3_productorB.o: ejercicio3_productorB.c queue.h
	$(CC) $(CFLAGS) -c ejercicio3_productorB.c

ejercicio3_consumidorB: $(OBJECTS3CB)
	$(CC) $(CFLAGS) -o ejercicio3_consumidorB $(OBJECTS3CB) $(LDLIBS)

ejercicio3_consumidorB.o: ejercicio3_consumidorB.c queue.h
	$(CC) $(CFLAGS) -c ejercicio3_consumidorB.c

ejercicio4: $(OBJECTS4A)
	$(CC) $(CFLAGS) -o ejercicio4 $(OBJECTS4A) $(LDLIBS)

ejercicio4a.o: ejercicio4a.c
	$(CC) $(CFLAGS) -c ejercicio4a.c

### TAD

queue.o: queue.c queue.h types.h
	$(CC) $(CFLAGS) -c queue.c

