#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include "readcmd.h"
#include "execcmd.h"
#include "csapp.h"

/* Function to assign a code to a given command */
int command_to_code(char* cmd) {
	int res;
	if (strcmp(cmd, "quit") == 0) res = 1;
	else if (strcmp(cmd, "cd") == 0) res = 2;
	else res = 0;
	return res;
}

/* Function to handle built in commands */
void built_in_command(struct cmdline *cmd) {
	char **args = cmd->seq[0];
	int code = command_to_code(args[0]);

	switch (code) {
		case 1:
			exit(0);
			break;
		case 2:
			if (args[1]) {
				if (chdir(args[1]) < 0) {
					perror("cd");
				}
			} else {
				fprintf(stderr, "cd: missing argument\n");
			}
			break;
	}
}

void external_command(struct cmdline *cmd){
	char **args = cmd->seq[0];
	pid_t pid = fork();

	if (pid < 0) {
		perror("fork");

	} else if (pid == 0) {
		if (cmd->in) input_redirection(cmd->in); /* If not null : name of file for input redirection. */
		if (cmd->out) output_redirection(cmd->out); /* If not null : name of file for output redirection. */

		if (execvp(args[0], args) == -1) {
			printf("%s: command not found\n", args[0]);
			exit(1);
		}
	} else {
		wait(NULL);
	}
	
}

/* Function to handle input redirections for a command  */
void input_redirection(char* in) {
	int fd = open(in, O_RDONLY);
	if (fd == -1) {
		perror(in);
		return;
	} else {
		Dup2(fd, 0);
		Close(fd);
	}
}

/* Function to handle output redirections for a command  */
void output_redirection(char* out) {
	int fd = Open(out, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1) {
		perror("open");
		return;
	} else {
		Dup2(fd, 1);
		Close(fd);
	}
}

void execute_command(struct cmdline *cmd)
{
	char** words = cmd->seq[0];
	if (!words) return; 
	
	if (command_to_code(words[0]) > 0) built_in_command(cmd);
	else external_command(cmd);
}