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

    struct mq_attr attributes;
    char colarec[TAMNAME] = "/";
    char msj[MSGSIZE] = "a";
    unsigned int prio = 1;
    int temp, i;
    mqd_t colarecibos;

    if(argc < 2) {
        printf("El programa a requiere el nombre de la cola de lectura:\n");
        printf("Terminando la ejecucion.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(&colarec[1], argv[1]);
    printf("Entrando al C\n");
    colarecibos = mq_open(colarec, O_RDWR, S_IRUSR | S_IWUSR, NULL);

    mq_getattr(colarecibos, &attributes);
    for(i=0; i<attributes.mq_curmsgs; i++) {
      /*Todos los ejercicios petan si intentan usar una cola inexistente*/
      temp = mq_receive(colarecibos, msj, MSGSIZE, &prio);
      if( temp == -1) {
          fprintf (stderr, "Error receiving message\n");
          return EXIT_FAILURE;
      }
      printf("%s\n", msj);
    }

    return 0;
}
