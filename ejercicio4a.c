/**
 * @file ejercicio4a.c
 * @author Juan Martin Pinilla (juan.martinp@estudiante.uam.es)
 * @author David Palomo Marcos (david.palomo@estudiante.uam.es)
 * Grupo: 2212
 * @date 10 Abr 2019
 */

#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define TAMNAME 20
#define MSGSIZE 2048

int main(int argc, char *argv[]) {
  int fd, i, veces;
  struct mq_attr attributes;
  char colamsjs[TAMNAME] = "/"; /*siempre deben iniciarse los nombres con es caracter*/
  char *cadena = NULL; /*Cadena en que se mapea el fichero*/
  mqd_t cola;
  struct stat estadisticas;

  if (argc < 3) {
    printf("[EJ4A] El programa a requiere el nombre del archivo y de la cola:\n");
    printf("[EJ4A] Terminando la ejecucion.\n");
    exit(EXIT_FAILURE);
  }
  strcpy(&colamsjs[1], argv[2]);

  /*Creamos la cola de mensajes*/
  cola = mq_open(colamsjs, O_RDWR, S_IRUSR | S_IWUSR, &attributes);
  if (cola == -1) {
    printf("[EJ4A] Error al abrir la cola %s\n", colamsjs);
    exit(EXIT_FAILURE);
  }
  mq_getattr(cola, &attributes);

  /*Abrir fichero*/
  fd = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    printf("[EJ4A] Error al abrir el fichero %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  /*Mapeamos fichero como cadena de caracteres (tamanio obtenido con fstat)*/
  fstat(fd, &estadisticas);
  cadena = (char *)mmap(NULL, estadisticas.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (cadena == MAP_FAILED) {
    printf("[EJ4A] Error al mapear el fichero %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  /*Imprimir cadena que contiene el texto del fichero original*/
  printf("[EJ4A] Obteniendo contenido del fichero %s:\n", argv[1]);
  printf("---------------------------------------------------------\n");
  printf("%s", cadena);
  printf("---------------------------------------------------------\n");

  /*Dividimos cadena en bloques de tamanio MSGSIZE*/
  veces = strlen(cadena) / MSGSIZE;
  if (veces > attributes.mq_maxmsg) {
    printf("\n[EJ4A] Error: mq no admite tantos mensajes\n");
    exit(EXIT_FAILURE);
  }
  /*Si necesitamos un bloque "pequenio" al final*/
  if (veces%MSGSIZE || veces==0)
    veces++;

  /*Enviamos a la cola tantos mensajes como bloques haya, dividiendo la cadena*/
  printf("[EJ4A] Enviando contenido en %d mensajes a la cola %s\n", veces, colamsjs);
  for (i = 0; i < veces; i++) {
    if (mq_send(cola, &cadena[MSGSIZE * i], MSGSIZE, 1) == -1) {
      fprintf(stderr, "[EJ4A] Error sending message\n");
      exit(EXIT_FAILURE);
    }
  }

  close(fd);
  exit(EXIT_SUCCESS);
}
