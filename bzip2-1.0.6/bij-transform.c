#include <stdio.h>
#include "transform.h"

/*const char * S_transform(const char *a){
	char w =LyndonFact(a);// factor(a);	//Duvalís algorithm for computing the Lyndon factorization
	char R[] = {""};
	size_t i;		//for(int i = 0; i < w.length(); i++)
	for(i = 0; i < strlen(w); i++)
		R = strncat(R, CyclicRotations(w), strlen(w)); //	R = R + CyclicRotations(w); 
	return last(R);
}*/
char * BWT(char * s){
 	//const char *R = CyclicRotations(s); // R = CyclicRotations(s)
	//return last(R);
	size_t dim = strlen(s);
	char copy[dim], rotations[dim][dim];
	int i = 0, j, k;
	for(;i<dim; i++){
		copy[dim] = s[i];
	//ciclic rotations(s){
		j = i;  // Current index in str 
        k = 0;  // Current index in temp 
        while (s[j] != '\0') { 
            rotations[i][k] = s[j]; 
            k++; 
            j++; 
        } 
        j = 0; 
        while (j < i) { 
            rotations[i][k] = s[j]; 
            j++; 
            k++; 
        } 
        rotations[i][dim+1] = '\0'; //end ciclic}	
	}   
	//printf("%s\n", rotations);
	for(i = 0; i < dim; i++){
		//printf("%c",rotations[i][dim-1]);
		copy[i] = rotations[i][dim-1];
	}
	copy[dim] = '\0';
	//printf("\n");
	//printf("%s\n", copy);
	char ch, res[dim], *r = res;
	for(i = 0;i<dim; i++){
		ch = minC(copy);
		r[i] = ch;
		removeC(copy,ch);
	}
	/*char ch, res[dim], *r = res;
	int p = 0;
	 j = 0;
	for(i = 0; i < dim; i++){
		copy[i] = rotations[i][0];
	}
	copy[dim] = '\0';
	for(i = 0;i < dim; i++){
		p = minN(copy);	
		if(p != i){	//swap  (rotations[i], rotations[p]),(copy[i], copy[p])
			for(k = 0; k < dim; k++){
				ch = rotations[p][k];
				rotations[p][k] = rotations[i][k];
				rotations[i][k] = ch;
			} 
			ch = copy[p];
			copy[p] = copy[i];
			copy[i] = ch;
		}
		removeC(copy, ch);
	}
	for(i = 0;i < dim; i++)
		res[i] = rotations[i][dim -1];*/
	return r;
}
/*char * LyndonFact(char *s){
	int i = 0, n = strlen(s), j, k;
	//char fact[] = "";
	char *lyndon = malloc(2*n);//fact;
	
	//printf("entra while\n");
	
	while (i < n) {
		
		printf("%d ", i);
		
		j = i+1, k = 1;
		while (j < n && s[k] <= s[j]) {
			
			printf("%d\n", j);
			
            if (s[k] < s[j])
                k = i;
            else
                k++;
            j++;
        }
        
        printf("enter copy\n");
        
        while (i <= k) {
            strcat(lyndon, strncpy( lyndon, &s[i], j-k ));	//factorization.push_back(s.substr(i, j - k));
            i += j - k;
            
            printf("%s\n", lyndon);
            
        }
	}
	
	printf("OK");
	
	lyndon[2*n-1] = '\0';
	return lyndon;
}
*/char * LyndonFact(char* s){ 	//vector<string> duval(string const& s) {
	
    int n = strlen(s), i = 0, j, k, cont, ll = 0;
    char* factorization;
    while (i < n) {
        j = i + 1;
		k = i;
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j])
                k = i;
            else
                k++;
            j++;
        }
        while (i <= k) {
        	/////////////////////////////
        	//appendi stringa(break) alla lista di stringhe -> array di stringhe(matrice di char)
        	///////////////////////////////
           
		    //factorization.push_back(s.substr(i, j - k)); : append element s.substring(i, j-k) to factorization
         
       	     i += j - k;
        }
    }
	printf("\n");
    return factorization;
}

