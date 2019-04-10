/**
 * @file ejercicio3_consumidor.c
 * @author Juan Martin Pinilla (juan.martinp@estudiante.uam.es)
 * @author David Palomo Marcos (david.palomo@estudiante.uam.es)
 * Grupo: 2212
 * @date 10 Abr 2019
 */

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
#define SHM_NAME "/shm_ej3"
#define SEM "/sem3"

int main(int argc, char *argv[]) {
  int fd_shm;
  char c;
  Queue *queue = NULL;
  sem_t *sem = NULL;

  /*Abrimos la memoria compartida sin crearla*/
  fd_shm = shm_open(SHM_NAME, O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
  if (fd_shm == -1) {
    fprintf(stderr, "Error al crear el segmento de memoria compartida\n");
	shm_unlink(SHM_NAME);
    exit(EXIT_FAILURE);
  }
  /*Mapeamos el segmento de memoria*/
  queue = (Queue *)mmap(NULL, sizeof(Queue *), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
  if (queue == MAP_FAILED) {
    fprintf(stderr, "Error al mapear el segmento de memoria compartida\n");
    shm_unlink(SHM_NAME);
    exit(EXIT_FAILURE);
  }
  /*Cerramos descriptor de fichero una vez mapeado*/
  shm_unlink(SHM_NAME);
  close(fd_shm);

  /*Abrimos semaforo de productor/consumidor sin crearlo*/
  if ((sem = sem_open(SEM, O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }
  sem_unlink(SEM);

  /*Bucle principal: Lectura (extraccion) de la cola hasta que lee el '\0'*/
  while (c != '\0') {
    sem_wait(sem);
    if (queue_isEmpty(queue) == FALSE) {
      c = queue_extract(queue);
      if (c == '\0') printf("   Extrayendo '\\0'\nFinaliza el consumidor.\n");
      else printf("   Extrayendo '%c'\n", c);
    }
    sem_post(sem);
  }

  /*Liberar la memoria compartida reservada*/
  munmap(queue, sizeof(Queue *));
  exit(EXIT_SUCCESS);
}
