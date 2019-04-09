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
#define NHIJOS 3


int main(int argc, char *argv[]) {

    struct mq_attr attributes1;
    struct mq_attr attributes2;
    char ncolaenv[MSGSIZE];
    char ncolarec[MSGSIZE];
    pid_t ppid;
    mqd_t colaenviar;
    mqd_t colarecibir;
    in i;

    attributes1.mq_flags = 0;
    attributes1.mq_maxmsg = 10;
    attributes1.mq_curmsgs = 0;
    attributes1.mq_msgsize = sizeof(msj);

    attributes2.mq_flags = 0;
    attributes2.mq_maxmsg = 10;
    attributes2.mq_curmsgs = 0;
    attributes2.mq_msgsize = sizeof(msj);

    if(argc < 4) {
        printf("El programa requiere el nombre del fichero y las dos colas:\n");
        printf("Terminando la ejecucion.\n");
        exit(EXIT_FAILURE);
    }

    /*Copiamos los argumentos con / delante*/
    strcpy(&cola[1], argv[2]);
    strcpy(&colamsjs[1], argv[3]);

    ppid = getpid();
    for(i = 0; i < NHIJOS ; i++){

    }

    return 0;
}
