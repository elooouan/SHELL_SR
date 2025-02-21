/* 
 *        Created by : Elie HATOUM and Elouan DURANTON-MAMADOU 
 *
 *   
 * execcmd.h - prototypes and definitions for the fcl shell - command exec 
 *
 */

/**
 * @file execcmd.h
 * @brief Prototypes and definitions for the fcl shell's command execution functions.
 * 
 * This file contains function declarations for handling built-in and external commands, 
 * including input/output redirection, and managing processes in the shell.
 */

#ifndef __EXECCMD_H
#define __EXECCMD_H

#include <stddef.h>
#include <limits.h>
#include "readcmd.h"
#include "csapp.h"

/**
 * @brief Converts a command string to its corresponding command code.
 *
 * This function checks the given command and returns an integer code based on the command.
 * Valid commands and their respective codes:
 * - quit: 1
 * - cd: 2
 * - kill: 3
 * - jobs: 4
 * - fg: 5
 * - bg: 6
 *
 * @param cmd The command string.
 * @return The integer code for the given command.
 */
int command_to_code(char* cmd);

/**
 * @brief Handles built-in commands like quit, cd, kill, jobs, fg, and bg.
 *
 * This function will check the command code and perform the appropriate action.
 * For example, it exits the program on "quit", changes directories on "cd", 
 * prints jobs on "jobs", and manages foreground or background processes on "fg" and "bg".
 *
 * @param cmd The command line struct containing the sequence of commands.
 * @param i The index of the command to execute.
 */
void built_in_command(struct cmdline *cmd, int i);

/**
 * @brief Executes an external command using execvp.
 *
 * This function is used when a command is not built-in and is executed externally.
 *
 * @param cmd The command line struct containing the sequence of commands.
 * @param i The index of the command to execute.
 */
void external_command(struct cmdline *cmd, int i);

/**
 * @brief Handles input redirection for a command.
 *
 * This function redirects the input stream (stdin) of a command from a file.
 *
 * @param in The input file to read from.
 */
void input_redirection(char* in);

/**
 * @brief Handles output redirection for a command.
 *
 * This function redirects the output stream (stdout) of a command to a file.
 *
 * @param out The output file to write to.
 */
void output_redirection(char* out);

/**
 * @brief Executes a command in the shell.
 *
 * This function decides whether to execute a built-in or external command based on the input.
 *
 * @param cmd The command line struct containing the sequence of commands.
 * @param i The index of the command to execute.
 */
void execute_command(struct cmdline *cmd, int i);

/**
 * @brief Counts the number of commands in a sequence.
 *
 * This function counts the number of individual commands in the sequence of commands.
 *
 * @param cmd The command line struct containing the sequence of commands.
 * @return The number of commands in the sequence.
 */
int count_commands(struct cmdline* cmd);

/**
 * @brief Creates pipes for a sequence of commands.
 *
 * This function creates a set of pipes to handle inter-process communication between commands in a pipeline.
 *
 * @param pipes The array of pipes to create.
 * @param number_cmds The number of commands in the sequence.
 */
void create_pipes(int pipes[][2], int number_cmds);

/**
 * @brief Closes all the pipes after the commands have been executed.
 *
 * This function closes the file descriptors for all pipes used in a sequence of commands.
 *
 * @param pipes The array of pipes to close.
 * @param number_cmds The number of commands in the sequence.
 */
void close_pipes(int pipes[][2], int number_cmds);

/**
 * @brief Waits for all child processes to finish if the command is executed in the foreground.
 *
 * This function waits for all child processes to finish if the commands are executed in the foreground.
 *
 * @param number_cmds The number of commands in the sequence.
 * @param background Whether the commands are executed in the background or foreground.
 */
void wait_all(int number_cmds, int background);

/**
 * @brief Handles and connects multiple pipes for a pipeline of commands.
 *
 * This function is used to manage a pipeline of commands by creating pipes and handling the 
 * input/output redirection between the commands.
 *
 * @param cmd The command line struct containing the sequence of commands.
 * @param number_cmds The number of commands in the sequence.
 * @param background Whether the commands are executed in the background or foreground.
 */
void pipeline_handler(struct cmdline* cmd, int number_cmds, int background);

/**
 * @brief Handles the sequence of commands, including piping and redirection.
 *
 * This function handles a sequence of commands, including piping between them and handling input/output redirection.
 *
 * @param cmd The command line struct containing the sequence of commands.
 */
void sequence_handler(struct cmdline* cmd);

#endif