//@reimplement append single char
char * appendC(char *str, char c){
	size_t len = strlen(str);
    char *str2 = malloc(len + 2); /* one for extra char, one for trailing zero */
    if(str2 == NULL){	//try...catch... ?
		printf("Memoria esaurita\n");
		exit(1);}
    strcpy(str2, str);
    str2[len] = c;
    str2[len + 1] = '\0';
    //@perchË mi crasha la free improvvisamente?
    //free(str); //memoria inutile
    return str2;
}
void removeC(char *str, char c){
	int len = strlen(str);
	int i = 0;
	while(i < len && str[i] != c){
		i++;
	}  
  	while(i < len && str[i] != '\0'){
  		str[i] = str[i + 1];
		i++;  
	}
}
/*char * last(char *s, int dim){
 	char copy[strlen(s) + 1]; //copia per non modificare s
 	int i = 0;
 	for(i = 0; i < (dim + 1)*sizeof(char); i++){
 		copy[i] = s[dim * i];//i+1?
 	}
 	char *n = malloc((strlen(s) + 1)*sizeof(char));	
 	char p ;
	for(i = 0; i < (strlen(s) + 1)*sizeof(char); i++){	//eseguito (i<n) n/2 volte, se metto i<22 (32>>stlen(s)) non crasha, semplicemente non inserisce altri cartteri
		p = minC(copy);
		n[i] = p;
 		removeC(copy, p);		
	}
	n[strlen(s)] = '\0';
	
	return n;
}*/
const char * CyclicRotations(char *a){
	int n =  strlen(a);//let n Åﬂ |a|
	char R[n][n];
	char *matrix = R[0];//a = R;
	int i, j, k;
	
	for (i = 0; i < n -1; i++){//for(int i = 0; i < a.length(); i++)
		j = i;  // Current index in str 
        k = 0;  // Current index in temp 
        while (a[j] != '\0') { 
            R[i][k] = a[j]; 
            k++; 
            j++; 
        } 
        j = 0; 
        while (j < i) { 
            R[i][k] = a[j]; 
            j++; 
            k++; 
        } 
        //R[i][n] = ' '; //printf
        R[i][n] = '\0'; 
	}
	
	return matrix;	
}
void CyclicRotation(char *a, short size){
	char R[size][size];
	int i, j, k;
	
	for (i = 0; i < size -1; i++){//for(int i = 0; i < a.length(); i++)
		j = i;  // Current index in str 
        k = 0;  // Current index in temp 
        while (a[j] != '\0') { 
            R[i][k] = a[j]; 
            k++; 
            j++; 
        } 
        j = 0; 
        while (j < i) { 
            R[i][k] = a[j]; 
            j++; 
            k++; 
        } 
        //R[i][n] = ' '; //printf
        R[i][size] = '\0'; 
	}
	a = *R;		
}
char * last1(char *s){
 	char copy[strlen(s) + 1]; //copia per non modificare s
 	strcpy(copy, s);
 	char *n = malloc((strlen(s) + 1)*sizeof(char));
 	//printf("%d ",sizeof(char));//1
 	/*printf("%d ",strlen(s));//11
 	printf("%d \n",strlen(n));//lunghezza attuale stringa, non dimensione array*/	
 	int i = 0;
 	char p ;
	for(; i < (strlen(s) + 1)*sizeof(char); i++){	//eseguito (i<n) n/2 volte, se metto i<22 (32>>stlen(s)) non crasha, semplicemente non inserisce altri cartteri
		p = minC(copy);
		n[i] = p;
 		removeC(copy, p);
 	//	printf("%d ",i);
	 /*	printf("%c \n",p);
 		printf("length n: %d \n",strlen(n));//3?
 		printf("input: %s\n",copy);
 		printf("output: %s\n ",n);*/
 		
	}
	n[strlen(s)] = '\0';
	
	return n;
}/**/
char maxC(char *s){
	char m = (char)1; //smallest possible ASCII char
	int i;
	for (i = 0; i < strlen(s); i++){
		if ((int)s[i] > (int)m )
			m = s[i];}
	return m;
}
char minC(char *s){
	char m = (char)125; //greatest possible ASCII char
	int i;
	for (i = 0; i < strlen(s); i++){
		if ((int)s[i] < (int)m && s[i] != ' ')//spazio ha valore 32 (piccolo)
			m = s[i];
		}
	if((int)m == 125)
		return ' ';
	return m;
}
int minN(char *s){
	char m = (char)125; //greatest possible ASCII char
	int i,j;
	for (i = 0; i < strlen(s); i++){
		if ((int)s[i] < (int)m && s[i] != ' ')//spazio ha valore 32 (piccolo)
			m = s[i];
			j = i;
		}
	if((int)m == 125)
		return 0;
	return j;
}
//to complete
void orderMatrix(char * s, short size){
	char w[size][size] ;
	unsigned int i, j, k;
		k = 0;
		for ( i = 0; i < size ; i++){
			for ( j = 0; i < size ; i++){
				w[i][j] = s[k];
				k++;
			}
	/*		w[0][0] = 'H';
			w[0][1] = 'e';
			w[0][2] = 'l';
			w[0][3] = 'l';
			w[0][4] = 'o';
			w[0][5] = '\0';
			w[1][0] = 'C';
			w[1][1] = 'i';
			w[1][2] = 'a';
			w[1][3] = 'o';
			w[1][4] = '!';
			w[1][5] = '\0';
			w[2][0] = 'H';
			w[2][1] = 'o';
			w[2][2] = 'l';
			w[2][3] = 'h';
			w[2][4] = 'a';
			w[2][5] = '\0';
			w[3][0] = 'H';
			w[3][1] = 'e';
			w[3][2] = 'l';
			w[3][3] = 'l';
			w[3][4] = 'o';
			w[3][5] = '\0';
			w[4][0] = 'o';
			w[4][1] = 'l';
			w[4][2] = 'l';
			w[4][3] = 'e';
			w[4][4] = 'H';
			w[4][5] = '\0';
	//}*/
	printf("%s\n", w);
		}//
	char temp[size];
/*	for ( i = 0; i < size - 1; i++)
        for ( j = i + 1; j < size; j++)
            if (w[i][0] > w[j][0]){	// simply swap the characters
               strcpy(tmp, w[i]);
			   strcpy(w[i], w[j]);
			   strcpy(w[j], tmp); 
		}*/
      for(i=0;i<=size;i++)
      for(j=i+1;j<=size;j++)
      {
           if(strcmp(w[i],w[j])>0)
           {
            	strcpy(temp,w[i]);
             	strcpy(w[i],w[j]);
              	strcpy(w[j],temp);
           }
      }
  //printf("The sorted string\n");

  //for(i=0;i <= size;i++){
  //    puts(w[i]);}
//	printf("%s",w[0]);
}
//ordered matrix working but crash
static int myCompare(const void* a, const void* b) { 
  //??WHAT IS THIS?
    // setting up rules for comparison 
    return strcmp(*(const char**)a, *(const char**)b); 
} 
void sort(char* arr[], int n) { 
    // calling qsort function to sort the array  with the help of Comparator 
    qsort(arr, n, sizeof(const char*), myCompare); 
} 
void print(char *t) { 	//@to print strings?
   if (*t == '\0')
      return;
   printf("%c", *t);
   print(++t);
}

