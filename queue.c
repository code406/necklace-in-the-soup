/**
 * @file queue.c
 * @author Juan Martin Pinilla (juan.martinp@estudiante.uam.es)
 * @author David Palomo Marcos (david.palomo@estudiante.uam.es)
 * Grupo: 2212
 * @date 10 Abr 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define MAXQUEUE 11
#define ERRORCHAR -1

struct _Queue {
	char items [MAXQUEUE];
	int front;
	int rear;
};

/**------------------------------------------------------------------
Inicializa la cola: reserva memoria para ella e inicializa todos sus elementos a NULL.
------------------------------------------------------------------*/
Queue* queue_ini() {
	Queue *q = NULL;
	q = (Queue*) malloc(sizeof(Queue));
	if (!q) {
		perror("Error en cola ini");
		return NULL;
	}
	/* Inicializo tope y asigno los punteros a función */
	q->rear = 0;
	q->front = 0;
	return q;
}

/**------------------------------------------------------------------
Libera la cola, liberando todos sus elementos.
------------------------------------------------------------------*/
void queue_destroy(Queue *q) {
	q->rear = 0;
	q->front = 0;
	free(q);
}

/**------------------------------------------------------------------
Inserta un nuevo nodo en la cola, reservando memoria nueva para él y haciendo una copia del elemento recibido.
------------------------------------------------------------------*/
Status queue_insert(Queue *q, char c) {
	/*si la cola esta llena no instertamos ningun elemento*/
	if (!q || queue_isFull(q)) return ERROR;
	q->items[q->rear] = c;
	/*incrementamos el rear */
	q->rear = (q->rear+1) % MAXQUEUE;
	return OK;
}

/**------------------------------------------------------------------
Extrae un elemento de la cola. Devuelve directamente el puntero al elemento (no hace copia).
------------------------------------------------------------------*/
char queue_extract(Queue *q) {
	char c;
	if (!q || queue_isEmpty(q)) return ERRORCHAR;
	c = q->items[q->front];
	/*actualizamos el front*/
	q->front = (q->front+1) % MAXQUEUE;
	return c;
}

/**------------------------------------------------------------------
Comprueba si la pila esta vacía. Entrada: puntero a la pila. Salida: TRUE si
está vacía o FALSE si no lo esta
------------------------------------------------------------------*/
Bool queue_isEmpty(const Queue *q) {
	/*si no existe la cola, se considera vacia para evitar extaccion*/
	if (!q)	return TRUE;
	if (q->rear == q->front) return TRUE;
	return FALSE;
}

/**------------------------------------------------------------------
Comprueba si la pila esta llena. Entrada: puntero a la pila. Salida: TRUE si
está llena o FALSE si no lo esta
------------------------------------------------------------------*/
Bool queue_isFull(const Queue *q) {
	/*si no existe la pila, se considera llena para evitar insercion*/
	if (!q) return TRUE;
	if (q->front == (q->rear+1) % MAXQUEUE) return TRUE;
	return FALSE;
}

/**------------------------------------------------------------------
Imprime toda la cola, devolviendo el número de caracteres escritos.
------------------------------------------------------------------*/
int queue_print(FILE *pf, const Queue *q) {
	int i, impr = 0;
	if (!pf || !q)	return -1;
	if (queue_isEmpty(q) == TRUE) return fprintf(pf, "Queue vacia\n");
	impr += fprintf(pf, "Cola con %d elementos:\n", queue_size(q));
	for (i = 0; i < queue_size(q); i++) impr += fprintf(pf, "[%c]", q->items[i]);
	return impr;
}

/**------------------------------------------------------------------
Devuelve el número de elementos de la cola.
------------------------------------------------------------------*/
int queue_size(const Queue *q) {
	int size = -1;
	if(!q) return -1;
	size = (q->rear - q->front) % MAXQUEUE;
	if (size < 0) return (MAXQUEUE + size);
	return size;
}
