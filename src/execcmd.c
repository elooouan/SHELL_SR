#include "execcmd.h"
#include "handlers.h"
#include "jobs.h"
#include "foreground.h"

/* Function to assign a code to a given command */
int command_to_code(char* cmd) {
	int res;

	if (strcmp(cmd, "quit") == 0) res = 1;
	else if (strcmp(cmd, "cd") == 0) res = 2;
	else if (strcmp(cmd, "kill") == 0) res = 3;
	else if (strcmp(cmd, "jobs") == 0) res = 4;
	else if (strcmp(cmd, "fg") == 0) res = 5;
	else if (strcmp(cmd, "bg") == 0) res = 6;
	else res = 0;

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
			// if (!args[1] && cmd->background) printf("[job nb] : %d\n", getpid());
			if (args[1]) {
				if (chdir(args[1]) == -1) {
					perror("cd");
				}
			} else {
				fprintf(stderr, "cd: missing argument\n");
			}
			break;
		case 3:
			/* Need to fill */
			break;
		case 4:
			print_jobs();
			break;
		case 5:
			/* Handle fg */
			fg_command(cmd);
			break;
		case 6:
			/* handle bg */
			bg_command(cmd);
			break;
	}
}

/* Function to handle external commands */
void external_command(struct cmdline *cmd, int i) {
	char **args = cmd->seq[i];
	
	// if (cmd->background) fprintf(stderr,"\n[job nb] : %d\n", getpid());
	//Setpgid(0, 0);
	if (execvp(args[0], args) == -1) {
		printf("fclsh: command not found: %s\n", args[0]);
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

/* Function to wait for all child processes if in foreground*/
void wait_all(int number_cmds, int background)
{
	if (!background) {		
		for (int i = 0; i < number_cmds; i++) {
			wait(NULL); /* if error do nothing -> child process completed succesfully faster than shell */
		}
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
			if(i == 0){ /* if first child -> set pgid as pid*/
				Setpgid(0, 0);
			}else{ /* set brothers pgid as first child pid*/
				while (first_child_pid == 0);
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
			execvp(cmd->seq[i][0], cmd->seq[i]);
		} else { /* Parent */
			if (i == 0) { /* if first child -> save his pid*/
				first_child_pid = pid;
			}
			add_foreground(pid);
		}
	}
	
	close_pipes(pipes, number_cmds);
	wait_all(number_cmds, background);

	if (background) {
		add_job(first_child_pid, cmd);
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
	// printf("The process is in %s\n", background ? "Background" : "Foreground"); // -> remove just for debugging

	Signal(SIGCHLD, handler_sigchild);

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
					add_job(pid, cmd);
				}else{
					add_foreground(pid);
					while(foreground_list) sleep(1);
				}
			}
		}
	} else { /* Pipes */
		pipeline_handler(cmd, number_cmds, background);
	}
}

