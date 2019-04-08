#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"

typedef struct _Queue Queue;

/**------------------------------------------------------------------
Inicializa la cola: reserva memoria para ella e inicializa todos sus elementos a NULL.
------------------------------------------------------------------*/
Queue* queue_ini();

/**------------------------------------------------------------------
Libera la cola, liberando todos sus elementos.
------------------------------------------------------------------*/
void queue_destroy(Queue *q);

/**------------------------------------------------------------------
Inserta un nuevo nodo en la cola, reservando memoria nueva para él y haciendo una copia del elemento recibido.
------------------------------------------------------------------*/
Status queue_insert(Queue *q, char c);

/**------------------------------------------------------------------
Extrae un elemento de la cola. Devuelve directamente el puntero al elemento (no hace copia).
------------------------------------------------------------------*/
char queue_extract(Queue *q);

/**------------------------------------------------------------------
Comprueba si la pila esta vacía. Entrada: puntero a la pila. Salida: TRUE si
está vacía o FALSE si no lo esta
------------------------------------------------------------------*/
Bool queue_isEmpty(const Queue *q);

/**------------------------------------------------------------------
Comprueba si la pila esta llena. Entrada: puntero a la pila. Salida: TRUE si
está llena o FALSE si no lo esta
------------------------------------------------------------------*/
Bool queue_isFull(const Queue *q);

/**------------------------------------------------------------------
Imprime toda la cola, devolviendo el número de caracteres escritos.
------------------------------------------------------------------*/
int queue_print(FILE *pf, const Queue *q);

/**------------------------------------------------------------------
Devuelve el número de elementos de la cola.
------------------------------------------------------------------*/
int queue_size(const Queue *q);

#endif /* QUEUE_H */
