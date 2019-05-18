#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <stdlib.h>
#include <string.h>

/*void free_item(item *x) {
  free(x);
  x = NULL;
}*/

char * appendC(char *str, char c);
const char * S_transform(const char *a);
void removeC(char *str, char c);
const char * CyclicRotations(char *a);
void CyclicRotation(char *a, short size); //how to override
char * last(char *a);
char minC(char *s);	
int * LyndonFact(char *s);
char * BWT(char * s);
void orderMatrix(char * s, short size);

/*Vector
typedef struct dynamic_vector{
	char* data;
	size_t size; // Total size of the vector
	size_t count; //Number of vectors in it at present
} cvector;
void vector_init(cvector *v){
	v->data = NULL;
	v->size = 0;
	v->count = 0;
}
int vector_count(cvector *v){
	return v->count;
}
void vector_free(cvector *v)
{
	free(v->data);
	vector_init(v);
}
void vector_add(cvector *v, char *e){
	if (v->size == 0) {
		v->size = 10;
		v->data = malloc(sizeof(void*) * v->size);
		memset(v->data, '\0', sizeof(void) * v->size);
	}

	// condition to increase v->data:
	// last slot exhausted
	if (v->size == v->count) {
		v->size *= 2;
		v->data = realloc(v->data, sizeof(void*) * v->size);
	}

	v->data[v->count] = e;
	v->count++;
}
*/
char *substring(const char *string, int position, int length) {
   char *pointer;
   int c;

   pointer = malloc(length+1);

   if (pointer == NULL)   {
      printf("Unable to allocate memory.\n");
      exit(1);   }

   for (c = 0 ; c < length ; c++)   {
      *(pointer+c) = *(string+position-1);      
      string++;   
   }

   *(pointer+c) = '\0';

   return pointer;
}

#endif
