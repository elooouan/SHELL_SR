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
	Signal(SIGTSTP, handler_sigtstp);

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

		/* If entered command is "quit" then terminate shell */
		if (l->seq[0] && command_to_code(l->seq[0][0]) == 1) execute_command(l, 0);

		/* Execution  */
		sequence_handler(l);
	}
}
