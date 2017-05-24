#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <linux/limits.h>
#include "LineParser.h"

#define MAXBUFFSIZE 2048

#define true 1
#define false 0

int execute(cmdLine *pCmdLine) {
  int result = execv(pCmdLine->arguments[0],pCmdLine->arguments);
  if (result) {
    perror("error in execute");
    exit(EXIT_FAILURE);
  }
  return result;
}

void sigHandler(int signum) {
  char* strsig = strsignal(signum);
  printf("signal caught and ignored: %s\n", strsig);
}

int main(int argc, char** argv) {
  char readbuffer[MAXBUFFSIZE];
  char path[PATH_MAX];

  getcwd(path, PATH_MAX);

  signal(SIGQUIT,sigHandler);
  signal(SIGTSTP,sigHandler);
  signal(SIGCHLD,sigHandler);

  while (true) {
    printf("%s$ ", path);
    fgets(readbuffer, MAXBUFFSIZE, stdin);

    cmdLine *line = parseCmdLines(readbuffer);
    if (!strcmp(line->arguments[0], "quit"))
      break;

    execute(line);
  }

  return 0;
}