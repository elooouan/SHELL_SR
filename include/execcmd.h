/* 
 *        Created by : Elie HATOUM and Elouan DURANTON-MAMADOU 
 *
 *   
 * execcmd.h - prototypes and definitions for the fcl shell - command exec 
 *
 */
/* $begin execcmd.h */
#ifndef __EXECCMD_H
#define __EXECCMD_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include "readcmd.h"
#include "csapp.h"


/* Commamd handling */
int command_to_code(char* cmd); /* Assigns a code to an internal command */
void built_in_command(struct cmdline *cmd, int i); /* Handles built-in commands like cd and quit */
void external_command(struct cmdline *cmd, int i); /* Executes an external command */
void execute_command(struct cmdline *cmd, int i, int background); /* Executes a command */
int count_commands(struct cmdline* cmd); /* Counts the number of commands in seq */

/* Pipe handling */
void create_pipes(int pipes[][2], int number_cmds); /* Creates n pipes for inter-process communication */
void close_pipes(int pipes[][2], int number_cmds); /* Closes n-1 pipes */
void pipeline_handler(struct cmdline* cmd, int number_cmds, int background); /* Handles execution of multiple commands in a pipeline */

/* Sequence handling */
void sequence_handler(struct cmdline* cmd); /* Handles execution of a sequence of commands */

/* I/O redirection (< >)*/
void input_redirection(char* in); /* Handles input redirection */
void output_redirection(char* out); /* Handles output redirection */

/* Wait for n child processes to die */
void wait_all(int number_cmds, int background);

#endif