int main(int argc, char *argv[]){
	//char s[100] = "I am learning C programming language";
	char s[11] = "Hello World";
	printf("%s\n", s);
	char *z = s;
	z = appendC(z, '!');
	printf("%s", z);
	printf("	appendC: OK\n");
	removeC(z, 'l');
	printf("%s", z);
	printf("	removeC: OK\n");
	char c = minC(z);
	printf("%c, ", c); 
	printf("%c", maxC(z));//
	printf("		minC, maxC: OK\n");	/**/
	free(z) ;
	z = NULL;
	const char *m = CyclicRotations("BananA$");
	printf("%s\n", m);
	printf("	CyclicRotations: OK\n");/**/
/*	orderMatrix(m, strlen("BananA$"));
	printf("%s\n", m);
	printf("	ordered CyclicRotations\n");*/
	//char b[12] = "ciao mondo"; //OK s<->b
	//removeC(s,' '); //OK
	/*a = last(a, strlen(s));
//	printf("%s%s", z, a);
	printf("prima: %s\n", s);
	printf("dopo: %s", a);
	printf("	last: OK\n");/**/
	int i= 0;
/*	char *burr = BWT("Ciao Mondo!!");
	printf("Prima: Ciao Mondo!! \nDopo: ");	//c'Ë un bug in "%s" in string.h con stringhe di certe dimensioni
	//printf("%s", burr);
	for(i=0; i < strlen("Ciao Mondo!!"); i++)
		printf("%c",burr[i]);
	
	printf("\n");
	burr = BWT("BANANA");
	printf("Prima: BANANA \nDopo: %s", burr);
	///////////////////////MEMORY LEAK HERE
	printf("	BWT: OK\n");
	free(burr);*/
	
	char* arr[]  = { "geeksforgeeks", "geeksquiz", "clanguage", "helloworld", "geekt", "HELLO", "Hi" }; 
	int n = sizeof(arr) / sizeof(arr[0]);
	sort(arr, n); 
	for(i = 0; i < n; i++)
		printf("%s ",arr[i]);
		
	printf("	Order matrix: OK\n");/**/
/*	a = LyndonFact(a);
	printf("%s", a);
	printf("	LyndonFact: OK\n");	*/
	//free(arr);
	//free(a);
	
	char *test = (char*) malloc(100*sizeof(char));
    strcpy(test, "I am learning C programming language");	//abrakadabra//hello WORLD
    printf("%s\n", test);
    char *W = LyndonFact(test);
    //////////////////////////MEMORY LEAK HERE
    printf("%s", W);
    free(test);
    printf("	LyndonFact: OK\n");/**/
    
	return 0;
}
