/* 
 *        Created by : Elie HATOUM and Elouan DURANTON-MAMADOU 
 */

/**
 * @file execcmd.h
 * @brief Prototypes and definitions for the shell's command execution functions.
 * 
 * This file contains function declarations for handling built-in and external commands, 
 * including input/output redirection, and managing processes in the shell.
 */

#ifndef __EXECCMD_H
#define __EXECCMD_H

#include <stddef.h>
#include <limits.h>
#include "jobs.h"

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
* @return If it's an external command, returns 0.
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
 * @brief Closes all the pipes.
 *
 * This function closes the file descriptors for all pipes used in a sequence of commands.
 *
 * @param pipes The array of pipes to close.
 * @param number_cmds The number of commands in the sequence.
 */
void close_pipes(int pipes[][2], int number_cmds);

/**
 * @brief Handle a pipeline of commands.
 *
 * This function sets up and executes a pipeline of multiple commands. It creates the required
 * pipes for inter-process communication, forks a child process for each command in the pipeline,
 * and redirects input and output as necessary. The first command handles input redirection if
 * specified, while the last command handles output redirection. Intermediate commands receive their
 * input from the previous command's pipe and send output to the current command's pipe.
 *
 * In the child processes, the process group is set so that all commands in the pipeline belong to
 * the same group, enabling proper job control. After setting up redirections, each child process
 * executes its corresponding command.
 *
 * If the background flag is non-zero, the pipeline is added to the job list as a background job.
 * Otherwise, the function waits until the foreground job completes.
 *
 * @param cmd Pointer to the command line structure containing the command sequence and redirection info.
 * @param number_cmds The total number of commands in the pipeline.
 * @param background Non-zero if the pipeline should execute in the background, zero if it should run in the foreground.
 */
void pipeline_handler(struct cmdline* cmd, int number_cmds, int background);

/**
 * @brief Handle and execute a sequence of commands.
 *
 * This function processes the command line structure.
 * - If no command is provided it returns immediately.
 * - For a single command, it distinguishes between built-in and external commands:
 *   - If it is a built-in command, it applies input and output redirection (if specified) and executes it directly.
 *   - For an external command, it forks a new process, sets the process group, applies redirections,
 *     and then executes the command. In the parent process, if the command is marked to run in the background,
 *     the job is added to the job list; otherwise, it is managed as a foreground job and the shell waits for its completion.
 * - If there are multiple commands, it delegates execution to the \ref pipeline_handler function.
 *
 * @param cmd Pointer to the command line structure that contains the command sequence, redirection info,
 *            and the background execution flag.
 */
void sequence_handler(struct cmdline* cmd);

#endif