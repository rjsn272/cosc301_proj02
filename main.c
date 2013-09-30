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


int main(int argc, char **argv) {
	
	char* delim = ";";
	int comment = 0;
	printf("%s","type here:  ");
	fflush(stdout);
	char buffer[1024];
	char* token;
	while (fgets(buffer, 1024, stdin) !=NULL) {
		token = strtok(buffer, delim);
		while (token!=NULL && comment!=1){ //itterate the line sepearted by ;
			printf("%s\n",token);
			//if comment, break --NOT WORKING
			/*if (strchr(token,"#") ==0){ 
					comment = 1;
					break;
			}*/

			//mode parallel, call new function
			if (strncmp(token,"mode parallel", 13)==0) {
				parallel(buffer);
			}
			else if (strncmp(token,"mode p",6)==0) {
				parallel();
			}

			//mode sequential, call new function
			else if (strncmp(token,"mode sequential",14)==0) {
				sequential();
			}
			else if (strncmp(token,"mode s",6)==0) {
				sequential();
			}			
	
			
			//if exit, exit
			else if (strncmp(token,"exit\n",4)==0) {
				printf("%s\n","test");
				exit(0);
			}
			else {

				//run command
			}

			token = strtok(NULL, delim); //next token in line
		}
		printf("%s","type here:  ");
		fflush(stdout);		
	}

	return 0;
}

void sequential (char* currentLine) { //use if sequential is called

	printf("%s\n","mode sequential indeed");
}

void parallel (char* currentLine) { //use if parallel is called

	printf("%s\n","mode parallel indeed");
}
