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

    struct mq_attr attributes1;
    struct mq_attr attributes2;
    char colarec[TAMNAME] = "/";      /*Inicializamos las dos cadenas con un / porque*/
    char colaenv[TAMNAME] = "/";     /*siempre deben iniciarse los nombres con es caracter*/
    char msj[MSGSIZE] = "a";          /*puede que esta inicialización no sea correcta*/
    int temp, p, i;
    int *prio;
    mqd_t colarecibos;
    mqd_t colaenvios;

    p = 1;
    prio = &p;

    attributes1.mq_flags = 0;
	attributes1.mq_maxmsg = 10;
	attributes1.mq_curmsgs = 0;
	attributes1.mq_msgsize = sizeof(msj);

    attributes2.mq_flags = 0;
    attributes2.mq_maxmsg = 10;
    attributes2.mq_curmsgs = 0;
    attributes2.mq_msgsize = sizeof(msj);

    if(argc < 3) {
        printf("El programa b requiere el nombre de la cola para recibir y enviar mensajes:\n");
        printf("Terminando la ejecucion.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(&colarec[1], argv[1]);
    strcpy(&colaenv[1], argv[2]);


    /*Asignamos las colas de lectura y escritura*/
    colarecibos = mq_open(colarec, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attributes1);
    colaenvios = mq_open(colaenv, O_RDWR | O_CREAT, S_IWUSR | S_IWUSR, &attributes2);

    strcpy(msj, colarec);


    /* TODO en este bucle peta*/
    while(temp = mq_receive(colarecibos, msj, sizeof(msj), prio)) {
        if( temp == -1) {
            fprintf (stderr, "Error receiving message\n");
            return EXIT_FAILURE;
        }

        for(i = 0; msj[i] != 0 ; i++){
            if(msj[i] == 'z')
                msj[i] = 'a';
            else
                msj[i] = (msj[i] + 1);
        }

        if( mq_send(colaenvios, msj, sizeof(msj), 1) == -1) {
            fprintf (stderr, "Error sending message\n");
            return EXIT_FAILURE;
        }
    }


    return 0;
}
