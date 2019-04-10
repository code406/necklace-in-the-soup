#include <fcntl.h>
#include <mqueue.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "queue.h"

#define NAME_MAX 25
#define FILENAME "queue_data.txt"
#define SEM "/sem3b"
#define TAM 10 // Tamano de cola
int main(int argc, char *argv[]) {
  int fd;
  int c;
  int restante = TAM - 1;
  Queue *queue = NULL;
  sem_t *sem = NULL;

  /*Creamos la memoria compartida abriendola para lectura y escritura*/
  fd = open(FILENAME, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    fprintf(stderr, "Error al crear el segmento de memoria compartida\n");
    exit(EXIT_FAILURE);
  }
  /*Le damos el tamanio adecuado para almacenar la cola (puntero, la estructura
   * es privada)*/
  if (ftruncate(fd, sizeof(Queue *))) {
    fprintf(stderr,
            "Error al cambiar el tamaño del segmento de memoria compartida\n");
    unlink(FILENAME);
    exit(EXIT_FAILURE);
  }
  /*Mapeamos el segmento de memoria*/
  queue = (Queue *)mmap(NULL, sizeof(Queue *), PROT_READ | PROT_WRITE,
                        MAP_SHARED, fd, 0);
  if (queue == MAP_FAILED) {
    fprintf(stderr, "Error al mapear el segmento de memoria compartida\n");
    unlink(FILENAME);
    exit(EXIT_FAILURE);
  }
  /*Cerramos descriptor de fichero una vez mapeado*/
  close(fd);

  /*Crea y abre un semaforo de productor/consumidor*/
  if ((sem = sem_open(SEM, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) ==
      SEM_FAILED) {
    perror("sem_open");
    return -1;
  }

  /*Bucle principal: Leer caracteres de stdin, insertandolos en la cola*/
  printf("Introduce hasta %d caracteres: ", restante);
  while ((c = getchar()) != EOF) {
    sem_wait(sem);

    restante = (TAM - 1) - queue_size(queue);
    if (c == '\n')
      printf("Queda espacio para %d caracteres: ", restante);
    else if (restante > 0) {
      printf("   Insertando '%c'\n", c);
      queue_insert(queue, c);
    } else
      printf("   '%c' no insertado\n", c);

    sem_post(sem);
  }

  /*Cuando se llegue a EOF (ctrl+D en terminal), escribir '\0' en la cola*/
  printf("EOF\n   Insertando '\\0' (fin de cadena)\n");
  queue_insert(queue, '\0');

  /*Liberar la memoria compartida reservada*/
  munmap(queue, sizeof(Queue *));
  exit(EXIT_SUCCESS);
}
