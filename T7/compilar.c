#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "pss.h"

// Cola global para almacenar los nombres de los archivos .c
Queue *cola_global;

void recorrer_directorio(char *nombre) {
  struct stat almacenaje;
  int rc;
  int rc_o;
  rc = stat(nombre, &almacenaje);
  if (rc != 0) {
    printf("%s no existe\n", nombre);
    exit(0);
  }
  // Es un archivo regular
  if (S_ISREG(almacenaje.st_mode)) {
    char *extension = strrchr(nombre, '.');
    // Comprobamos si es un archivo .c
    if (strcmp(extension, ".c") == 0) {
      char *archivo_o = strdup(nombre);
      strcpy(strrchr(archivo_o, '.'), ".o");
      struct stat almacenaje_o;
      rc_o = stat(archivo_o, &almacenaje_o);
      if (rc_o != 0) {
        // Agregamos el archivo a la cola.
        char *archivo_o = strdup(nombre);
        put(cola_global, archivo_o);
      }
      else {
        // Comparamos las fechas de modificación.
        if (almacenaje.st_mtime > almacenaje_o.st_mtime) {
          char *archivo_o = strdup(nombre);
          put(cola_global, archivo_o);
        }
      }
      free(archivo_o);
    }  
  }
  // Es un directorio
  else if (S_ISDIR(almacenaje.st_mode)) {
    DIR *directorio = opendir(nombre);
    if (directorio == NULL) {
      perror(nombre);
      exit(1);
    }
    for (struct dirent *entrada = readdir(directorio); entrada != NULL; entrada = readdir(directorio)) {
      if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
        continue;
      }
      char *nombre_archivo = malloc(strlen(nombre) + strlen(entrada->d_name) + 2);
      strcpy(nombre_archivo, nombre);
      strcat(nombre_archivo, "/");
      strcat(nombre_archivo, entrada->d_name);
      recorrer_directorio(nombre_archivo);
      free(nombre_archivo);
    }
    closedir(directorio); 
  }
  // Podria ser un dispositivo, un link simbolico, etc.
  else {
    fprintf(stderr, "Archivo %s es de tipo desconocido\n", nombre);
    exit(1);
  }
}

// Función de comparación para ordenar alfabéticamente los archivos.
int comparar_punteros(void *arreglo, int i, int j) {
  char **a = arreglo;
  return strcmp(a[i], a[j]);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "uso: %s <arch|dir>\n", argv[0]);
    exit(1);
  }
  cola_global = makeQueue();
  recorrer_directorio(argv[1]);
  int largo_cola = queueLength(cola_global);
  char **archivos = malloc(largo_cola * sizeof(char *));
  // Transferimos los archivos de la cola a nuestra colección.
  for (int i = 0; i < largo_cola; i++) {
    archivos[i] = get(cola_global);
  }
  // Ordenar los archivos alfabéticamente.
  sortPtrArray(archivos, 0, largo_cola - 1, comparar_punteros);
  // Mostramos los nombres y liberamos la memoria asignada para los archivos.
  for (int i = 0; i < largo_cola; i++) {
    printf("%s\n", archivos[i]);
    free(archivos[i]);
  }
  free(archivos);
  return 0;
}
