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
	int i = 0,j,k;
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
	return r;
}
char * LyndonFact(char *s){
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
/**/

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
void orderMatrix(const char * s, short size){
	char temp[size];
	int min[size];
	char R[size][size];
	int i, n,l = 0; //contatore riga, contatore stringa, contatore linea
	while (n < strlen(s)){
		for (i = 0; i < size; i++){
			R[l][i] = s[n];
			n++;
		}
		l++;
		temp[l] = s[l];
	}
	char p;
	int pos;
	for (i = 0; i < size; i++){
		pos = 0;
		p = minC(temp);
		while(p != temp[pos]){
			pos++;
		}
		min[i] = pos;
		removeC(temp, p);
		
		printf("min : %d", pos, "	temp: %s\n",temp);	
	}
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
	const char *m = CyclicRotations("BananA$");
	printf("%s\n", m);
	printf("	CyclicRotations: OK\n");/**/
/*	orderMatrix(m, strlen("BananA$"));
	printf("%s\n", m);
	printf("	ordered CyclicRotations\n");*/
	//char b[12] = "ciao mondo"; //OK s<->b
	//removeC(s,' '); //OK
	char *a = s;
	/*a = last(a, strlen(s));
//	printf("%s%s", z, a);
	printf("prima: %s\n", s);
	printf("dopo: %s", a);
	printf("	last: OK\n");/**/
	char *burr = BWT("Ciao Mondo!!");
	printf("Prima: Ciao Mondo!! \nDopo: %s", burr);	//c'Ë un bug in "%s" in string.h con stringhe di certe dimensioni
	printf("	BWT: OK\n");/**/
/*	a = LyndonFact(a);
	printf("%s", a);
	printf("	LyndonFact: OK\n");	*/
	
	//free(a);
	free(z);
	
	return 0;
}
