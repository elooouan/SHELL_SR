#include "execcmd.h"

/* Assign a code to a given command */
int command_to_code(char* cmd) {
	int res;
	
	 /* Built-in Commands > 0 */
	if (strcmp(cmd, "quit") == 0) res = 1;
	else if (strcmp(cmd, "cd") == 0) res = 2;
	else if (strcmp(cmd, "kill") == 0) res = 3;
	else if (strcmp(cmd, "jobs") == 0) res = 4;
	else if (strcmp(cmd, "fg") == 0) res = 5;
	else if (strcmp(cmd, "bg") == 0) res = 6;

	else res = 0; /* External Command */

	return res;
}

/* Function to handle built in commands */
void built_in_command(struct cmdline *cmd, int i) {
	char **args = cmd->seq[i];
	int code = command_to_code(args[0]);

	switch (code) {
		case 1:
			exit(0);
			break;
		case 2:
			if (args[1]) {
				if (chdir(args[1]) == -1) {
					perror("cd");
				}
			} else {
				fprintf(stderr, "cd: missing argument\n");
			}
			break;
		case 3:
			if (!args[1] || !args[2]) {
				fprintf(stderr, "kill : not enough arguments\n");
				break;
			}
			int pid = atoi(args[1]++);
			int sig = atoi(++args[2]);
			args[1]--;
			args[2]--;
			/* If user enter kill -pid -sig with "-" before pid -> sends sig to gpid of id pid */
			if (args[1][0] == '-') {
				/* Need to make sure sig is between 1 and 31 and that the pid is < to the shell id */
				if (pid == 0 || sig < 1 || sig > 31 || kill(pid, sig) == -1) fprintf(stderr, "kill: error\n");
			} else {
				pid = atoi(args[1]);
				if (pid == 0 || sig > 31 || sig < 1 || kill(pid, sig) == -1) fprintf(stderr, "kill: error \n");
			}
			break;
		case 4:
			print_jobs();
			break;
		case 5:
			/* Handle fg */
			if (!args[1]) fg_command(cmd, nbJobs); /* -> if no argument is passed we take the default job (latest) */
			else if(args[1] && args[1][0] == '%') fg_command(cmd, args[1][1] - '0'); /* fg %id */
			else fprintf(stderr, "fg: enter \'%%\' before the job id\n");
			break;
		case 6:
			/* Handle bg */
			if (!args[1]) bg_command(cmd, nbJobs); /* -> if no argument is passed we take the default job (latest) */
			else if(args[1] && args[1][0] == '%') bg_command(cmd, args[1][1] - '0'); /* fg %id */
			else fprintf(stderr, "bg: enter \'%%\' before the job id\n");
			break;
	}
}

/* Function to handle external commands */
void external_command(struct cmdline *cmd, int i) {
	char **args = cmd->seq[i];

	if (execvp(args[0], args) == -1) {
		fprintf(stderr, "ᓚᘏᗢ : command not found: %s\n", args[0]);
		exit(1);
	}
}

/* Function to handle input redirections for a command  */
void input_redirection(char* in) {
	int fd = open(in, O_RDONLY, 0);

	if (fd == -1) {
		perror(in);
		exit(1); // return
	} else {
		Dup2(fd, STDIN_FILENO);
		Close(fd);
	}
}

/* Function to handle output redirections for a command  */
void output_redirection(char* out) {
	int fd = Open(out, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	
	if (fd == -1) {
		perror("open");
		exit(-1); // return
	} else {
		Dup2(fd, STDOUT_FILENO);
		Close(fd);
	}
}

/* Our main function to run commands */
void execute_command(struct cmdline *cmd, int i)
{
	char** words = cmd->seq[i];
	
	if (command_to_code(words[0]) > 0) built_in_command(cmd, i);
	else external_command(cmd, i);

}

/* Function to count number of commands in seq */
int count_commands(struct cmdline* cmd)
{
	int number_cmds = 0;

	while (cmd->seq[number_cmds]) number_cmds++;

	return number_cmds;
}

/* Function to create n pipes */
void create_pipes(int pipes[][2], int number_cmds)
{
	for (int i = 0; i < number_cmds - 1; i++) { //  we need (number_cmds - 1) pipes
		if (pipe(pipes[i]) == -1) {
			perror("pipes");
			return;
		}
	}
}

/* Function to close all n-1 pipes */
void close_pipes(int pipes[][2], int number_cmds)
{
	for (int i = 0; i < number_cmds - 1; i++) {
		Close(pipes[i][0]);
		Close(pipes[i][1]);
	}
}

/* Function to handle and connect multiple pipes i.e. a pipeline */
void pipeline_handler(struct cmdline* cmd, int number_cmds, int background)
{
	int pipes[number_cmds - 1][2]; // Array of n pipes -> ... | ... | ... 

	pid_t first_child_pid = 0;

	create_pipes(pipes, number_cmds);

	for (int i = 0; i < number_cmds; i++) {
		pid_t pid = Fork();

		/* Child */
		if (pid == 0) {
			if (i == 0) { 
				/* if first child -> set pgid as pid*/
				Setpgid(0, 0);
			} else {
				/* set brothers pgid as first child pid*/
				// while (first_child_pid == 0);
				Setpgid(0, first_child_pid);
			}

			if (i == 0 && cmd->in) {
				input_redirection(cmd->in);
			} else if (i > 0) {
				Dup2(pipes[i-1][0], STDIN_FILENO); /* If it's not the first command -> read from the pipeline */
			}

			if (i == number_cmds - 1 && cmd->out) {
				output_redirection(cmd->out);
			} else if (i < number_cmds - 1) {
				Dup2(pipes[i][1], STDOUT_FILENO); /* If it's not the last command -> write into the pipeline */
			}
			
			close_pipes(pipes, number_cmds);
			execute_command(cmd, i);
		}
		/* Parent */
		else {
			if (i == 0) { /* if first child -> save his pid*/
				first_child_pid = pid;
			}
			char* command = copy_cmdline(cmd);
			add_foreground(pid, first_child_pid, command);
		}
	}
	
	close_pipes(pipes, number_cmds);

	if (background) {
		char* command = copy_cmdline(cmd);
		add_job(first_child_pid, command);
		print_jobs();
	}else{
		while(foreground_list) sleep (1);
	}
}

/* Function to handle piping -> multiple commands */
void sequence_handler(struct cmdline* cmd)
{
	int number_cmds = count_commands(cmd);
	int background = cmd->background;

	/* No command (Enter) */
	if (!number_cmds) return;
	else if (number_cmds == 1) { /* Single command */
		if (command_to_code(cmd->seq[0][0]) > 0) {
			if (cmd->in) input_redirection(cmd->in); /* If not null : name of file for input redirection. */
			if (cmd->out) output_redirection(cmd->out); /* If not null : name of file for output redirection. */

			execute_command(cmd, 0);
		} else {
			pid_t pid = Fork();
			
			/* Child */
			if (pid == 0) {
				Setpgid(0, 0); // man doesn't work

				if (cmd->in) input_redirection(cmd->in); /* If not null : name of file for input redirection. */
				if (cmd->out) output_redirection(cmd->out); /* If not null : name of file for output redirection. */
				execute_command(cmd, 0);
			} else {
				/* The Shell */
				if(background){
					char* command = copy_cmdline(cmd);
					add_job(pid, command);
				}else{
					char* command = copy_cmdline(cmd);
					add_foreground(pid, pid, command);
					while(foreground_list) sleep(1);
				}
			}
		}
	} else { /* Pipes */
		pipeline_handler(cmd, number_cmds, background);
	}
}

