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



void list_insert(const char* path, struct node **head) {
	struct node *newnode = malloc(sizeof(struct node));
	char* pathFinal;
	pathFinal = strdup(path);
	newnode->path=pathFinal;

	newnode->next=*head;
	*head = newnode;
}


void list_insertPid(pid_t pid, struct node **head, const char* command) {
	struct node *newnode = malloc(sizeof(struct node));
	int pidFinal = (int)malloc(sizeof(pid));
	char* cmdFinal;
	cmdFinal = strdup(command);
	pidFinal = pid;
	newnode->pid=pidFinal;
	newnode->cmd = cmdFinal;

	newnode->next=*head;
	*head = newnode;
}

void readFile(struct node **head) {
	char* line;
	struct node *temp = NULL;
	temp=*head;
	size_t numberBytes=NULL;
	FILE *datafile;
	ssize_t read;
	datafile = fopen("shell-config.txt","r");
 	if (datafile == NULL) {
                printf("%s\n","Unable to open file");
                exit(-1);
        }
	while(read = getline(&line,&numberBytes, datafile)!=-1) { ///loop to read file 
		line[strlen(line)-1]='\0';
		list_insert(line,&temp);
		free(line);
		line=NULL;
	}
	*head=temp;
	fclose(datafile);
}

int main(int argc, char **argv) {
	//exit wont exit
	//pid print on completetion is 0 and not pid
	//prints prompt to often
	//need to add the following commands: jobs, paus PID, resume PID
	char* cmt = "#";
	char* cmdToPrint;
	int exitSwitch = (int)malloc(sizeof(int));
	exitSwitch=0;
	int seq = 1;
	int status;
	int i = 0;
	pid_t parentPid= (pid_t)malloc(sizeof(pid_t)); 
	int pidArray; 
	int run = 1;
	char buffer[1024];
	char* cmttoken;
	char **paths = (char **)malloc(sizeof(char**));
	char **linefinal= (char **)malloc(sizeof(char**));
	struct node *head = NULL;
	struct node *headPid = NULL;
	struct node *headPidTemp = NULL;
	pid_t pidStatus;
	readFile(&head);	//create linked list

	struct pollfd pfd;
	pfd.fd = 0;
	pfd.events = POLLIN;
	pfd.revents = 0;

	int rv = 0;
	

	printf("%s","prompt>  "); //print promot
	fflush(stdout);
	while (run) {
	rv = poll(&pfd, 1, 100);
	//while (fgets(buffer, 1024, stdin) !=NULL) {
		if (rv == 0) { //if no typing
			//printf("timeout\n");
		}
		else if (rv>0) { //if type something
			printf("%s\n",""); 
			fgets(buffer, 1024, stdin);
			pidArray = fork();
			list_insertPid(pidArray,&headPid,buffer);
				cmttoken = strtok(buffer, cmt); //Nothing included after first '#'
				linefinal = tokenify(cmttoken,";");
			if (pidArray<0) { //if fork fail, error
				perror("Fork ERROR"); //should we exit
			}	
			if (pidArray==0) { //CHILD
				//execute

				if (seq == 1) {


					sequential(linefinal,&head);
				}
				else{
					parallel(linefinal,&head);
				}

				//while loop to check exit and mode switch after everything has run, commented out exitSwitch
				i=0;
				while (linefinal[i]!=NULL){ //itterate the line sepearted by ;		
					if (strncmp(linefinal[i],"exit",4)==0) {
						exitSwitch = 1;
					}
					else if (strncmp(linefinal[i],"mode parallel",13)==0||strncmp(linefinal[i],"mode p",6)==0){
						seq = 0;
						printf("%s\n","YOU HAVE NOW ENTERED MODE PARALLEL");	
					}
					else if (strncmp(linefinal[i],"mode sequential",15)==0 || strncmp(linefinal[i],"mode s",6)==0){
						seq = 1;
						printf("%s\n","YOU HAVE NOW ENTERED MODE SEQUENTIAL");
					}
					else if (strncmp(linefinal[i],"mode",4)==0) {
						if (seq == 0) {
							printf("%s\n","Mode: Parallel");
						}
						if (seq == 1) {
							printf("%s\n","Mode:  Sequential");
						}
					}
					i++;
				}	
				if (exitSwitch==1){
					run=0;
				}
				printf("%s","prompt>  ");
			}
			//else { //parent	
				//do only once child ends
				/*i=0;
				while (linefinal[i]!=NULL){ //itterate the line sepearted by ;	
					printf("%s","test:   ");
					printf("%s\n",linefinal[i]);	
					if (strncmp(linefinal[i],"exit",4)==0) {
						exitSwitch = 1;
					}
					else if (strncmp(linefinal[i],"mode parallel",13)==0||strncmp(linefinal[i],"mode p",6)==0){
						seq = 0;
						printf("%s\n","YOU HAVE NOW ENTERED MODE PARALLEL");	
					}
					else if (strncmp(linefinal[i],"mode sequential",15)==0 || strncmp(linefinal[i],"mode s",6)==0){
						seq = 1;
						printf("%s\n","YOU HAVE NOW ENTERED MODE SEQUENTIAL");
					}
					else if (strncmp(linefinal[i],"mode",4)==0) {
						if (seq == 0) {
							printf("%s\n","Mode: Parallel");
						}
						if (seq == 1) {
							printf("%s\n","Mode:  Sequential");
						}
					}
					
					if (exitSwitch==1){
						run=0;
					}
					i++;
				}*/	
				//
				headPidTemp = headPid;
				while(headPid!=NULL) {	
					pidStatus = waitpid(headPid->pid, &status, WNOHANG); //parent wait, without hang
					if (pidStatus!=0) {
						printf("%s\n","");
						printf("%s","PROCESS: ");	
						strcpy(cmdToPrint,headPid->cmd);
						cmdToPrint[strlen(headPid->cmd)-1]='\0';//strip newline
						printf("%d",headPid->pid);
						printf("%s"," (");
						printf("%s",cmdToPrint);
						printf("%s\n",") COMPLETED");
						printf("%s","prompt> ");

						break;
					}
					if (headPid->next==NULL) {
						headPid=headPidTemp;
					}
					headPid=headPid->next;
				}
				headPid = headPidTemp;
			
			//}
			fflush(stdout);

		}
		else {
			//printf("there was some kind of error: %s\n",strerro(errno));
		}
	}
	free(linefinal);
	free(paths);
	free(parentPid);
	free(exitSwitch);
	free(cmdToPrint);
	return 0;
}

