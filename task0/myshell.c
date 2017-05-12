#include "LineParser.h"
#include <stdlib.h>
#include "linux/limits.h"
#include <unistd.h>
#include <stdio.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2



int main(int argc,char** argv){
	char* buffer[2048];
	getcwd(buffer, PATH_MAX);
	printf(buffer,PATH_MAX);
	printf("\n");
	fgets(buffer,2048,stdin);
	cmdLine* cmd_line = parseCmdLines(buffer);
	
	return 0;
}

