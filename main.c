#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>
#include "proj02.h"

int main(int argc, char **argv) {
	char* cmt = "#";
	int exitSwitch = (int)malloc(sizeof(int));
	exitSwitch=0;

	int par = 0;
	int seq = 1;
	int i = 0;
	printf("%s","type here:  ");
	fflush(stdout);
	char buffer[1024];
	char* cmttoken;
	//char *cmd[] = { "/bin/ls", "-ltr", ".", NULL };
	char **linefinal= (char **)malloc(sizeof(char**));
	while (fgets(buffer, 1024, stdin) !=NULL) {
		cmttoken = strtok(buffer, cmt); //Nothing included after first '#'
		linefinal = tokenify(cmttoken); 

		//while loop to check exit and mode switch
		while (linefinal[i]!=NULL){ //itterate the line sepearted by ;
			if (strncmp(linefinal[i],"exit",3)==0) {
				exitSwitch = 1;
			}

			//mode parallel, call new function
			else if (strncmp(linefinal[i],"mode parallel",14)==0) {
				par = 1;
			}
			else if (strncmp(linefinal[i],"mode p",6)==0) {
				par = 1;
			}
			else if (strncmp(linefinal[i],"mode sequential",14)==0) {
				seq = 1;
			}
			else if (strncmp(linefinal[i],"mode s",6)==0) {
				seq = 1;
			}
			i++;
		}
		if (seq == 1) {
			sequential(linefinal);
		}
		if (par ==1){
			parallel();
		}


	}
	return 0;
}

void sequential (char** linefinal) { //use if sequential is called
	int i = 0;
	printf("%s\n","got to seq");
	pid_t pid;
	if ((pid=fork())<0) { //if fork fail, error
		perror("Fork ERROR"); //should we exit
	}
	//child
	else if (pid==0) { //if child, execv
		printf("%s\n","I Forked");
		while (linefinal[i]!=NULL){ //itterate the line sepearted by ;
			printf("%s","current token:  ");
			printf("%s\n",linefinal[i]);
			//ask Sommers if this if is OK
			if (strncmp(linefinal[i],"exit",3)==0) {
			}
			else if (strncmp(linefinal[i],"mode parallel",14)==0) {
			}
			else if (strncmp(linefinal[i],"mode p",6)==0) {
			}
			else{
				//execv(linefinal[i],linefinal);
				printf("%s","test   :");
				printf("%s\n",linefinal[i]);
			}
			i++;
		}

	}
	//parent
	else (pid>0) {
		int wc = wait(NULL); 	//parent wait
		printf("%s\n","got to parent");
	}
	
}

void parallel () { //use if parallel is called

	printf("%s\n","mode parallel indeed");
	
}

char **tokenify(const char *string) {
	const char *sep = ";";
	char *tmp, *word;
	char *s = strdup(string);
	int num_tokens = 0;
	for(word=strtok_r(s,sep,&tmp);word != NULL;word=strtok_r(NULL,sep,&tmp)){
		printf("%s\n",word);
		num_tokens++;		
	}
	
	char ** final = (char**)malloc((num_tokens+1) * sizeof(char*));
        int i = 0;

	for(word=strtok_r(s,sep,&tmp);word != NULL;word=strtok_r(NULL,sep,&tmp)){
                final[i++] = strdup(word);            
        }

	final[i] = NULL;
	free(s);

	return final;			
}
