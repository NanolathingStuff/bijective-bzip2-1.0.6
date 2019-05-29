/*#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>*/
#include "private.h"

#define BZ_MAX_ALPHA_SIZE 258
#define BZ_N_GROUPS 6



//define funct: copia incolla il testo della funzione dove è stata chiamata(così com'è)

static inline void bsW (EState* s, int n, UInt32 v )
{
    while (s->bsLive >= 8) {                   \
      s->zbits[s->numZ] = (unsigned char)(s->bsBuff >> 24);          \
      s->numZ++;                              \
      s->bsBuff <<= 8;                        \
      s->bsLive -= 8;    
	}
   s->bsBuff |= (v << (32 - s->bsLive - n));
   s->bsLive += n;
}
