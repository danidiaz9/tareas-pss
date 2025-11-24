#include <string.h>

int strCmp(char *s1, char *s2) {
  while (*s1 != '\0' || *s2 != '\0') {
    char c1 = *s1++;
    char c2 = *s2++;
    if (c1 != c2) {
      if (c1 >= 'a' && c1 <= 'z')
        c1 = (c1 - 'a' + 13) % 26 + 'a';
      if (c2 >= 'a' && c2 <= 'z')
        c2 = (c2 - 'a' + 13) % 26 + 'a';
      return c1 - c2;
    }
  }
  return 0;
}

void sort(char **a, int n) {
  char **ult= &a[n-1];
  char **p= a;
  while (p<ult) {
    // No modifique nada arriba de esta linea
    // Inicio de la parte que debe cambiar

    int t1= strCmp(p[0], p[1]);

    // Fin de la parte que debe cambiar
    // No Cambie nada mas a partir de aca
    if (t1 <= 0)
      p++;
    else {
      char *tmp= p[0];
      p[0]= p[1];
      p[1]= tmp;
      p= a;
    }
  }
}
