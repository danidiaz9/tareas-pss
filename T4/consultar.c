#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "pss.h"

#define MAXTAM (800+2)

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Uso: ./consultar.bin <diccionario> <llave>\n");
    exit(1);
  }
  
  char* archivo = argv[1];
  char* llave = argv[2];
  int tamanho_llave = strlen(llave);

  //Definimos un buffer para leer la primera linea
  char buffer_llave[tamanho_llave + 1];
  char buffer[MAXTAM];
  
  FILE *archivo_dicc = fopen(archivo, "r");
  if (archivo_dicc == NULL) {
    perror(archivo);
    exit(1);
  }
  
  //Leemos la primera linea y calculamos el tamaño del archivo
  fgets(buffer, sizeof(buffer),archivo_dicc);
  fseek(archivo_dicc, 0, SEEK_END);
  int tamanho_archivo = ftell(archivo_dicc);
  int largo_linea = strlen(buffer);
  int numero_lineas = tamanho_archivo / largo_linea;
  if (largo_linea == 0) {
    fprintf(stderr, "El archivo está vacio\n");
    exit(1);
  }
  if (tamanho_archivo == 0) {
    fprintf(stderr, "%s: el dicionario esta vacio\n", archivo);
    exit(1);
  }
  if (tamanho_archivo % largo_linea != 0) {
    fprintf(stderr, "%s: el tamaño del archivo no es multiplo del tamaño de la linea\n", archivo);
    exit(1);
  }
  
  //Buscamos la llave en la tabla de hash
  int n = hash_string(llave) % numero_lineas;
  int llave_encontrada = 0;
  char buffer_definicion[largo_linea - tamanho_llave - 2];
  fseek(archivo_dicc, n * largo_linea, SEEK_SET);
  for (int contador_lineas = 0; contador_lineas < numero_lineas; contador_lineas++) {
    fseek(archivo_dicc, n * largo_linea, SEEK_SET);
    if (fgets(buffer, sizeof(buffer),archivo_dicc) == NULL) {
      fprintf(stderr, "El archivo está vacio\n");
      exit(1);
    }
    int largo_linea_actual = strlen(buffer);
    if (largo_linea_actual != largo_linea) {
      fprintf(stderr,"%s: linea %i de tamaño incorrecto\n", archivo, n);
      exit(1);
    }
    
    if (buffer[0] == '\0') {
      fprintf(stderr, "Linea vacia\n");
      exit(1);
    }
    strncpy(buffer_llave, buffer, tamanho_llave);
      buffer_llave[tamanho_llave] = '\0';
      if (strcmp(buffer_llave, llave) == 0) {
        if (buffer[tamanho_llave] == ':') {
          strncpy(buffer_definicion, buffer + tamanho_llave + 1, largo_linea - tamanho_llave - 3);
          buffer_definicion[largo_linea - tamanho_llave - 3] = '\0';
          llave_encontrada = 1;
          printf("%s\n", buffer_definicion);
          exit(0);
        }
        else {
          fprintf(stderr, "%s: linea %d no posee : para terminar la llave\n", archivo, n);
          exit(1);
        }
      }
    n = (n + 1) % numero_lineas;
  }
  if (llave_encontrada == 0) {
    fprintf(stderr, "%s: el diccionario no contiene la llave %s\n", archivo, llave);
    exit(1);
  }
  fclose(archivo_dicc);
  exit(0);
}