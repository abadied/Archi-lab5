#include "LineParser.h"
#include <stdlib.h>
#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

int execute(cmdLine *pCmdLine){
	int result = execv(pCmdLine->inputRedirect,pCmdLine->arguments);
	if(!result){
		perror("error happened!!");

	}
	return result;
}

int main(int argc,char** argv){
	char buffer[2048];
	
	getcwd(buffer, PATH_MAX);
	printf(buffer,PATH_MAX);
	printf("\n");
	fgets(buffer,2048,stdin);
	cmdLine* cmd = parseCmdLines(buffer);
	while(strcmp(cmd->arguments[0],"quit") != 0){
		execute(cmd);
		free(cmd);
		fgets(buffer,2048,stdin);
		cmd = parseCmdLines(buffer);
	}
	return 0;
}

