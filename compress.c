
/*-------------------------------------------------------------*/
/*--- Compression machinery (not incl block sorting)        ---*/
/*---                                            compress.c ---*/
/*-------------------------------------------------------------*/

/* ------------------------------------------------------------------
   This file is part of bzip2/libbzip2, a program and library for
   lossless, block-sorting data compression.

   bzip2/libbzip2 version 1.0.6 of 6 September 2010
   Copyright (C) 1996-2010 Julian Seward <jseward@bzip.org>

   Please read the WARNING, DISCLAIMER and PATENTS sections in the 
   README file.

   This program is released under the terms of the license contained
   in the file LICENSE.
   ------------------------------------------------------------------ */


/* CHANGES
    0.9.0    -- original version.
    0.9.0a/b -- no changes in this file.
    0.9.0c   -- changed setting of nGroups in sendMTFValues() 
                so as to do a bit better on small files
*/

#include "bzlib_private.h"


/*---------------------------------------------------*/
/*--- Bit stream I/O                              ---*/
/*---------------------------------------------------*/

/*---------------------------------------------------*/
void BZ2_bsInitWrite ( EState* s )
{
   s->bsLive = 0;	//set the buffers for bit stream 
   s->bsBuff = 0;	//creation to 0
}


/*---------------------------------------------------*/
static
void bsFinishWrite ( EState* s )
{
   while (s->bsLive > 0) {	//zbits = array, numZ = currentPos
      s->zbits[s->numZ] = (UChar)(s->bsBuff >> 24);
      s->numZ++;
      s->bsBuff <<= 8;	//buffer left shifted
      s->bsLive -= 8;
   }
}


/*---------------------------------------------------*/
#define bsNEEDW(nz)                           \
{                                             \
   while (s->bsLive >= 8) {                   \
      s->zbits[s->numZ]                       \
         = (UChar)(s->bsBuff >> 24);          \
      s->numZ++;                              \
      s->bsBuff <<= 8;                        \
      s->bsLive -= 8;                         \
   }                                          \
}
//numZ:input and output limits and current posns    //zbits: alias arr1    
//insert right-shifted(24) bsbuff in zbits every 8 bsLive-value
/*---------------------------------------------------*/
//bsBuff, bsLive :variables for stream creation
static
__inline__
void bsW ( EState* s, Int32 n, UInt32 v )	
{	//@insert in s->zbits, compacter((Uchar)s->bsBuff),	increase buff size
   bsNEEDW ( n );	//after bsneedw transform put result in OR with
   s->bsBuff |= (v << (32 - s->bsLive - n));	//<-this
   s->bsLive += n;
}//metti tante printf per vedere che fa
//@ used in: bsPutUInt32[84], bsPutUChar[95], sendMTFValues ( EState* s )[240],BZ2_compressBlock ( EState* s, Bool is_last_block )[603]

/*---------------------------------------------------*/
static
void bsPutUInt32 ( EState* s, UInt32 u )
{
   bsW ( s, 8, (u >> 24) & 0xffL );
   bsW ( s, 8, (u >> 16) & 0xffL );
   bsW ( s, 8, (u >>  8) & 0xffL );
   bsW ( s, 8,  u        & 0xffL );
}


/*---------------------------------------------------*/
static
void bsPutUChar ( EState* s, UChar c )
{
   bsW( s, 8, (UInt32)c );
}


/*---------------------------------------------------*/
/*--- The back end proper                         ---*/
/*---------------------------------------------------*/

/*---------------------------------------------------*/
static
void makeMaps_e ( EState* s )
{
   Int32 i;
   s->nInUse = 0;
   for (i = 0; i < 256; i++)
      if (s->inUse[i]) {
         s->unseqToSeq[i] = s->nInUse;	//sign in unseqToSeq the blcks in use
         s->nInUse++;				//and update the counter
      }
}


