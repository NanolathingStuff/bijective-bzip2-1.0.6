#include <stdio.h>
#include "transform.h"

/*const char * S_transform(const char *a){
	char w =LyndonFact(a);// factor(a);	//Duval’s algorithm for computing the Lyndon factorization
	char R[] = {""};
	size_t i;		//for(int i = 0; i < w.length(); i++)
	for(i = 0; i < strlen(w); i++)
		R = strncat(R, CyclicRotations(w), strlen(w)); //	R = R + CyclicRotations(w); 
	return last(R);
}
char * bij_transform(char * a){
	int *w = LyndonFact(a);
	char *R;
	
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
int* LyndonFact(char* s){ 	//vector<string> duval(string const& s) {
	//printf("%s\n", s);
    int n = strlen(s), i = 0, j, k;
    
    int breaks[strlen(s)];
    breaks[0] = 0;
    // char* factorization;
    int cont = 0;
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
        	//appendi stringa(break) alla lista di stringhe -> array di stringhe(matrice di char)
        	cont++;
        	breaks[cont] = i + j - k ;//i + (j - k);
        //	printf("%d ",breaks[cont]);
		    //factorization.push_back(s.substr(i, j - k)); : append element s.substring(i, j-k) to factorization
         	
       	    i += j - k;
        }
    }
    //printf("\n");
	int *b = malloc(sizeof(int) * cont); //breaks;/*

	for(i = 0; i < cont +1; i++){
		b[i] = breaks[i];
    //	printf("%d ",b[i]);
    }
    //printf("return\n");
	return b;
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
    //@perchè mi crasha la free improvvisamente?
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
char * last(char *s){
	int dim = strlen(s);
	//int i,j;//printf("%d",dim);
	//"ciaociaociaociau" FUNZIONE -> problema cyclic rotation	
	char m[dim ][dim + 1];
	int i, j, k;//const char * r = CyclicRotations(s) ;printf("rotations: %s\n", r);
	///CYCLICROTATIONs NOt WORKING: #preprocessore DA QUI
	for (i = 0; i < dim ; i++){
		j = i;  
        k = 0;  
        while (s[j] != '\0') { 
            m[i][k] = s[j]; 
            k++; 
            j++; 
        } 
        j = 0; 
        while (j < i) { 
            m[i][k] = s[j]; 
            j++; 
            k++; 
        } 
        m[i][dim] = '\0'; 
	}	//A QUI!
//	char *cyclic = strcpy(cyclic, r);
	//char copy[dim*dim];
	//strcpy(copy, r);
	//printf("test: %c", copy[0]);
	char *res = (char*)malloc((dim + 1)*sizeof(char));
	//char m[dim ][dim + 1];

	/*for(i = 0; i<dim; i++){
		for(j = 0; j < dim; j++){
			//printf("insert: %c	", r[(i*dim) + j] );
			m[i][j] = r[(i*dim) + j];
		}	//	memcpy( m[i], &r[10 ], dim);// &r[10*sizeof(char)]
		//printf("\n");
		//	m[i] = substring( r, i*dim, dim);
		m[i][dim] = '\0';
		//printf("%s\n", m[i]);	
	}//printf("%s\n", m);*/
	//take m[i][dim-1]
	//////////PRPROCESSOR THIS SHIT in some sort function
	//orderMatrix(*m, (short)dim);
	char minS[dim+1], tmp[dim+1];
	int min_idx;
	for (i = 0; i < dim-1; i++) { 
        // Find the minimum element in unsorted array 
        int min_idx = i; 
        strcpy(minS, m[i]); 
        for (j = i+1; j < dim; j++) { 
            // If min is greater than arr[j] 
            if (strcmp(minS, m[j]) > 0)  { 
                // Make arr[j] as minStr and update min_idx 
                strcpy(minS, m[j]); 
                min_idx = j; 
            } 
        } 
   
        // Swap the found minimum element with the first element 
        if (min_idx != i){  
            strcpy(tmp, m[i]); //swap item[pos] and item[i] 
            strcpy(m[i], m[min_idx]); 
            strcpy(m[min_idx], tmp); 
        } 
    } 
	//sort(m, dim);	
	for(i = 0; i<dim; i++){
	//	printf("%s\n", m[i]);
		res[i] = m[i][dim-1];
	}res[dim]='\0';/**/ 
 	return res;
}
char * CyclicRotations(char *a){
	int n =  strlen(a);//let n ß |a|
	char R[n][n];
	/*for(i = o to n){r[i]=malloc....}???? GUARDA COmE FARE
	for( i = 0; i<n; i++)
		R[i] = malloc(sizeof(char)*n);*/
	
	char *matrix = (char*)malloc((n*n+1)*sizeof(char));//R[0];//a = R;//(char*)malloc(n*n*sizeof(char));
	int i, j, k;
	
	for (i = 0; i < n ; i++){//for(int i = 0; i < a.length(); i++)
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
	
	int cont =0;
	for (i = 0; i < n ; i++)
		for (j = 0; j < n ; j++){
			matrix[cont++] = R[i][j];
		}matrix[n*n] = '\0'; 
	//printf("copia di:%s\n",a);
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
static int myCompare(const void* a, const void* b) { //controlla se 2 oggetti sono identici
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
void selectionSort(char arr[][256], int n) { 
    int i, j, min_idx; 
   
    // One by one move boundary of unsorted subarray 
    char minStr[n]; 
    for (i = 0; i < n-1; i++) 
    { 
        // Find the minimum element in unsorted array 
        int min_idx = i; 
        strcpy(minStr, arr[i]); 
        for (j = i+1; j < n; j++) 
        { 
            // If min is greater than arr[j] 
            if (strcmp(minStr, arr[j]) > 0) 
            { 
                // Make arr[j] as minStr and update min_idx 
                strcpy(minStr, arr[j]); 
                min_idx = j; 
            } 
        } 
   
        // Swap the found minimum element with the first element 
        char temp[n]; 
        if (min_idx != i)  { 
            strcpy(temp, arr[i]); //swap item[pos] and item[i] 
            strcpy(arr[i], arr[min_idx]); 
            strcpy(arr[min_idx], temp); 
        } 
    } 
} 

char * bij_t(char * str){
	int *W = LyndonFact(str), dim = strlen(str), i = 1, j = 0, cont = 0, l = 0, leng;

	leng = sizeof(W)/sizeof(W[0]);
	printf("size:%d\n", leng);
		
	char *R = (char*)malloc((dim*dim)*sizeof(char));
	char tmp[dim], c;
	while (i < leng){
		printf("%d\n",W[i]);
		
		
		cont = 0;//tmp[0--len] = str[j] ;R = R U CyclicRotations(w), w € W
		for(; j < W[i];j++){
			tmp[cont] = str[j];
			cont++;	
		}
		
		char *m = CyclicRotations(tmp); 
		printf("%s\n%s %d\n", tmp, m, strlen(m));
		//strcpy(m,r);
		for(cont = 0; cont < strlen(m);cont++){
			//printf("%s",m);
			c = m[cont];
			R[l] = c;
			//printf("%c %c %c\n",  c, m[cont], R[l]); //CAN't ACCESS R[l]
			l++;	
		}
		memset(&tmp[0], 0, sizeof(tmp)); //empty tmp
		i++;
		free(m);
	}
	//printf("result = %s\n", R);
	
	char *res; //= (char*)malloc(strlen(str)*sizeof(char)); //GIà messa il last
	res = last(R);
	free(R);
	printf("return");
	return res;//str	
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
	char *m = CyclicRotations("BananA$");
	printf("%s", m);
	printf("	CyclicRotations: OK\n");/*
	////PROBLEMS PASSING ROTATIONS AS ARGUMENT
	const char *m1 = CyclicRotations(m);
	printf("%s", m1);
	printf("	CyclicRotations: OK\n");*/
/*	orderMatrix(m, strlen("BananA$"));
	printf("%s\n", m);
	printf("	ordered CyclicRotations\n");*/
	//char b[12] = "ciao mondo"; //OK s<->b
	//removeC(s,' '); //OK
	char * a = "burrows-wheeler"; //"burrows-wheeler""ciao";
	printf("prima: %s\n", a);
	a = last(a);
	printf("dopo: %s", a);
	printf("	last: OK\n");/**/
	int i= 0;
/*	char *burr = BWT("Ciao Mondo!!");
	printf("Prima: Ciao Mondo!! \nDopo: ");	//c'è un bug in "%s" in string.h con stringhe di certe dimensioni
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
	
	char *test = (char*) malloc(100*sizeof(char));
    strcpy(test, "I am learning C programming language!");	//abrakadabra//hello WORLD //I am learning C programming language//hola!//abacabab
    printf("prima: %s\n", test);
    int *W = LyndonFact(test);
    int co = 0;
    for(i = 1; i < (sizeof(W))*2/sizeof(int); i++){
    //	printf("%d %d",W[i-1],W[i]);
    	for(co = W[i-1]; co < W[i]; co++) {
    		printf("%c", test[co]);
    	} 
		printf("\n");
    }	
    free(test);
    printf("	LyndonFact: OK\n");/**/
    
    
    bij_t("hola!");
	return 0;
}
