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
#include <errno.h>

int main(int argc, char **argv) {
	char* cmt = "#";
	int exitSwitch = (int)malloc(sizeof(int));
	exitSwitch=0;

	int par = 1;
	int seq =0;
	int i = 0;
	printf("%s","type here:  ");
	fflush(stdout);
	char buffer[1024];
	char* cmttoken;
	//char *cmd[] = { "/bin/ls", "-ltr", ".", NULL };
	char **linefinal= (char **)malloc(sizeof(char**));
	while (fgets(buffer, 1024, stdin) !=NULL) {
		cmttoken = strtok(buffer, cmt); //Nothing included after first '#'
		linefinal = tokenify(cmttoken,";");
		i=0; 
		//printf("%s","Second:  ");
		//printf("%s\n",linefinal[1]);

		//while loop to check exit and mode switch
		while (linefinal[i]!=NULL){ //itterate the line sepearted by ;
			printf("%s","current token:  ");			
			printf("%s\n",linefinal[i]);			
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
			parallel(linefinal);
		}
		fflush(stdout);
		printf("%s","type here:  ");
	}
	return 0;
}

int sequential (char** linefinal) { //use if sequential is called
	int i = 0;
	int status;
	char **arrayToExec= NULL;
	printf("%s\n","got to seq");
	pid_t pid = fork();
	if (pid<0) { //if fork fail, error
		perror("Fork ERROR"); //should we exit
	}
	//child
	else if (pid==0) { //if child, execv
		printf("%s\n","I Forked");
		while (linefinal[i]!=NULL){ //itterate the line sepearted by ;

			arrayToExec = tokenify(linefinal[i],"\n,\t, ");
			printf("%s","current token:  ");
			printf("%s\n",arrayToExec[i]);
			//ask Sommers if this if is OK
			if (isBuiltIn(linefinal[i])==0) {
				if (execv(arrayToExec[0], arrayToExec) < 0) {
					fprintf(stderr, "execv failed: %s\n", strerror(errno));
			    	}
				printf("%s","test   :");
				printf("%s\n",linefinal[i]);
			}
			printf("%s\n","HERE!!!");
			i++;
		}
	}
	//parent
	else {
		printf("%d\n",1);
		waitpid(pid, &status, 0); 	//parent wait
		printf("%s\n","got to parent");
	}
	return 0;	
}

int parallel (char** linefinal) { //use if parallel is called
	int status;
	int i = 0;
	pid_t pid = fork();
	char ** arrayToExec = NULL;
	pid_t pidArray[sizeof(linefinal)];
	while (linefinal[i]!=NULL){
		arrayToExec = tokenify(linefinal[i],"\n,\t, ");
		pidArray[i] = fork();
		if (pidArray[i]<0) { //if fork fail, error
			perror("Fork ERROR"); //should we exit
		}	
		if (pidArray[i]==0) {
			if (execv(arrayToExec[0], arrayToExec) < 0) {
				fprintf(stderr, "execv failed: %s\n", strerror(errno));
			}
		}
		else {
			waitpid(pidArray[i], &status, 0); 	//parent wait
			printf("%s\n","got to parent");		
		}
		i++;
	}
	return 0;
	
}

int isBuiltIn(char* linefinal) {
	//char* linefinal = linefinal;
	if (strncmp(linefinal,"exit",3)==0) {
		return 1;
	}
	else if (strncmp(linefinal,"mode parallel",14)==0) {
		return 1;
	}
	else if (strncmp(linefinal,"mode p",6)==0) {
		return 1;
	}
	return 0;
}
char** tokenify(const char *str, const char *delim) {
	char *sep = strdup(delim);
	char *temp, *word;
	char *s = strdup(str);
	int counter = 0;
	for (word = strtok_r(s, sep, &temp); word != NULL; word = strtok_r(NULL, sep, &temp)) {
		counter++;		
	}
	free(s);
	counter++;
	char **array = (char**)malloc(sizeof(char*) * counter);
	int x = 0;
	char *temp1, *word1;
	char *s1 = strdup(str);
	
	for (word1 = strtok_r(s1, sep, &temp1); word1 != NULL; word1 = strtok_r(NULL, sep, &temp1)) {
		array[x]= strdup(word1);		
		x++;
	}	
	free(s1);
	array[x]=NULL;
	return array;
	
}