/*---------------------------------------------------*/
static
void generateMTFValues ( EState* s )
{
   UChar   yy[256];
   Int32   i, j;
   Int32   zPend;
   Int32   wr;
   Int32   EOB;
//@// update s->mtfv €[0,1], create variables in cache if (Uchar)0 == s->unseqToSeq[block[j]], j =???
   /* 
      After sorting (eg, here),
         s->arr1 [ 0 .. s->nblock-1 ] holds sorted order,
         and
         ((UChar*)s->arr2) [ 0 .. s->nblock-1 ] 
         holds the original block data.

      The first thing to do is generate the MTF values,
      and put them in
         ((UInt16*)s->arr1) [ 0 .. s->nblock-1 ].
      Because there are strictly fewer or equal MTF values
      than block values, ptr values in this area are overwritten
      with MTF values only when they are no longer needed.

      The final compressed bitstream is generated into the
      area starting at
         (UChar*) (&((UChar*)s->arr2)[s->nblock])

      These storage aliases are set up in bzCompressInit(),
      except for the last one, which is arranged in 
      compressBlock().
   */
   UInt32* ptr   = s->ptr;	//array1
   UChar* block  = s->block;	//block input
   UInt16* mtfv  = s->mtfv;

   makeMaps_e ( s );	//maps used blocks
   EOB = s->nInUse+1;	//END Of Cicle

   for (i = 0; i <= EOB; i++) s->mtfFreq[i] = 0;

   wr = 0;
   zPend = 0;
   for (i = 0; i < s->nInUse; i++) yy[i] = (UChar) i;

   for (i = 0; i < s->nblock; i++) {
      UChar ll_i;
      AssertD ( wr <= i, "generateMTFValues(1)" );
      j = ptr[i]-1; if (j < 0) j += s->nblock;
      ll_i = s->unseqToSeq[block[j]];
      AssertD ( ll_i < s->nInUse, "generateMTFValues(2a)" );

      if (yy[0] == ll_i) { 
         zPend++;
      } else {	
		//else update s->mtfv & register(put in CPUcache) some (UChar)vars:  {rtmp, * ryy_j, rll_i}
         if (zPend > 0) {
            zPend--;
            while (True) {	//TRUE: (zPend >= 2)
               if (zPend & 1) {	//if most right bit of zPend == 1
                  mtfv[wr] = BZ_RUNB; wr++; // BZ_RUNB = 1
                  s->mtfFreq[BZ_RUNB]++; 
               } else {
                  mtfv[wr] = BZ_RUNA; wr++; // BZ_RUNA = 0
                  s->mtfFreq[BZ_RUNA]++; 
               }
               if (zPend < 2) break;
               zPend = (zPend - 2) / 2;
            };	//end while
            zPend = 0;
         }
         {
            register UChar  rtmp;	//put in CPUcache
            register UChar* ryy_j;
            register UChar  rll_i;
            rtmp  = yy[1];
            yy[1] = yy[0];
            ryy_j = &(yy[1]);
            rll_i = ll_i;
            while ( rll_i != rtmp ) {
               register UChar rtmp2;
               ryy_j++;
               rtmp2  = rtmp;
               rtmp   = *ryy_j;
               *ryy_j = rtmp2;
            };
            yy[0] = rtmp;
            j = ryy_j - &(yy[0]);
            mtfv[wr] = j+1; wr++; s->mtfFreq[j+1]++;
         }

      }
   }
	// update s->mtfv 
   if (zPend > 0) {
      zPend--;
      while (True) {
         if (zPend & 1) {
            mtfv[wr] = BZ_RUNB; wr++; 
            s->mtfFreq[BZ_RUNB]++; 
         } else {
            mtfv[wr] = BZ_RUNA; wr++; 
            s->mtfFreq[BZ_RUNA]++; 
         }
         if (zPend < 2) break;
         zPend = (zPend - 2) / 2;
      };
      zPend = 0;
   }

   mtfv[wr] = EOB; wr++; s->mtfFreq[EOB]++;

   s->nMTF = wr;
}


/*---------------------------------------------------*/
#define BZ_LESSER_ICOST  0
#define BZ_GREATER_ICOST 15

