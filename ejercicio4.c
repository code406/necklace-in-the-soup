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
#define NHIJOS 3

int main(int argc, char *argv[]) {

    struct mq_attr attributes1;
    struct mq_attr attributes2;
    char ncolaenv[MSGSIZE] = "/";
    char ncolarec[MSGSIZE] = "/";
    pid_t pid;
    mqd_t colaenviar;
    mqd_t colarecibir;
    int i;

    attributes1.mq_flags = 0;
    attributes1.mq_maxmsg = 10;
    attributes1.mq_curmsgs = 0;
attributes1.mq_msgsize = sizeof(char) * 20;

    attributes2.mq_flags = 0;
    attributes2.mq_maxmsg = 10;
    attributes2.mq_curmsgs = 0;
attributes2.mq_msgsize = sizeof(char) * 20;

    if(argc < 4) {
        printf("El programa requiere el nombre del fichero y las dos colas:\n");
        printf("Terminando la ejecucion.\n");
        exit(EXIT_FAILURE);
    }

    /*Copiamos los argumentos con / delante*/
    strcpy(&ncolaenv[1], argv[2]);
    strcpy(&ncolarec[1], argv[3]);

    colaenviar = mq_open(ncolaenv, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attributes1);
    colarecibir = mq_open(ncolarec, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attributes2);

    for (i = 0; i < NHIJOS; i++) {
      if ((pid = fork()) < 0) {
        printf("ERROR en fork.\n");
        exit(EXIT_FAILURE);
      }
      /*Hijo*/
      else if (pid == 0) {
        if(i==0) {
          if(execlp("./ejercicio4A", "./ejercicio4A", argv[1], argv[2], NULL) == -1){
              printf("Error en execlp (1)\n");
              exit(EXIT_FAILURE);
          }
        } else if(i==1) {
          if(execlp("./ejercicio4B", "./ejercicio4B", argv[2], argv[3], NULL) == -1){
            printf("Error en execlp (2)\n");
            exit(EXIT_FAILURE);
          }
        } else {
          if(execlp("./ejercicio4C", "./ejercicio4C", argv[3], NULL) == -1) {
            printf("Error en execlp (3)\n");
            exit(EXIT_FAILURE);
          }
        }
      }
    }
    for (i = 0; i < NHIJOS; i++)
      wait(NULL);

    printf("Programa principal terminando\n");
    // CERRAR COLAS??
    mq_unlink(ncolaenv);
    mq_unlink(ncolarec);

    return 0;
}
