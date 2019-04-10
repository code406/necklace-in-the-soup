/**
 * @file ejercicio4b.c
 * @author Juan Martin Pinilla (juan.martinp@estudiante.uam.es)
 * @author David Palomo Marcos (david.palomo@estudiante.uam.es)
 * Grupo: 2212
 * @date 10 Abr 2019
 */

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMNAME 20
#define MSGSIZE 2048

int main(int argc, char *argv[]) {
  struct mq_attr attributes1;
  struct mq_attr attributes2;
  char colarec[TAMNAME] = "/";
  char colaenv[TAMNAME] = "/";
  char msj[MSGSIZE] = "a";
  int i;
  long veces;
  unsigned int prio = 1;
  mqd_t colarecibos;
  mqd_t colaenvios;

  if(argc < 3) {
      printf("El programa b requiere el nombre de la cola para recibir y enviar mensajes:\n");
      printf("Terminando la ejecucion.\n");
      exit(EXIT_FAILURE);
  }
  strcpy(&colarec[1], argv[1]);
  strcpy(&colaenv[1], argv[2]);

  /*Asignamos las colas de lectura y escritura*/
  colarecibos = mq_open(colarec, O_RDWR, S_IRUSR | S_IWUSR, &attributes1);
  if (colarecibos == -1) {
    printf("Error al abrir la cola %s\n", colarec);
    exit(EXIT_FAILURE);
  }
  colaenvios = mq_open(colaenv, O_RDWR, S_IWUSR | S_IWUSR, &attributes2);
  if (colaenvios == -1) {
    printf("Error al abrir la cola %s\n", colaenv);
    exit(EXIT_FAILURE);
  }
  mq_getattr(colarecibos, &attributes1);
  mq_getattr(colaenvios, &attributes2);

  /*Tantas veces como mensajes haya en cola (mq_curmsgs)*/
  for(veces=0L; veces<attributes1.mq_curmsgs; veces++) {
	/*Recibir un mensaje*/
    if (mq_receive(colarecibos, msj, MSGSIZE, &prio) == -1) {
      printf("Error al recibir de la cola %s\n", colarec);
      exit(EXIT_FAILURE);
    }
	/* Reemplazar caracteres en el rango a-z por su siguiente en el abecedario*/
    for(i = 0; msj[i] != 0; i++) {
        if(msj[i] == 'z')
            msj[i] = 'a';
        else if (msj[i] >= 'a' && msj[i] <= 'z')
            msj[i] = (msj[i] + 1);
    }
	/*Enviar la cadena modificada a otra cola*/
    if(mq_send(colaenvios, msj, MSGSIZE, 1) == -1) {
        fprintf (stderr, "Error sending message\n");
        exit(EXIT_FAILURE);
    }
  }

  exit(EXIT_SUCCESS);
}
