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

/* Assigns a code to an internal command */
int command_to_code(char* cmd);

/* Handles built-in commands like cd and quit */
void built_in_command(struct cmdline *cmd, int i);

/* Executes an external command */
void external_command(struct cmdline *cmd, int i);

/* Handles input redirection */
void input_redirection(char* in);

/* Handles output redirection */
void output_redirection(char* out);

/* Executes a command */
void execute_command(struct cmdline *cmd, int i);

/* Counts the number of commands in seq */
int count_commands(struct cmdline* cmd);

/* Creates n pipes for inter-process communication */
void create_pipes(int pipes[][2], int number_cmds);

/* Closes n-1 pipes */
void close_pipes(int pipes[][2], int number_cmds);

/* Wait for n child processes to die */
void wait_all(int number_cmds);

/* Handles execution of multiple commands in a pipeline */
void pipeline_handler(struct cmdline* cmd, int number_cmds);

/* Handles execution of a sequence of commands */
void sequence_handler(struct cmdline* cmd);

#endif