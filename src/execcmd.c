#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include "readcmd.h"
#include "execcmd.h"


void execute_command(struct cmdline *cmd)
{
	if (!cmd->seq[0]) return; 

	char **args = cmd->seq[0];

	if (strcmp(args[0], "quit") == 0) {
		exit(0);
	} else if (strcmp(args[0], "cd") == 0) {
		if (args[1]) {
			if (chdir(args[1]) < 0) {
				perror("cd");
			}
		} else {
			fprintf(stderr, "cd: argument manquant\n");
		}
	} else {
		pid_t pid = fork();
		if (pid < 0) {
			perror("fork");
		} else if (pid == 0) {
			if (execvp(args[0], args) == -1) {
				perror("execvp");
				exit(1);
			}
		} else {
			wait(NULL);
		}
	}
}