#include <sys/time.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <libgen.h>

#define SUBDIR "./Test-files/"
#define DEF "bzip2 -k "
#define MINE "./"
#define COMPRESSED ".bz2"
#define NFILES 53

struct timeval  tv1, tv2;
struct stat buffer;
///INSERT file names to try here
const char *filenames[1000] = {"Steps.txt", "Sempty.txt",  "bij-transform.c","a-bijective.pdf","4.mp3", "r1000", "r5000",
								"zeros", "x", "x1", "x2","x3","x4","x5","x6","x7","x8","x9","x10", "tcgen.exe", "tcgen.cpp",
								"Galaxy.bmp","Wolf.bmp","Galaxy.eps","Wolf.eps","Galaxy.exr","Wolf.exr","Galaxy.gif","Wolf.gif",
								"Galaxy.ico","Wolf.ico","Galaxy.jpg","Wolf.jpg","Galaxy.png","Wolf.png","Galaxy.tga","Wolf.tga",
								"Galaxy.tiff","Wolf.tiff","Galaxy.svg","Wolf.svg","Galaxy.wbmp","Wolf.wbmp","test.py","jmock.zip",
								"mainwindow.h","Curriculum.docx", "asgal.py","CurrencyConverter.java","detectEventsPaired.py",
								"formatSAMPaired.py","HelloWorldServlet.java","SpliceAwareAlignerPaired.cpp"};												

int main(){

	if( access("results.txt" , F_OK) != -1 ) { //file exists	
		fprintf(stderr, "File already exist.  Replacing;\n");	//replace with new one
		remove("results.txt");
	}
	FILE *file = fopen("results.txt", "w");	//FILE *file = fopen(filename, mode);
	fprintf(file, "DEFAULT BZIP2:	\t\t\t\t		MY IMPLEMENT:\t\t\t\t\t		RISPARMIO\n");

	char *command1, *command2, *output;
	
	int i = 0, status, size, corr, tmp1 ;
	double tmp2, totT = 0.0, totS = 0.0, totC = 0.0, totB = 0.0, totM = 0.0;
	
	/*COPY FROM FOLDER
	int src_fd, dst_fd, err, n;
    unsigned char buff[4096];
    char * src_path, * dst_path;
	
	src_path = SUBDIR;
    dst_path = "./";

    src_fd = open(src_path, O_RDONLY);
    dst_fd = open(dst_path, O_CREAT | O_WRONLY);
	////END copy*/
	
	for(i = 0; i < NFILES; i++){ //while()...{
		/*copy file
		err = read(src_fd, buff, 4096);
        if (err == -1) {
            printf("Error reading file.\n");
            exit(1); }
        if (err == 0) break;
		   n = err;
		   err = write(dst_fd, buff, n);
        if (err == -1) {
            printf("Error writing to file.\n");
            exit(1);
        }*/

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
		//strcat(command1, SUBDIR);
		strcat(command1, filenames[i]);	//strcat(str, "strings ");
		gettimeofday(&tv1, NULL);
		system(command1);//("bzip2 -k Steps.txt ");	//exe
		gettimeofday(&tv2, NULL);
		
		//TOT dimension before compression
		status = lstat(filenames[i], &buffer);//("Steps.txt.bz2", &buffer);
		if(status == 0) {
			totB += buffer.st_size;
		}else {	corr = 1; }	//corruption
		
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
		tmp1 = size;
		tmp2 = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
		///MY
		
		strcpy(command2, MINE);	//string = strcat(mine, default, filenames[i])	
		strcat(command2, DEF);
		//strcat(command2, SUBDIR);
		strcat(command2, filenames[i]);
		gettimeofday(&tv1, NULL);
		system(command2); //("bzip2 -k Steps.txt ");
		gettimeofday(&tv2, NULL);
		//strcat(command2, COMPRESSED);//string = strcat(mine, default, filenames[i],compressed)
		status = stat(output, &buffer); //stat("./Steps.txt.bz2", &buffer);
		if(status == 0) {
			size = buffer.st_size;	// size of file is in member buffer.st_size;
			totC += size;
		}else {	corr = 1;	}	//corruption	
		status = remove(output);//("Steps.txt.bz2"); DELETE FILE
		if (status != 0)  return 1;

		if(corr == 0)
			fprintf(file, "Total time = %f seconds, size %d		\t\t",(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec), size);
		else
			fprintf(file, "Total time = %f seconds, file corrupted		\t\t",(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));
		//printf("%s\n", command2);
		free(command2);//free(string)
		free(output);
		
		fprintf(file, "	Risparmio = %f seconds,  %d bytes, percent compression more: %f '%' \n", tmp2 - (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec),
				tmp1 - size, ((double)tmp1 - (double)size)*100/tmp1);
		totS += (double)tmp1 - (double)size;
		totT += tmp2 - (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
		totM += ((double)tmp1 - (double)size)*100/tmp1;
		
		/*file copyed
		close(src_fd);
		close(dst_fd);
		status = remove(filenames[i]);
		if (status != 0)  return 1;*/
		}  
			
	fprintf(file, "\nRisparmio tot = %f seconds,  %d bytes \n", totT, (int)totS);
	fprintf(file, "compressione media percentuale = %f, in più: %f % \n", (double)(((totB-totC)*100/(totB))), (double)totM/(double)NFILES);
				
	fclose(file);
	return 0;   
}
