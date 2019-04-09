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
#define MSGSIZE 20      /*¿¿¿¿Tiene que ser de 2KB????*/


int main(int argc, char *argv[]){

    FILE *flectura = NULL;
    struct mq_attr attributes;
    char fichero[TAMNAME] = "/";     /*Inicializamos las dos cadenas con un / porque*/
    char colamsjs[TAMNAME] = "/";     /*siempre deben iniciarse los nombres con es caracter*/
    char msj[MSGSIZE] = "a";
    mqd_t cola;

    attributes.mq_flags = 0;
	attributes.mq_maxmsg = 10;
	attributes.mq_curmsgs = 0;
	attributes.mq_msgsize = sizeof(msj);

    if(argc < 3) {
        printf("El programa requiere el nombre del archivo y de la cola:\n");
        printf("Terminando la ejecucion.\n");
        exit(EXIT_FAILURE);
    }

    /*Copiamos los argumentos con / delante*/
    strcpy(&fichero[1], argv[1]);
    strcpy(&colamsjs[1], argv[2]);

    /*Creamos la cola de mensajes solo de escritura (para enviar)*/
    cola = mq_open(colamsjs, O_WRONLY | O_CREAT, S_IWUSR, &attributes);

    /*Usamos argv[1] porque aqui no queremos la / al inicio*/
    flectura = fopen(argv[1], "r");

    while(fscanf(flectura, "%s", msj) != EOF) {
        if( mq_send(cola, msj, sizeof(msj), 1) == -1) {
    		fprintf (stderr, "Error sending message\n");
    		return EXIT_FAILURE;
    	}
    }

    mq_close(cola);
    mq_unlink(colamsjs);
    fclose(flectura);

    return 0;
}
