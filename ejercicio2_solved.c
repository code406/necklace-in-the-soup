/**
 * @file ejercicio2_solved.c
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

#define NAME_MAX 25
#define SHM_NAME "/shm_ej2"
#define SEM "/sem2"
#define MAXTIME 10

typedef struct {
  int previous_id;     //!< Id of the previous client.
  int id;              //!< Id of the current client.
  char name[NAME_MAX]; //!< Name of the client.
} ClientInfo;

/*Manejador vacio para SIGUSR1*/
void manejador(int sig) {}

int main(int argc, char *argv[]) {
  int nsons, i, fd_shm;
  pid_t pid, ppid;
  struct sigaction act;
  ClientInfo *clientinfo = NULL;
  sem_t *sem = NULL;

  if (argc < 2) {
    printf("El programa requiere el numero de hijos a crear:\n");
    printf("Terminando la ejecucion.\n");
    exit(EXIT_FAILURE);
  }
  nsons = atoi(argv[1]); /*Numero de hijos que se quieren*/
  if (nsons < 0) {
    printf("Introduce un numero positivo\n");
    exit(EXIT_FAILURE);
  }

  /*Crea y abre un semaforo*/
  if ((sem = sem_open(SEM, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
    perror("sem_open");
    return -1;
  }
  sem_unlink(SEM);

  sigemptyset(&(act.sa_mask));
  act.sa_flags = 0;
  /* Se arma la señal SIGUSR1. */
  act.sa_handler = manejador;
  if (sigaction(SIGUSR1, &act, NULL) < 0) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  /*Creamos la memoria compartida abriendola para lectura y escritura*/
  fd_shm = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
  if (fd_shm == -1) {
    fprintf(stderr, "Error al crear el segmento de memoria compartida\n");
    exit(EXIT_FAILURE);
  }
  /*Le damos el tamanio adecuado, en este caso el de la estructura*/
  if (ftruncate(fd_shm, sizeof(ClientInfo))) {
    fprintf(stderr, "Error al cambiar el tamaño del segmento de memoria compartida\n");
    shm_unlink(SHM_NAME);
    exit(EXIT_FAILURE);
  }
  /*Mapeamos el segmento de memoria*/
  clientinfo = mmap(NULL, sizeof(*clientinfo), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
  if (clientinfo == MAP_FAILED) {
    fprintf(stderr, "Error al mapear el segmento de memoria compartida\n");
    shm_unlink(SHM_NAME);
    exit(EXIT_FAILURE);
  }
  /*Liberamos una vez mapeado*/
  shm_unlink(SHM_NAME);
  close(fd_shm);

  /*Inicializamos memoria*/
  clientinfo->id = 0;
  clientinfo->previous_id = -1;

  ppid = getpid();
  for (i = 0; i < nsons; i++) {
    if ((pid = fork()) < 0) {
      printf("ERROR en fork.\n");
      exit(EXIT_FAILURE);
    }
    /*Hijo*/
    else if (pid == 0) {
      sem_wait(sem);
      srand(getpid());
      /*Duerme un tiempo aleatorio entre 1 y 10 segundos*/
      sleep((rand() % MAXTIME) + 1);
      /*Incrementa el id del cliente previo*/
      clientinfo->previous_id++;
      /*Solicita nombre y lo escribe en memoria compartida*/
      printf("Introduzca nombre para un cliente nuevo: ");
      fgets(clientinfo->name, sizeof(char) * NAME_MAX, stdin);
      /*Incrementa el id del cliente*/
      clientinfo->id++;
      /*Envia la señal SIGUSR1 al proceso padre*/
      if (kill(ppid, SIGUSR1) == -1) {
        fprintf(stderr, "Error al enviar SIGUSR1\n");
        exit(EXIT_FAILURE);
      }
      sem_post(sem);
      exit(EXIT_SUCCESS);
    }
    /*Padre*/
    else {
      pause();
      sem_wait(sem);
      printf("Leyendo memoria compartida: \n  Id_Previo: %d\n  Id: %d\n  Nombre: %s",
             clientinfo->previous_id, clientinfo->id, clientinfo->name);
      sem_post(sem);
    }
  }

  /*Solo el padre*/
  for (i = 0; i < nsons; i++)
    wait(NULL);

  /*Liberar la memoria compartida reservada*/
  munmap(clientinfo, sizeof(*clientinfo));
  exit(EXIT_SUCCESS);
}