void sequential (char** linefinal,struct node **head) { //use if sequential is called			
		
	struct node *temp = NULL;
	temp=*head;
	int foundFile = -1;
	int status;
	int execute = 0;
	int i = 0;
	char* fileTempPath=(char *)malloc(sizeof(char*)*1024);
	struct stat statresult;
	char ** arrayToExec = NULL;
	pid_t pidArray[sizeof(linefinal)];
	while (linefinal[i]!=NULL){
		if (isBuiltIn(linefinal[i])==0){
			arrayToExec = tokenify(linefinal[i],"\n,\t, ");
			if (arrayToExec[0][0]=='/') { //if given path
				execute = 1;
			}
			else { //no path given
				foundFile = stat(arrayToExec[0],&statresult);
				//check if file exists
				while (foundFile==-1 && temp!=NULL) {
					fileTempPath[0]='\0';
					strcat(fileTempPath,temp->path);
					strcat(fileTempPath,"/");
					strcat(fileTempPath,arrayToExec[0]);
					foundFile = stat(fileTempPath,&statresult);
					temp = temp->next;
					if (foundFile==0) {
						execute = 1;
					}
				}
				printf("%s\n","FILE DOES NOT EXIST");
			}
			if (execute == 1) {
				pidArray[i] = fork();
				if (pidArray[i]<0) { //if fork fail, error
					perror("Fork ERROR"); //should we exit
				}	
				if (pidArray[i]==0) {
					if (execv(arrayToExec[0], arrayToExec) < 0) {
						fprintf(stderr, "execv failed: %s\n", strerror(errno));
						exit(0);
					}
				}
				else {
					waitpid(pidArray[i], &status, 0); 	//parent wait	
				}
			}
		}
		execute = 0;
		i++;
	}
	i = 0;

	free(fileTempPath);
	free(arrayToExec);
		
}

void parallel (char** linefinal, struct node **head) { //use if parallel is called
	int status;
	struct node *temp = NULL;
	temp=*head;
	int i = 0;
	int execute = 0;
	int foundFile = -1;
	char ** arrayToExec = NULL;
	struct stat statresult;
	char* fileTempPath=(char *)malloc(sizeof(char*)*1024);
	pid_t pidArray[sizeof(linefinal)];
	while (linefinal[i]!=NULL){

		if (isBuiltIn(linefinal[i])==0){
			arrayToExec = tokenify(linefinal[i],"\n,\t, ");
			if (arrayToExec[0][0]=='/') { //if given path
				execute = 1;
			}
			else { //no path given
				foundFile = stat(arrayToExec[0],&statresult);
				//check if file exists
				while (foundFile==-1 && temp!=NULL) {
					fileTempPath[0]='\0';
					strcat(fileTempPath,temp->path);
					strcat(fileTempPath,"/");
					strcat(fileTempPath,arrayToExec[0]);
					foundFile = stat(fileTempPath,&statresult);
					temp = temp->next;
					if (foundFile==0) {
						execute = 1;
					}
				}
				printf("%s\n","FILE DOES NOT EXIST");
			}
			if (execute ==1){

				pidArray[i] = fork();
				if (pidArray[i]<0) { //if fork fail, error
					perror("Fork ERROR"); //should we exit
				}	
				if (pidArray[i]==0) {
					if (execv(arrayToExec[0], arrayToExec) < 0) {
						fprintf(stderr, "execv failed: %s\n", strerror(errno));
						exit(0);
					}
				}
				else if (pidArray[i]>0) {
				}
			}

		}
		execute = 0;
		i++;
	}
	i = 0;
	for (;i<sizeof(pidArray);i++){	
		waitpid(pidArray[i], &status, 0); 	//parent wait	
	}
	free(fileTempPath);
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
	else if (strncmp(linefinal,"mode sequential",15)==0) {
		return 1;
	}
	else if (strncmp(linefinal,"mode s",6)==0) {
		return 1;
	}
	else if (strncmp(linefinal,"\n",2)==0) {
		return 1;
	}
	else if (strncmp(linefinal,"mode",4)==0){
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
