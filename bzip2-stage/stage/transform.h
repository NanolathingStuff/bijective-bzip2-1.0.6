#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <stdlib.h>
#include <string.h>

char * appendC(char *str, char c);
const char * S_transform(const char *a);
void removeC(char *str, char c);
const char * CyclicRotations(char *a);
char * last(char *a);
char minC(char *s);	
char * LyndonFact(char *s);
char * BWT(char * s);
void orderMatrix(const char * s, short size);

#endif
