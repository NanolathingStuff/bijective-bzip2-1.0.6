#ifndef RUN_LENGTH_H
#define RUN_LENGTH_H
#include <stdlib.h>
#include <ctype.h>

extern unsigned int BZ2_crc32Table[256];

typedef struct{
	unsigned int   state_in_ch;
    int    state_in_len;
    int rNToGo;                               
   	int rTPos ;		
   	unsigned int blockCRC;
   	//non della run-length
   	int     inUse[256];	//BOOL
   	unsigned char*   block;
   	int nblock;
}EState;

void init_RL ( EState* s );
int isempty_RL ( EState* s );
void add_pair_to_block ( EState* s );
void flush_RL ( EState* s );

#endif
