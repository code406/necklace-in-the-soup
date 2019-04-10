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
    int fd, i, veces;
    struct mq_attr attributes;
    char colamsjs[TAMNAME] = "/";     /*siempre deben iniciarse los nombres con es caracter*/
    char *cadena = NULL;
    mqd_t cola;
    struct stat estadisticas;

    if(argc < 3) {
        printf("El programa a requiere el nombre del archivo y de la cola:\n");
        printf("Terminando la ejecucion.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(&colamsjs[1], argv[2]);

    /*Creamos la cola de mensajes*/
    cola = mq_open(colamsjs, O_RDWR, S_IRUSR | S_IWUSR, &attributes);
    /*Usamos argv[1] porque aqui no queremos la / al inicio*/
    fd = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);
    fstat(fd, &estadisticas);
    cadena = (char *) mmap(NULL, estadisticas.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("CONTENIDO DEL FICHERO:\n%s",cadena);
    veces = strlen(cadena) / MSGSIZE;
    if(veces > 10){
      printf("\n[!] Error: mq no admite tantos mensajes\n");
      return EXIT_FAILURE;
    }
    if(veces%MSGSIZE) veces++;
    for(i = 0; i < veces; i++) {
      if( mq_send(cola, &cadena[MSGSIZE*i], MSGSIZE, 1) == -1) {
          fprintf (stderr, "Error sending message\n");
          return EXIT_FAILURE;
      }
    }

    close(fd);
    return 0;
}
