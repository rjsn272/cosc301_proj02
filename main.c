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
	char* cmt = "#";
	pid_t pid;
	int seq = 0;
	int numcommands = 0;
	int i = 0;
	printf("%s","type here:  ");
	fflush(stdout);
	char buffer[1024];
	char* token;
	char* cmttoken;
	char *cmd[] = { "/bin/ls", "-ltr", ".", NULL };
	while (fgets(buffer, 1024, stdin) !=NULL) {
		cmttoken = strtok(buffer, cmt); //Nothing included after first '#'
		numcommands = 0;
		for(i=0;cmttoken[i]!=NULL;i++){
			if (cmttoken[i]==';')
				numcommands++;
		}
		i=0;
		char **commands[numcommands+1];
		token = strtok(cmttoken, delim); //cmttoken is strictly the non-commented code
		commands[i]=token;
		//assume parallel unless user switches out
		//if not one of our speciall instructions, fork
		//in fork, update the cmd array to be token (user instruction)
		//execv
		while (token!=NULL){ //itterate the line sepearted by ;
			printf("%s","current token:  ");
			printf("%s\n",token);
			//if exit, exit
			if (strncmp(token,"exit\n",4)==0) {
				exit(0);
			}

			//mode sequential, call new function
			else if (strncmp(token,"mode sequential",14)==0) {
				seq = 1;
			}
			else if (strncmp(token,"mode s",6)==0) {
				seq = 1;
			}

			//if mode, print mode type 
			//mode parallel, call new function
			/*if (strncmp(token,"mode parallel", 13)==0) {
				parallel(buffer);
			}
			else if (strncmp(token,"mode p",6)==0) {
				parallel();
			}*/	
			else {
				if ((pid=fork())==-1) { //if fork fail, error
					perror("Fork ERROR");
				}
				if (pid==0) { //if child, execv
					printf("%s\n","I Forked");
					strcpy(cmd[1],token);
					execv(cmd[0],cmd);
				}
			}
			i++;
			token = strtok(NULL, delim); //next token in line
			commands[i]=token;
		}
		if (seq == 1) { //if sequential acctivated, go to sequential
			sequential();
		}
		printf("%s","type here:  ");
		fflush(stdout);		
	}

	return 0;
}

void sequential (char* token) { //use if sequential is called
	printf("%s\n","mode sequential indeed");

	
}

void parallel (char* currentLine) { //use if parallel is called

	printf("%s\n","mode parallel indeed");
	
}
