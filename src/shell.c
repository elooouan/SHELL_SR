/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "execcmd.h"
#include "csapp.h"

int main()
{
	while (1) {
		struct cmdline *l;
		// int i, j;

		/* Analysis */
		printf("fclsh> ");
		l = readcmd();

		/* If input stream closed, normal termination */
		if (!l) {
			printf("exit\n");
			exit(0);
		}

		if (l->err) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
		}

		/* If command is "quit" then terminate shell */
		if (l->seq[0] && command_to_code(l->seq[0][0]) == 1) execute_command(l, 0, l->background); /* WARNING: cannot use "quit" inside of a pipe -> if you want to do so add "quit" inside of execcmd and use kill(ppid (shell pid), SIGKILL)*/

		// more bugs are -> gedit & and then gedit, cd tests & then multiple quits

		/* Execution  */
		sequence_handler(l);

		// if (l->in) printf("in: %s\n", l->in);
		// if (l->out) printf("out: %s\n", l->out);

		/* Display each command of the pipe */
		// for (i=0; l->seq[i]!=0; i++) {
		// 	char **cmd = l->seq[i];
		// 	printf("seq[%d]: ", i);
		// 	for (j=0; cmd[j]!=0; j++) {
		// 		printf("%s ", cmd[j]);
		// 	}
		// 	printf("\n");
		// }
	}
}
