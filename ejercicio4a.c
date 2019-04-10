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


int main(int argc, char *argv[]){
    int fd, i;
    struct mq_attr attributes;
    char fichero[TAMNAME];     /*Inicializamos las dos cadenas con un / porque*/
    char colamsjs[TAMNAME] = "/";     /*siempre deben iniciarse los nombres con es caracter*/
    char msj[MSGSIZE] = "a";
    char *cadena = NULL;
    mqd_t cola;
    struct stat estadisticas;

    attributes.mq_flags = 0;
    attributes.mq_maxmsg = 10;
    attributes.mq_curmsgs = 0;
attributes.mq_msgsize = MSGSIZE;

    if(argc < 3) {
        printf("El programa a requiere el nombre del archivo y de la cola:\n");
        printf("Terminando la ejecucion.\n");
        exit(EXIT_FAILURE);
    }

    printf("Entrando al A\n");
    strcpy(&colamsjs[1], argv[2]);

    /*Creamos la cola de mensajes*/
    cola = mq_open(colamsjs, O_RDWR, S_IRUSR | S_IWUSR, &attributes);
    printf("Cola abierta en A\n");
    /*Usamos argv[1] porque aqui no queremos la / al inicio*/
    fd = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);
    fstat(fd, &estadisticas);
    cadena = (char *) mmap(NULL, estadisticas.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("Cadena :%s\n",cadena );
    if( mq_send(cola, cadena, MSGSIZE, 1) == -1) {
        fprintf (stderr, "Error sending message\n");
        return EXIT_FAILURE;
    }
    if( mq_send(cola, &cadena[MSGSIZE], MSGSIZE, 1) == -1) {
        fprintf (stderr, "Error sending message\n");
        return EXIT_FAILURE;
    }
    /*for(i=0; i<strlen(cadena) && cadena[i] != EOF; i++) {
        if( mq_send(cola, &cadena[i], sizeof(char), 1) == -1) {
    		    fprintf (stderr, "Error sending message\n");
    		    return EXIT_FAILURE;
        }
    }*/

    mq_close(cola);
    //mq_unlink(colamsjs);
    close(fd);

    return 0;
}
