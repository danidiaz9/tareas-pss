#include <stdio.h>
#include <stdlib.h>
#include "elim-rango.h"

void eliminarRango(Nodo **phead, double y, double z) {
  Nodo *actual = *phead;
  Nodo *anterior = NULL;
  Nodo *auxiliar;

  while (actual != NULL) {
    if (actual->x >= y && actual->x <= z) {
      auxiliar = actual;
      actual = actual->prox;
      if (anterior != NULL) {
        anterior->prox = actual;
      }
      else {
        *phead = actual;
      }
      free(auxiliar);
    }
    else if (actual->x > z) {
      break;
    }
    else {
      anterior = actual;
      actual = actual->prox;
    }
  }
}