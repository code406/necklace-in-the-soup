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
    printf("[EJ4C] El programa a requiere el nombre de la cola de lectura:\n");
    printf("[EJ4C] Terminando la ejecucion.\n");
    exit(EXIT_FAILURE);
  }
  strcpy(&colarec[1], argv[1]);

  /*Abrir la cola en la que vienen los mensajes*/
  colarecibos = mq_open(colarec, O_RDWR, S_IRUSR | S_IWUSR, NULL);
  if (colarecibos == -1) {
    printf("[EJ4C] Error al abrir la cola %s\n", colarec);
    exit(EXIT_FAILURE);
  }
  mq_getattr(colarecibos, &attributes);

  /*Para tantos mensajes como haya en la cola (mq_curmsgs), imprimirlos*/
  printf("[EJ4C] Leyendo %ld mensajes de la cola %s:\n", attributes.mq_curmsgs, colarec);
  for (i = 0; i < attributes.mq_curmsgs; i++) {
    temp = mq_receive(colarecibos, msj, MSGSIZE, &prio);
    if (temp == -1) {
      fprintf(stderr, "[EJ4C] Error receiving message\n");
      exit(EXIT_FAILURE);
    }
    msj[MSGSIZE] = '\0';
    printf("[EJ4C] Imprimiendo el mensaje numero %d\n", i+1);
    printf("---------------------------------------------------------\n");
    printf("%s", msj);
    if(i != (attributes.mq_curmsgs - 1)) printf("\n");
    printf("---------------------------------------------------------\n");
  }

  exit(EXIT_SUCCESS);
}
