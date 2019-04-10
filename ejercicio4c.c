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
    int temp;
    mqd_t colarecibos;

    attributes.mq_flags = 0;
	attributes.mq_maxmsg = 10;
	attributes.mq_curmsgs = 0;
attributes.mq_msgsize = MSGSIZE;

    if(argc < 2) {
        printf("El programa a requiere el nombre de la cola de lectura:\n");
        printf("Terminando la ejecucion.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(&colarec[1], argv[1]);
  printf("Entrando al C\n");
    colarecibos = mq_open(colarec, O_RDWR, S_IRUSR | S_IWUSR, &attributes);

    /*Todos los ejercicios petan si intentan usar una cola inexistente*/
    temp = mq_receive(colarecibos, msj, MSGSIZE, &prio);
    if( temp == -1) {
        fprintf (stderr, "Error receiving message\n");
        return EXIT_FAILURE;
    }
    msj[strlen(msj)-2] = ' ';
    printf("%s\n", msj);






    /*Todos los ejercicios petan si intentan usar una cola inexistente*/
    temp = mq_receive(colarecibos, msj, MSGSIZE, &prio);
    if( temp == -1) {
        fprintf (stderr, "Error receiving message\n");
        return EXIT_FAILURE;
    }
    msj[strlen(msj)-2] = ' ';
    printf("%s\n", msj);

    return 0;
}
