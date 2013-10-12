#ifndef __PROJ02_H__
#define __PROJ02_H__

struct node {
    const char* path;
	const char** cmd;
	pid_t pid;
	int working;
    struct node *next; 
};



void list_insert(const char* path, struct node **head);
void readFile(struct node **head);
void parallel(char ** linefinal, struct node **head);
void sequential(char** linefinal, struct node **head);
char **tokenify(const char *string, const char *delim);
int isBuiltInt(char* linefinal);

#endif // __PROJ02_H__
