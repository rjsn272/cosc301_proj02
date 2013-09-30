#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
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
	printf("%s\n","testing");
	while (fgets(buffer, 1024, stdin) !=NULL) {
		printf("%s\n","1");
		token = strtok(buffer, delim);
		printf("%s\n","2");
		while (token!=NULL && comment!=1){ //itterate the line sepearted by ;
						printf("%s\n",token);
			if (strchr(token,"#") ==0){ //if comment, break
					comment = 1;
					printf("%s\n","HERE!!!");
			}		

			token = strtok(NULL, delim);
		}
	}

	return 0;
}

