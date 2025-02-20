/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "execcmd.h"
#include "handlers.h"
#include "csapp.h"

int main()
{
	/* Signal Handling */
	Signal(SIGCHLD, handler_sigchild);
	Signal(SIGINT, handler_sigint);

	printf( "⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⣾⣗⣷⢶⣾⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣾⣿⣷⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀\n"
			"⠀⠀⠀⠀⠀⢀⣤⣶⠾⠻⠛⠦⠀⢙⠿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⠋⠘⠦⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡆⠙⢄⠀⠀⠀⢠⠿⢾⣿⠳⢤⡀⠀⠀\n"
			"⠀⠀⢀⣤⠔⣿⡿⡿⡤⠀⠀⢀⡤⠋⢠⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣿⡏⣀⣀⣤⣀⠈⣽⠦⠀⠀⠀⠀⠀⠀⠀⠀⠀⣶⣿⣿⡀⠀⠳⣆⠀⣠⢾⣻⣿⣿⣷⣿⣄⠀\n"
			"⠀⣠⣾⣿⣿⣿⣛⠷⣄⠀⣴⠟⠁⣠⡾⡵⣶⡀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠖⣿⡿⠟⣉⣉⠛⠸⣟⠻⠖⠀⠀⠀⠀⠀⣠⡴⠊⢉⣼⣇⡿⡆⠀⠘⣿⣷⢟⣽⣶⣾⣿⣿⢿⡄\n"
			"⠀⡿⣿⣿⣷⣶⣯⡻⣞⣿⠋⠀⢰⢿⣸⣧⡈⠉⠢⢀⠀⠀⠀⠀⠴⠯⣵⣻⠿⠂⠙⠛⠛⠏⠽⣈⡓⢦⣀⠀⠀⠀⡾⠋⠀⠀⣿⣿⣿⣧⣽⡆⠀⣿⣿⢼⣿⣿⣿⣿⣿⢘⡇\n"
			"⠀⡇⣿⣿⣿⣿⣿⡷⣿⣿⢀⣶⣿⣼⣿⣿⡷⠀⠀⠈⢦⠀⠀⠀⣠⠴⠋⠀⠠⣶⠾⡶⣤⠀⠘⠓⠛⠯⠿⣟⡀⠀⡇⠀⠀⠀⢹⣿⣿⣿⠿⣷⡀⠻⣽⣮⣙⣛⣻⠿⠋⣻⡇\n"
			"⢸⣿⠈⠻⣿⣛⣛⣵⣿⡿⢂⡾⠿⣿⣿⣿⡇⠀⠀⠀⢀⠀⠠⠾⠗⣿⠭⠟⠀⠀⠀⠉⠙⠃⢀⣤⣘⣿⠯⡍⠀⠀⢷⡄⠀⠀⠈⣿⡯⠔⠒⠈⣿⡄⢹⣇⡀⢡⣄⡀⢸⡏⠀\n"
			"⠀⢸⡆⢀⣠⣄⢀⣸⡏⢀⣾⠀⠐⠒⢿⣿⠀⠀⠀⢀⠈⠀⢀⡶⢚⣁⡀⠘⣷⢶⣴⣲⠀⠀⠙⠚⠻⢼⡗⠿⠶⠄⠀⢿⡀⠀⢀⡿⢶⣄⣠⣴⣿⢷⣄⣿⢽⣿⣿⣿⣿⡇⠀\n"
			"⠀⢸⣿⣿⣿⣿⡿⣿⣀⡾⣷⣶⣤⣠⡶⠿⡀⠀⢀⡞⠀⠛⣛⣚⡭⣞⣣⠤⢤⠀⠀⠀⠀⢰⠒⠦⠤⠤⣭⣳⢤⡀⠀⠈⠣⣴⡏⠀⠀⠈⠁⠀⠘⢾⣿⣿⣮⠭⣽⣿⣿⠇⠀\n"
			"⠀⠘⣿⣿⣯⠭⢽⣿⣿⡶⠏⠀⠈⠁⠀⠀⢸⣦⡼⠁⠀⠀⠉⠉⠉⠁⠀⠀⣸⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠈⠙⠛⠀⠀⠀⣿⠉⢀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⣿⣿⡏⠀⠀\n"
			"⠀⠀⢹⣿⣿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⣀⠉⣿⠀⠀⠀⠀⠀⠀⠀⢀⣠⣾⠵⠒⠲⢦⣠⢬⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡇⠀⠉⠑⠒⠒⠒⠚⣻⣻⣿⣿⣿⣿⡗⠀⠀\n"
			"⠀⠀⢸⣿⣿⣿⣿⣟⣟⠒⠒⠒⠒⠂⠉⠀⢰⡏⠀⠀⠀⠀⠀⠀⠐⠋⠁⠀⠀⠀⠀⠀⠉⠃⠈⠙⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⡤⢀⡀⠀⠀⠀⠀⣿⣿⣿⣿⣿⡟⠁⠀⠀\n"
			"⠀⠀⠀⠻⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⡀⠠⠟⠀⠀⠀⠀⠀⠀⠀                       ⠱⡄⠈⠉⠉⠉⠉⠉⢩⣷⡿⠋⠀⠀⠀⠀\n"
			"⠀⠀⠀⠀⠈⢿⣾⡍⠉⠉⠉⠁⠉⠁⢀⠎⠀⠀                         ⠀⠀⠀ ⠘⠦⣀⣀⣀⣀⣀⣼⠟⠁       \n"
			" _       __     __                             __           __         __                               \n"
			"| |     / /__  / /________  ____ ___  ___     / /_____     / /   ___  / /_  ____ _____  ____  ____      \n"
			"| | /| / / _ \\/ / ___/ __ \\/ __ `__ \\/ _ \\   / __/ __ \\   / /   / _ \\/ __ \\/ __ `/ __ \\/ __ \\/ __ \\     \n"
			"| |/ |/ /  __/ / /__/ /_/ / / / / / /  __/  / /_/ /_/ /  / /___/  __/ /_/ / /_/ / / / / /_/ / / / /     \n"
			"|__/|__/\\___/_/\\___/\\____/_/ /_/ /_/\\___/   \\__/\\____/  /_____/\\___/_.___/\\__,_/_/ /_/\\____/_/ /_/      \n\n");


	while (1) {
		struct cmdline *l;


		/* Analysis */
		printf("ᓚᘏᗢ ");

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
		if (l->seq[0] && command_to_code(l->seq[0][0]) == 1) execute_command(l, 0); /* WARNING: cannot use "quit" inside of a pipe -> if you want to do so add "quit" inside of execcmd and use kill(ppid (shell pid), SIGKILL)*/

		// more bugs are -> gedit & and then gedit, cd tests & then multiple quits
		// gedit & and then jbasjabsdlsad
		// cat res.txt | sort > res.txt

		/* Execution  */
		sequence_handler(l);

		/* DEBUGGING */
		/* Display each command of the pipe */
		// int i, j;

		// if (l->in) printf("in: %s\n", l->in);
		// if (l->out) printf("out: %s\n", l->out);

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
