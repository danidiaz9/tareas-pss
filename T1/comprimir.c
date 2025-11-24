#include <stdlib.h>
#include <string.h>
#include "comprimir.h"

typedef unsigned int uint;
uint comprimir(uint *a, int nbits) {
  uint r = 0;
  int cnt = 0;
  uint z = -1;
  uint mask = ~(z << (nbits - 1) << 1);
  for (int i = nbits; i <= (sizeof(uint) << 3); i += nbits) {
    r = r << (nbits - 1) << 1;
    r |= (a[cnt] & mask);
    cnt++;  
  }
  return r;
}
