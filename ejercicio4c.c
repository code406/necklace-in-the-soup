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
#define MSGSIZE 2000


int main(int argc, char *argv[]) {

    struct mq_attr attributes;
    char colarec[TAMNAME] = "/";
    char msj[MSGSIZE] = "a";
    int *prio;
    int prioridad = 1;
    int temp;
    mqd_t colarecibos;

    attributes.mq_flags = 0;
	attributes.mq_maxmsg = 10;
	attributes.mq_curmsgs = 0;
	attributes.mq_msgsize = sizeof(msj);

    if(argc < 2) {
        printf("El programa a requiere el nombre de la cola de lectura:\n");
        printf("Terminando la ejecucion.\n");
        exit(EXIT_FAILURE);
    }

    prio = &prioridad;

    strcpy(&colarec[1], argv[1]);

    colarecibos = mq_open(colarec, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attributes);

    /*Todos los ejercicios petan si intentan usar una cola inexistente*/
    temp = mq_receive(colarecibos, msj, sizeof(msj), prio);
    if( temp == -1) {
        fprintf (stderr, "Error receiving message\n");
        return EXIT_FAILURE;
    }

    printf("%s", msj);

    return 0;
}
