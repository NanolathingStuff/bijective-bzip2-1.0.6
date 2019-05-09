#ifndef PRIVATE_H
#define PRIVATE_H

#include <stdlib.h>

#ifndef BZ_NO_STDIO
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#endif
#define BZ_VERSION  "1.0.6, 6-Sept-2010"

typedef char            Char;
typedef unsigned char   Bool;
typedef unsigned char   UChar;
typedef int             Int32;
typedef unsigned int    UInt32;
typedef short           Int16;
typedef unsigned short  UInt16;

#define True  ((Bool)1)
#define False ((Bool)0)

#ifndef __GNUC__
#define __inline__  /* */
#endif 
#define VPrintf0(zf) \
   fprintf(stderr,zf)
#define VPrintf1(zf,za1) \
   fprintf(stderr,zf,za1)
#define VPrintf2(zf,za1,za2) \
   fprintf(stderr,zf,za1,za2)
#define VPrintf3(zf,za1,za2,za3) \
   fprintf(stderr,zf,za1,za2,za3)
#define VPrintf4(zf,za1,za2,za3,za4) \
   fprintf(stderr,zf,za1,za2,za3,za4)
#define VPrintf5(zf,za1,za2,za3,za4,za5) \
   fprintf(stderr,zf,za1,za2,za3,za4,za5)

#define BZALLOC(nnn) (strm->bzalloc)(strm->opaque,(nnn),1)
#define BZFREE(ppp)  (strm->bzfree)(strm->opaque,(ppp))


/*-- Header bytes. --*/

#define BZ_HDR_B 0x42   /* 'B' */
#define BZ_HDR_Z 0x5a   /* 'Z' */
#define BZ_HDR_h 0x68   /* 'h' */
#define BZ_HDR_0 0x30   /* '0' */
  
/*-- Constants for the back end. --*/

#define BZ_MAX_ALPHA_SIZE 258
#define BZ_MAX_CODE_LEN    23

#define BZ_RUNA 0
#define BZ_RUNB 1

#define BZ_N_GROUPS 6
#define BZ_G_SIZE   50
#define BZ_N_ITERS  4

#define BZ_MAX_SELECTORS (2 + (900000 / BZ_G_SIZE))



/*-- Stuff for randomising repetitive blocks. --*/

extern Int32 BZ2_rNums[512];

#define BZ_RAND_DECLS                          \
   Int32 rNToGo;                               \
   Int32 rTPos                                 \

#define BZ_RAND_INIT_MASK                      \
   s->rNToGo = 0;                              \
   s->rTPos  = 0                               \

#define BZ_RAND_MASK ((s->rNToGo == 1) ? 1 : 0)

#define BZ_RAND_UPD_MASK                       \
   if (s->rNToGo == 0) {                       \
      s->rNToGo = BZ2_rNums[s->rTPos];         \
      s->rTPos++;                              \
      if (s->rTPos == 512) s->rTPos = 0;       \
   }                                           \
   s->rNToGo--;



/*-- Stuff for doing CRCs. --*/

extern UInt32 BZ2_crc32Table[256];

#define BZ_INITIALISE_CRC(crcVar)              \
{                                              \
   crcVar = 0xffffffffL;                       \
}

#define BZ_FINALISE_CRC(crcVar)                \
{                                              \
   crcVar = ~(crcVar);                         \
}

#define BZ_UPDATE_CRC(crcVar,cha)              \
{                                              \
   crcVar = (crcVar << 8) ^                    \
            BZ2_crc32Table[(crcVar >> 24) ^    \
                           ((UChar)cha)];      \
}

typedef  struct {
      /* aliases for arr1 and arr2 */
      UInt32*  ptr;
      UChar*   block;
      UChar*   zbits;

      /* input and output limits and current posns */
      int    nblock;
      int    nblockMAX;
      int    numZ;
      int    state_out_pos;

      /* map of bytes used in block */
      int    nInUse;
      UChar    inUse[256];
      UChar    unseqToSeq[256];

      /* the buffer for bit stream creation */
      UInt32   bsBuff;
      int    bsLive;

      /* block and combined CRCs */
      UInt32   blockCRC;
      UInt32   combinedCRC;

      /* stuff for coding the MTF values */
      int    nMTF;
      int    mtfFreq    [258];
      UChar    selector   [258];
      UChar    selectorMtf[258];

      UChar    len     [BZ_N_GROUPS][BZ_MAX_ALPHA_SIZE];
      int    code    [BZ_N_GROUPS][BZ_MAX_ALPHA_SIZE];
      int    rfreq   [BZ_N_GROUPS][BZ_MAX_ALPHA_SIZE];
      /* second dimension: only 3 needed; 4 makes index calculations faster */
      UInt32   len_pack[BZ_MAX_ALPHA_SIZE][4];

   }EState;
   
   
#endif
