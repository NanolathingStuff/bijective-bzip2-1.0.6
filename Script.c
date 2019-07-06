#include <sys/time.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define DEF "bzip2 -k "
#define MINE "./"
#define COMPRESSED ".bz2"

struct timeval  tv1, tv2;
struct stat buffer;
const char *filenames[1000] = {"Steps.txt", "Sempty.txt",  "bij-transform.c","a-bijective.pdf"};

int main(){

	if( access("results.txt" , F_OK) != -1 ) { //file exists	
		fprintf(stderr, "File already exist.  Replacing;\n");	//replace with new one
		remove("results.txt");
	}
	FILE *file = fopen("results.txt", "w");	//FILE *file = fopen(filename, mode);
	fprintf(file, "DEFAULT BZIP2:	\t\t\t\t		MY IMPLEMENT:\n");

	char *command1, *command2, *output;
	
	int i = 0, status, size, corr;
	
	for(i = 0; i<4; i++){ //while()...{
		/* stuff to do! */
		corr = 0;
		command1 = malloc(60*sizeof(char));//char command[60]; //string = malloc (60 chars);
		command2 = malloc(62*sizeof(char));
		output = malloc(30*sizeof(char));
		//printf("%s\n", filenames[i]);
		strcpy(output, filenames[i]);
		strcat(output, COMPRESSED);
		//BZIP2
		strcpy(command1, DEF);//string = strcat(default, filenames[i])
		strcat(command1, filenames[i]);	//strcat(str, "strings ");
		gettimeofday(&tv1, NULL);
		system(command1);//("bzip2 -k Steps.txt ");	//exe
		gettimeofday(&tv2, NULL);
		
		strcat(command1, COMPRESSED);//string = strcat(default, filenames[i], compressed)
		status = stat(output, &buffer);//("Steps.txt.bz2", &buffer);
		if(status == 0) {
			size = buffer.st_size;	// size of file is in member buffer.st_size;
		}else {	corr = 1; }	//corruption
		status = remove(output);//("Steps.txt.bz2"); DELETE FILE
		if (status != 0)  return 1;
		
		if(corr == 0)
			fprintf(file, "Total time = %f seconds, size %d	\t\t",(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec), size);
		else
			fprintf(file, "Total time = %f seconds, file corrupted	\t\t",(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));
		//printf("%s\n", command1);
		free(command1);//free(string)
		///MY
		
		strcpy(command2, MINE);	//string = strcat(mine, default, filenames[i])	
		strcat(command2, DEF);
		strcat(command2, filenames[i]);
		gettimeofday(&tv1, NULL);
		system(command2); //("bzip2 -k Steps.txt ");
		gettimeofday(&tv2, NULL);
		//strcat(command2, COMPRESSED);//string = strcat(mine, default, filenames[i],compressed)
		status = stat(output, &buffer); //stat("./Steps.txt.bz2", &buffer);
		if(status == 0) {
			size = buffer.st_size;	// size of file is in member buffer.st_size;
		}else {	corr = 1;	}	//corruption	
		status = remove(output);//("Steps.txt.bz2"); DELETE FILE
		if (status != 0)  return 1;

		if(corr == 0)
			fprintf(file, "Total time = %f seconds, size %d		\n",(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec), size);
		else
			fprintf(file, "Total time = %f seconds, file corrupted		\n",(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));
		//printf("%s\n", command2);
		free(command2);//free(string)
		free(output);
		}  
		
	fclose(file);
	return 0;   
}
