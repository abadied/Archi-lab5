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
	int* status = 0;
	pid = fork();
	if(pid == -1 && debug){
		perror("There was an error in fork \n");
		fprintf(stderr,"Pid: %d \n",pid);
	}
	else if(pid == 0){
		int result = execvp(pCmdLine->arguments[0],pCmdLine->arguments);
		if(!result && debug){
			perror("error happened!!");
			_exit(result);

		}
		else if(debug){
			fprintf(stderr,"Pid: %d \n",pid);
		}
		exit(0);
	}
	if(pCmdLine->blocking == 1)
		waitpid(pid,&status,0);
}

void signalHandler(int signal){
	char*  sig = strsignal(signal);
	printf("signal caught:\n");
	printf("%s",sig);
	printf("\n");

}

void changeDir(cmdLine* cmd){
	int retval;
	char dir[PATH_MAX];
	retval = chdir(cmd->arguments[1]);
	if(retval == -1){
		perror("error ocuured in chdir \n");
	}
	else{
		printf("current directory: \n");
		getcwd(dir, PATH_MAX);
		printf(dir,PATH_MAX);
		printf("\n");
	}
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
		if(strcmp(cmd->arguments[0],"cd") == 0){
			changeDir(cmd);
			freeCmdLines(cmd);
		}
		else{
			execute(cmd);
			freeCmdLines(cmd);
		}
		fgets(buffer,2048,stdin);
		cmd = parseCmdLines(buffer);
	}
	return 0;
}