static
void sendMTFValues ( EState* s )
{
   Int32 v, t, i, j, gs, ge, totc, bt, bc, iter;
   Int32 nSelectors, alphaSize, minLen, maxLen, selCtr;
   Int32 nGroups, nBytes;

   /*--
   UChar  len [BZ_N_GROUPS][BZ_MAX_ALPHA_SIZE];
   is a global since the decoder also needs it.

   Int32  code[BZ_N_GROUPS][BZ_MAX_ALPHA_SIZE];
   Int32  rfreq[BZ_N_GROUPS][BZ_MAX_ALPHA_SIZE];
   are also globals only used in this proc.
   Made global to keep stack frame size small.
   --*/
	//[6][258]

   UInt16 cost[BZ_N_GROUPS];	//[6]
   Int32  fave[BZ_N_GROUPS];

   UInt16* mtfv = s->mtfv;

   if (s->verbosity >= 3)
      VPrintf3( "      %d in block, %d after MTF & 1-2 coding, "
                "%d+2 syms in use\n", 
                s->nblock, s->nMTF, s->nInUse );

   alphaSize = s->nInUse+2;
   for (t = 0; t < BZ_N_GROUPS; t++)
      for (v = 0; v < alphaSize; v++)
         s->len[t][v] = BZ_GREATER_ICOST;	//s->len[0..6][0..inUse+2] = 15

   /*--- Decide how many coding tables to use ---*/
   AssertH ( s->nMTF > 0, 3001 );
   if (s->nMTF < 200)  nGroups = 2; else
   if (s->nMTF < 600)  nGroups = 3; else
   if (s->nMTF < 1200) nGroups = 4; else
   if (s->nMTF < 2400) nGroups = 5; else
                       nGroups = 6;

   /*--- Generate an initial set of coding tables ---*/
   { 
      Int32 nPart, remF, tFreq, aFreq;

      nPart = nGroups;	//n coding tables
      remF  = s->nMTF;	//n MTFv
      gs = 0;
      while (nPart > 0) {
         tFreq = remF / nPart;	//MTF per coding table
         ge = gs-1;
         aFreq = 0;
         while (aFreq < tFreq && ge < alphaSize-1) {
            ge++;
            aFreq += s->mtfFreq[ge];	//number mtfValues of value ge
         }
		//how can this if become true?
         if (ge > gs 
             && nPart != nGroups && nPart != 1 
             && ((nGroups-nPart) % 2 == 1)) {
            aFreq -= s->mtfFreq[ge];
            ge--;
         }

         if (s->verbosity >= 3)
            VPrintf5( "      initial group %d, [%d .. %d], "
                      "has %d syms (%4.1f%%)\n",
                      nPart, gs, ge, aFreq, 
                      (100.0 * (float)aFreq) / (float)(s->nMTF) );
 
         for (v = 0; v < alphaSize; v++)
            if (v >= gs && v <= ge) 
               s->len[nPart-1][v] = BZ_LESSER_ICOST; else	//0
               s->len[nPart-1][v] = BZ_GREATER_ICOST;		//15
 
         nPart--;
         gs = ge+1;
         remF -= aFreq;
      }
   }

   /*--- 
      Iterate up to BZ_N_ITERS times to improve the tables.
   ---*/	//0 .. 4 .. ++
   for (iter = 0; iter < BZ_N_ITERS; iter++) {

      for (t = 0; t < nGroups; t++) fave[t] = 0;

      for (t = 0; t < nGroups; t++)
         for (v = 0; v < alphaSize; v++)
            s->rfreq[t][v] = 0;

      /*---
        Set up an auxiliary length table which is used to fast-track
	the common case (nGroups == 6). 
      ---*/
      if (nGroups == 6) {
         for (v = 0; v < alphaSize; v++) {
            s->len_pack[v][0] = (s->len[1][v] << 16) | s->len[0][v];	//possible values: 983055, 983040, 15, 0
            s->len_pack[v][1] = (s->len[3][v] << 16) | s->len[2][v];
            s->len_pack[v][2] = (s->len[5][v] << 16) | s->len[4][v];
		}
      }

      nSelectors = 0;
      totc = 0;
      gs = 0;
      while (True) {

         /*--- Set group start & end marks. --*/
         if (gs >= s->nMTF) break;
         ge = gs + BZ_G_SIZE - 1; 	//(0 or ge+1) + 49
         if (ge >= s->nMTF) ge = s->nMTF-1;

         /*-- 
            Calculate the cost of this group as coded
            by each of the coding tables.
         --*/
         for (t = 0; t < nGroups; t++) cost[t] = 0;
			//max dimension and first cicle
         if (nGroups == 6 && 50 == ge-gs+1) {
            /*--- fast track the common case ---*/
            register UInt32 cost01, cost23, cost45;
            register UInt16 icv;
            cost01 = cost23 = cost45 = 0;
			////for (nn = 0 to 50) BZ_ITER(nn) : update len_pack
#           define BZ_ITER(nn)                \
               icv = mtfv[gs+(nn)];           \
               cost01 += s->len_pack[icv][0]; \
               cost23 += s->len_pack[icv][1]; \
               cost45 += s->len_pack[icv][2]; \
			
            BZ_ITER(0);  BZ_ITER(1);  BZ_ITER(2);  BZ_ITER(3);  BZ_ITER(4);
            BZ_ITER(5);  BZ_ITER(6);  BZ_ITER(7);  BZ_ITER(8);  BZ_ITER(9);
            BZ_ITER(10); BZ_ITER(11); BZ_ITER(12); BZ_ITER(13); BZ_ITER(14);
            BZ_ITER(15); BZ_ITER(16); BZ_ITER(17); BZ_ITER(18); BZ_ITER(19);
            BZ_ITER(20); BZ_ITER(21); BZ_ITER(22); BZ_ITER(23); BZ_ITER(24);
            BZ_ITER(25); BZ_ITER(26); BZ_ITER(27); BZ_ITER(28); BZ_ITER(29);
            BZ_ITER(30); BZ_ITER(31); BZ_ITER(32); BZ_ITER(33); BZ_ITER(34);
            BZ_ITER(35); BZ_ITER(36); BZ_ITER(37); BZ_ITER(38); BZ_ITER(39);
            BZ_ITER(40); BZ_ITER(41); BZ_ITER(42); BZ_ITER(43); BZ_ITER(44);
            BZ_ITER(45); BZ_ITER(46); BZ_ITER(47); BZ_ITER(48); BZ_ITER(49);

#           undef BZ_ITER

            cost[0] = cost01 & 0xffff; cost[1] = cost01 >> 16;
            cost[2] = cost23 & 0xffff; cost[3] = cost23 >> 16;
            cost[4] = cost45 & 0xffff; cost[5] = cost45 >> 16;

         } else {
	    /*--- slow version which correctly handles all situations ---*/
            for (i = gs; i <= ge; i++) { 
               UInt16 icv = mtfv[i];
               for (t = 0; t < nGroups; t++) cost[t] += s->len[t][icv];
            }
         }//END generate constants
 
         /*-- 
            Find the coding table which is best for this group,
            and record its identity in the selector table.
         --*/
         bc = 999999999; bt = -1;
         for (t = 0; t < nGroups; t++)
            if (cost[t] < bc) { bc = cost[t]; bt = t; };
         totc += bc;
         fave[bt]++;
         s->selector[nSelectors] = bt;
         nSelectors++;

         /*-- 
            Increment the symbol frequencies for the selected table.
          --*/
         if (nGroups == 6 && 50 == ge-gs+1) {
            /*--- fast track the common case ---*/

#           define BZ_ITUR(nn) s->rfreq[bt][ mtfv[gs+(nn)] ]++

            BZ_ITUR(0);  BZ_ITUR(1);  BZ_ITUR(2);  BZ_ITUR(3);  BZ_ITUR(4);
            BZ_ITUR(5);  BZ_ITUR(6);  BZ_ITUR(7);  BZ_ITUR(8);  BZ_ITUR(9);
            BZ_ITUR(10); BZ_ITUR(11); BZ_ITUR(12); BZ_ITUR(13); BZ_ITUR(14);
            BZ_ITUR(15); BZ_ITUR(16); BZ_ITUR(17); BZ_ITUR(18); BZ_ITUR(19);
            BZ_ITUR(20); BZ_ITUR(21); BZ_ITUR(22); BZ_ITUR(23); BZ_ITUR(24);
            BZ_ITUR(25); BZ_ITUR(26); BZ_ITUR(27); BZ_ITUR(28); BZ_ITUR(29);
            BZ_ITUR(30); BZ_ITUR(31); BZ_ITUR(32); BZ_ITUR(33); BZ_ITUR(34);
            BZ_ITUR(35); BZ_ITUR(36); BZ_ITUR(37); BZ_ITUR(38); BZ_ITUR(39);
            BZ_ITUR(40); BZ_ITUR(41); BZ_ITUR(42); BZ_ITUR(43); BZ_ITUR(44);
            BZ_ITUR(45); BZ_ITUR(46); BZ_ITUR(47); BZ_ITUR(48); BZ_ITUR(49);

#           undef BZ_ITUR

         } else {
	    /*--- slow version which correctly handles all situations ---*/
            for (i = gs; i <= ge; i++)
               s->rfreq[bt][ mtfv[i] ]++;
         }

         gs = ge+1;
      }
      if (s->verbosity >= 3) {
         VPrintf2 ( "      pass %d: size is %d, grp uses are ", 
                   iter+1, totc/8 );
         for (t = 0; t < nGroups; t++)
            VPrintf1 ( "%d ", fave[t] );
         VPrintf0 ( "\n" );
      }

      /*--
        Recompute the tables based on the accumulated frequencies.
      --*/
      /* maxLen was changed from 20 to 17 in bzip2-1.0.3.  See 
         comment in huffman.c for details. */
      for (t = 0; t < nGroups; t++)
         BZ2_hbMakeCodeLengths ( &(s->len[t][0]), &(s->rfreq[t][0]), 
                                 alphaSize, 17 /*20*/ );
   }	//END For  4times


   AssertH( nGroups < 8, 3002 );
   AssertH( nSelectors < 32768 &&
            nSelectors <= (2 + (900000 / BZ_G_SIZE)),
            3003 );


   /*--- Compute MTF values for the selectors. ---*/
   {
      UChar pos[BZ_N_GROUPS], ll_i, tmp2, tmp;
      for (i = 0; i < nGroups; i++) pos[i] = i;
      for (i = 0; i < nSelectors; i++) {
         ll_i = s->selector[i];
         j = 0;
         tmp = pos[j];
         while ( ll_i != tmp ) {
            j++;
            tmp2 = tmp;
            tmp = pos[j];
            pos[j] = tmp2;
         };
         pos[0] = tmp;
         s->selectorMtf[i] = j;
      }
   };

   /*--- Assign actual codes for the tables. --*/
   for (t = 0; t < nGroups; t++) {
      minLen = 32;
      maxLen = 0;
      for (i = 0; i < alphaSize; i++) {
         if (s->len[t][i] > maxLen) maxLen = s->len[t][i];
         if (s->len[t][i] < minLen) minLen = s->len[t][i];
      }
      AssertH ( !(maxLen > 17 /*20*/ ), 3004 );
      AssertH ( !(minLen < 1),  3005 );
      BZ2_hbAssignCodes ( &(s->code[t][0]), &(s->len[t][0]), 
                          minLen, maxLen, alphaSize );
   }

   /*--- Transmit the mapping table. ---*/
   { 
      Bool inUse16[16];
      for (i = 0; i < 16; i++) {
          inUse16[i] = False;
          for (j = 0; j < 16; j++)
             if (s->inUse[i * 16 + j]) inUse16[i] = True;
      }
     
      nBytes = s->numZ;
      for (i = 0; i < 16; i++)
         if (inUse16[i]) bsW(s,1,1); else bsW(s,1,0);

      for (i = 0; i < 16; i++)
         if (inUse16[i])
            for (j = 0; j < 16; j++) {
               if (s->inUse[i * 16 + j]) bsW(s,1,1); else bsW(s,1,0);
            }

      if (s->verbosity >= 3) 
         VPrintf1( "      bytes: mapping %d, ", s->numZ-nBytes );
   }

   /*--- Now the selectors. ---*/
   nBytes = s->numZ;
   bsW ( s, 3, nGroups );
   bsW ( s, 15, nSelectors );
   for (i = 0; i < nSelectors; i++) { 
      for (j = 0; j < s->selectorMtf[i]; j++) bsW(s,1,1);
      bsW(s,1,0);
   }
   if (s->verbosity >= 3)
      VPrintf1( "selectors %d, ", s->numZ-nBytes );

   /*--- Now the coding tables. ---*/
   nBytes = s->numZ;

   for (t = 0; t < nGroups; t++) {
      Int32 curr = s->len[t][0];
      bsW ( s, 5, curr );
      for (i = 0; i < alphaSize; i++) {
         while (curr < s->len[t][i]) { bsW(s,2,2); curr++; /* 10 */ };
         while (curr > s->len[t][i]) { bsW(s,2,3); curr--; /* 11 */ };
         bsW ( s, 1, 0 );
      }
   }

   if (s->verbosity >= 3)
      VPrintf1 ( "code lengths %d, ", s->numZ-nBytes );

   /*--- And finally, the block data proper ---*/
   nBytes = s->numZ;
   selCtr = 0;
   gs = 0;
   while (True) {
      if (gs >= s->nMTF) break;
      ge = gs + BZ_G_SIZE - 1; 
      if (ge >= s->nMTF) ge = s->nMTF-1;
      AssertH ( s->selector[selCtr] < nGroups, 3006 );

      if (nGroups == 6 && 50 == ge-gs+1) {
            /*--- fast track the common case ---*/
            UInt16 mtfv_i;
            UChar* s_len_sel_selCtr 
               = &(s->len[s->selector[selCtr]][0]);
            Int32* s_code_sel_selCtr
               = &(s->code[s->selector[selCtr]][0]);

#           define BZ_ITAH(nn)                      \
               mtfv_i = mtfv[gs+(nn)];              \
               bsW ( s,                             \
                     s_len_sel_selCtr[mtfv_i],      \
                     s_code_sel_selCtr[mtfv_i] )

            BZ_ITAH(0);  BZ_ITAH(1);  BZ_ITAH(2);  BZ_ITAH(3);  BZ_ITAH(4);
            BZ_ITAH(5);  BZ_ITAH(6);  BZ_ITAH(7);  BZ_ITAH(8);  BZ_ITAH(9);
            BZ_ITAH(10); BZ_ITAH(11); BZ_ITAH(12); BZ_ITAH(13); BZ_ITAH(14);
            BZ_ITAH(15); BZ_ITAH(16); BZ_ITAH(17); BZ_ITAH(18); BZ_ITAH(19);
            BZ_ITAH(20); BZ_ITAH(21); BZ_ITAH(22); BZ_ITAH(23); BZ_ITAH(24);
            BZ_ITAH(25); BZ_ITAH(26); BZ_ITAH(27); BZ_ITAH(28); BZ_ITAH(29);
            BZ_ITAH(30); BZ_ITAH(31); BZ_ITAH(32); BZ_ITAH(33); BZ_ITAH(34);
            BZ_ITAH(35); BZ_ITAH(36); BZ_ITAH(37); BZ_ITAH(38); BZ_ITAH(39);
            BZ_ITAH(40); BZ_ITAH(41); BZ_ITAH(42); BZ_ITAH(43); BZ_ITAH(44);
            BZ_ITAH(45); BZ_ITAH(46); BZ_ITAH(47); BZ_ITAH(48); BZ_ITAH(49);

#           undef BZ_ITAH

      } else {
	 /*--- slow version which correctly handles all situations ---*/
         for (i = gs; i <= ge; i++) {
            bsW ( s, 
                  s->len  [s->selector[selCtr]] [mtfv[i]],
                  s->code [s->selector[selCtr]] [mtfv[i]] );
         }
      }


      gs = ge+1;
      selCtr++;
   }
   AssertH( selCtr == nSelectors, 3007 );

   if (s->verbosity >= 3)
      VPrintf1( "codes %d\n", s->numZ-nBytes );
}


