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
#define MSGSIZE 20

int main(int argc, char *argv[]) {

    struct mq_attr attributes1;
    struct mq_attr attributes2;
    char colarec[TAMNAME] = "/";      /*Inicializamos las dos cadenas con un / porque*/
    char colaenv[TAMNAME] = "/";     /*siempre deben iniciarse los nombres con es caracter*/
    char msj[MSGSIZE] = "a";          /*puede que esta inicialización no sea correcta*/
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

    strcpy(msj, colarec);

    for(veces=0L; veces<attributes1.mq_curmsgs; veces++) {
      if (mq_receive(colarecibos, msj, MSGSIZE, &prio) == -1) {
        printf("Error al recibir de la cola %s\n", colarec);
        exit(EXIT_FAILURE);
      }
      for(i = 0; msj[i] != 0; i++) {
          if(msj[i] == 'z')
              msj[i] = 'a';
          else if (msj[i] >= 'a' && msj[i] <= 'z')
              msj[i] = (msj[i] + 1);
      }
      if(mq_send(colaenvios, msj, MSGSIZE, 1) == -1) {
          fprintf (stderr, "Error sending message\n");
          return EXIT_FAILURE;
      }
    }

    return 0;
}
