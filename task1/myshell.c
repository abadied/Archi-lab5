#include "LineParser.h"
#include <stdlib.h>
#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

int debug = 0;

void execute(cmdLine *pCmdLine){
	int pid;
	pid = fork();
	if(pid == -1 && debug){
		perror("There was an error in fork \n");
	}
	else if(pid == 0){
		int result = execvp(pCmdLine->arguments[0],pCmdLine->arguments);
		if(!result && debug){
			perror("error happened!!");

		}
		exit(0);
	}
	wait();
}

void signalHandler(int signal){
	char*  sig = strsignal(signal);
	printf("signal caught:\n");
	printf("%s",sig);
	printf("\n");

}

int main(int argc,char** argv){
	/*check if debug mode*/
	if(argc > 1){
		if(strcmp(argv[1],"-d") == 0)
			debug = 1;
	}

	char buffer[2048];
	getcwd(buffer, PATH_MAX);
	printf(buffer,PATH_MAX);
	printf("\n");
	fgets(buffer,2048,stdin);
	cmdLine* cmd = parseCmdLines(buffer);
	while(strcmp(cmd->arguments[0],"quit") != 0){
		signal(SIGQUIT,signalHandler);
		signal(SIGTSTP,signalHandler);
		signal(SIGCHLD,signalHandler);
		execute(cmd);
		free(cmd);
		fgets(buffer,2048,stdin);
		cmd = parseCmdLines(buffer);
	}
	return 0;
}