/*---------------------------------------------------*/
void BZ2_compressBlock ( EState* s, Bool is_last_block )
{	//@ used in handle_compress[361]{bzlib.c}
   if (s->nblock > 0) {

      BZ_FINALISE_CRC ( s->blockCRC );	//crcVar = ~(s->blockCRC);
      s->combinedCRC = (s->combinedCRC << 1) | (s->combinedCRC >> 31);	//combinedCRC *2
      s->combinedCRC ^= s->blockCRC;	//^= : a=a&b binary operator
      if (s->blockNo > 1) s->numZ = 0;

      if (s->verbosity >= 2)
         VPrintf4( "    block %d: crc = 0x%08x, "
                   "combined CRC = 0x%08x, size = %d\n",
                   s->blockNo, s->blockCRC, s->combinedCRC, s->nblock );

      BZ2_blockSort ( s );
   }	//end notempty nblock

   s->zbits = (UChar*) (&((UChar*)s->arr2)[s->nblock]);

   /*-- If this is the first block, create the stream header. --*/
   if (s->blockNo == 1) {
      BZ2_bsInitWrite ( s );	//initialize s.live and s.buff = 0
      bsPutUChar ( s, BZ_HDR_B );	//bsW( s, 8, c ); //c ='B'
      bsPutUChar ( s, BZ_HDR_Z );					//c ='Z'
      bsPutUChar ( s, BZ_HDR_h );					//c ='h'
      bsPutUChar ( s, (UChar)(BZ_HDR_0 + s->blockSize100k) );	//c = Uchar blockSize100k
   }

   if (s->nblock > 0) {
		//bsW( s, 8, c );
      bsPutUChar ( s, 0x31 ); bsPutUChar ( s, 0x41 );	//c =codici speciali
      bsPutUChar ( s, 0x59 ); bsPutUChar ( s, 0x26 );	//c =codici speciali
      bsPutUChar ( s, 0x53 ); bsPutUChar ( s, 0x59 );	//c =codici speciali

      /*-- Now the block's CRC, so it is in a known place. --*/
      bsPutUInt32 ( s, s->blockCRC );	//4 bsw:for(i = 24;i>=0;i-=8) bsw(s,8,(s->blockCRC >> i) & 0xffL);  

      /*-- 
         Now a single bit indicating (non-)randomisation. 
         As of version 0.9.5, we use a better sorting algorithm
         which makes randomisation unnecessary.  So always set
         the randomised bit to 'no'.  Of course, the decoder
         still needs to be able to handle randomised blocks
         so as to maintain backwards compatibility with
         older versions of bzip2.
      --*/
      bsW(s,1,0);

      bsW ( s, 24, s->origPtr );
      generateMTFValues ( s );
      sendMTFValues ( s );
   }


   /*-- If this is the last block, add the stream trailer. --*/
   if (is_last_block) {

      bsPutUChar ( s, 0x17 ); bsPutUChar ( s, 0x72 );//c =codici speciali
      bsPutUChar ( s, 0x45 ); bsPutUChar ( s, 0x38 );
      bsPutUChar ( s, 0x50 ); bsPutUChar ( s, 0x90 );
      bsPutUInt32 ( s, s->combinedCRC );
      if (s->verbosity >= 2)
         VPrintf1( "    final combined CRC = 0x%08x\n   ", s->combinedCRC );
      bsFinishWrite ( s );
   }
}


/*-------------------------------------------------------------*/
/*--- end                                        compress.c ---*/
/*-------------------------------------------------------------*/
