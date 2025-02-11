#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include "readcmd.h"
#include "execcmd.h"
#include "csapp.h"


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
			if (cmd->in) {
				int fd = Open(cmd->in, O_RDONLY, 00400);
				if (fd == -1) {
					perror("open");
			} else {
				Dup2(fd, 0);
				Close(fd);
			}
		}
		if (cmd->out) {
			int fd = Open(cmd->out, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd == -1) {
				perror("open");
			} else {
				Dup2(fd, 1);
				Close(fd);
			}
		
		}
		if (execvp(args[0], args) == -1) {
			perror("execvp");
			exit(1);
		}

		} else {
			wait(NULL);
		}
	}
}