/**
 * @file ejercicio4.c
 * @author Juan Martin Pinilla (juan.martinp@estudiante.uam.es)
 * @author David Palomo Marcos (david.palomo@estudiante.uam.es)
 * Grupo: 2212
 * @date 10 Abr 2019
 */

#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define TAMNAME 20
#define MSGSIZE 20
#define NHIJOS 3

int main(int argc, char *argv[]) {
  struct mq_attr attributes1;
  struct mq_attr attributes2;
  char ncolaenv[MSGSIZE] = "/";
  char ncolarec[MSGSIZE] = "/";
  pid_t pid;
  int i;

  attributes1.mq_flags = 0;
  attributes1.mq_maxmsg = 10;
  attributes1.mq_curmsgs = 0;
  attributes1.mq_msgsize = MSGSIZE;

  attributes2.mq_flags = 0;
  attributes2.mq_maxmsg = 10;
  attributes2.mq_curmsgs = 0;
  attributes2.mq_msgsize = MSGSIZE;

  if (argc < 4) {
    printf("El programa requiere el nombre del fichero y las dos colas:\n");
    printf("Terminando la ejecucion.\n");
    exit(EXIT_FAILURE);
  }
  /*Copiamos los argumentos con / delante*/
  strcpy(&ncolaenv[1], argv[2]);
  strcpy(&ncolarec[1], argv[3]);

  if (mq_open(ncolaenv, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attributes1) ==
      -1) {
    printf("Error al abrir la cola %s\n", ncolaenv);
    exit(EXIT_FAILURE);
  }
  if (mq_open(ncolarec, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attributes2) ==
      -1) {
    printf("Error al abrir la cola %s\n", ncolarec);
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < NHIJOS; i++) {
    if ((pid = fork()) < 0) {
      printf("ERROR en fork.\n");
      exit(EXIT_FAILURE);
    }
    /*Hijo*/
    else if (pid == 0) {
      if (i == 0) {
        if (execlp("./ejercicio4a", "./ejercicio4a", argv[1], argv[2], NULL) == -1) {
          printf("Error en execlp (1)\n");
          exit(EXIT_FAILURE);
        }
      } else if (i == 1) {
        if (execlp("./ejercicio4b", "./ejercicio4b", argv[2], argv[3], NULL) == -1) {
          printf("Error en execlp (2)\n");
          exit(EXIT_FAILURE);
        }
      } else {
        if (execlp("./ejercicio4c", "./ejercicio4c", argv[3], NULL) == -1) {
          printf("Error en execlp (3)\n");
          exit(EXIT_FAILURE);
        }
      }
    /*Padre*/
    } else {
      wait(NULL);
    }
  }

  mq_unlink(ncolaenv);
  mq_unlink(ncolarec);
  exit(EXIT_SUCCESS);
}
