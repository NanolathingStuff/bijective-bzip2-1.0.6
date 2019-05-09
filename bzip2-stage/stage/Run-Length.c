//Run length encoding iniziale
#include "run-length.h"

void init_RL ( EState* s )
{
   s->state_in_ch  = 256;
   s->state_in_len = 0;
}
int isempty_RL ( EState* s )
{
   if (s->state_in_ch < 256 && s->state_in_len > 0)
      return 0; 
  else
      return 1;
}
void add_pair_to_block ( EState* s ){
   int i;
   unsigned char ch = (unsigned char)(s->state_in_ch);
   for (i = 0; i < s->state_in_len; i++) {
      //BZ_UPDATE_CRC( s->blockCRC, ch );
   		s->blockCRC = (s->blockCRC << 8) ^  BZ2_crc32Table[(s->blockCRC >> 24) ^  ((unsigned char)ch)]; 
   }
   s->inUse[s->state_in_ch] = 1;
   switch (s->state_in_len) {
      case 1:
         s->block[s->nblock] = (unsigned char)ch; s->nblock++;
         break;
      case 2:
         s->block[s->nblock] = (unsigned char)ch; s->nblock++;
         s->block[s->nblock] = (unsigned char)ch; s->nblock++;
         break;
      case 3:
         s->block[s->nblock] = (unsigned char)ch; s->nblock++;
         s->block[s->nblock] = (unsigned char)ch; s->nblock++;
         s->block[s->nblock] = (unsigned char)ch; s->nblock++;
         break;
      default:
         s->inUse[s->state_in_len-4] = 1;
         s->block[s->nblock] = (unsigned char)ch; s->nblock++;
         s->block[s->nblock] = (unsigned char)ch; s->nblock++;
         s->block[s->nblock] = (unsigned char)ch; s->nblock++;
         s->block[s->nblock] = (unsigned char)ch; s->nblock++;
         s->block[s->nblock] = ((unsigned char)(s->state_in_len-4));
         s->nblock++;
         break;
   }
}
void flush_RL ( EState* s ){
   if (s->state_in_ch < 256) add_pair_to_block ( s );
   init_RL ( s );
}
/////////////////////////DA RIDEFINIRE
/*#define ADD_CHAR_TO_BLOCK(zs,zchh0)               {                                                 
   UInt32 zchh = (UInt32)(zchh0);   
                 \
   //-- fast track the common case --           \

   if (zchh != zs->state_in_ch &&                 \
       zs->state_in_len == 1) {                   \
      UChar ch = (UChar)(zs->state_in_ch);        \
      BZ_UPDATE_CRC( zs->blockCRC, ch );          \
      zs->inUse[zs->state_in_ch] = True;          \
      zs->block[zs->nblock] = (UChar)ch;          \
      zs->nblock++;                               \
      zs->state_in_ch = zchh;                     \
   }                                              \
   else                                           \
   //-- general, uncommon cases --              \
   if (zchh != zs->state_in_ch ||                 \
      zs->state_in_len == 255) {                  \
      if (zs->state_in_ch < 256)                  \
         add_pair_to_block ( zs );                \
      zs->state_in_ch = zchh;                     \
      zs->state_in_len = 1;                       \
   } else {                                       \
      zs->state_in_len++;                         \
   }                                              \
}
*/