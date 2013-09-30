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

	printf("%s","type here:  ");
	fflush(stdout);
	char buffer[1024];
	while (fgets(buffer, 1024, stdin) !=NULL) {
		printf("%s\n","test");
	}

	return 0;
}

