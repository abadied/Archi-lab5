#define _GNU_SOURCE
#include "LineParser.h"
#include <stdlib.h>
#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "JobControl.h"
#include <sys/wait.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2


int debug = 0;

void execute(cmdLine *pCmdLine){
	int pid;
	int* status = 0;

	switch (pid = fork()) {
		case -1:
		if (debug)
			perror("fork failed");
		break;
		case 0:
			if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)){
				perror("error in execution");
				_exit(EXIT_FAILURE);
			}
			_exit(EXIT_SUCCESS);
		break;
		default:
			if (debug)
				fprintf(stderr,"Executing Command: %s\nChild PID: %d\n",pCmdLine->arguments[0],pid);
			if (pCmdLine->blocking == 1)
				waitpid(pid, status, 0);
	}
}

void sigHandler(int signum) {
	if (debug) {
		char* strsig = strsignal(signum);
		printf("signal caught and ignored: %s\n", strsig);
	}
}

void changeDir(cmdLine* cmd){
	if(chdir(cmd->arguments[1])){
		perror("error ocuured in chdir \n");
	}
}
int main(int argc,char** argv){
	/*check if debug mode*/
	if(argc > 1){
		if(strcmp(argv[1],"-d") == 0)
			debug = 1;
	}

	signal(SIGQUIT,sigHandler);
	signal(SIGTSTP,sigHandler);
	signal(SIGCHLD,sigHandler);

	char buffer[2048];
	char path[PATH_MAX];

	getcwd(path, PATH_MAX);
    printf("%s$ ", path);

	fgets(buffer,2048,stdin);
	job* jobs_list = NULL;
	cmdLine* cmd = parseCmdLines(buffer);
	while(strcmp(cmd->arguments[0],"quit") != 0){
		job* curr_job = addJob(&jobs_list, buffer);
		
		if(strcmp(cmd->arguments[0],"cd") == 0){
			changeDir(cmd);
		}
		else if(strcmp(cmd->arguments[0],"jobs") == 0 && jobs_list != NULL){
			printJobs(&jobs_list);
		}
		else{
			execute(cmd);
			
		}
		
		freeCmdLines(cmd);
		
		getcwd(path, PATH_MAX);
    	printf("%s$ ", path);
    
		fgets(buffer,2048,stdin);
		cmd = parseCmdLines(buffer);
	}
	freeCmdLines(cmd);
	if(jobs_list!=NULL)
		freeJobList(&jobs_list);
	return 0;
}

