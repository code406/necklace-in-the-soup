/**
 * @file ejercicio4c.c
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
#define MSGSIZE 2048

int main(int argc, char *argv[]) {
  struct mq_attr attributes;
  char colarec[TAMNAME] = "/";
  char msj[MSGSIZE] = "a";
  unsigned int prio = 1;
  int temp, i;
  mqd_t colarecibos;

  if (argc < 2) {
    printf("El programa a requiere el nombre de la cola de lectura:\n");
    printf("Terminando la ejecucion.\n");
    exit(EXIT_FAILURE);
  }
  strcpy(&colarec[1], argv[1]);

  /*Abrir la cola en la que vienen los mensajes*/
  colarecibos = mq_open(colarec, O_RDWR, S_IRUSR | S_IWUSR, NULL);
  if (colarecibos == -1) {
    printf("Error al abrir la cola %s\n", colarec);
    exit(EXIT_FAILURE);
  }
  mq_getattr(colarecibos, &attributes);

  /*Para tantos mensajes como haya en la cola (mq_curmsgs), imprimirlos*/
  printf("\nRESULTADO MODIFICADO Y EN BLOQUES:\n");
  for (i = 0; i < attributes.mq_curmsgs; i++) {
    printf("#### Bloque #%d\n", i);
    temp = mq_receive(colarecibos, msj, MSGSIZE, &prio);
    if (temp == -1) {
      fprintf(stderr, "Error receiving message\n");
      exit(EXIT_FAILURE);
    }
    msj[MSGSIZE] = '\0';
    printf("%s\n", msj);
  }

  exit(EXIT_SUCCESS);
}
