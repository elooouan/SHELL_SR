/**
 * @file handlers.h
 * @brief Header file for signal handlers.
 */

#ifndef __HANDLER_H_
#define __HANDLER_H_

#include "jobs.h"

/**
 * @brief Signal handler for SIGCHLD.
 *
 * This function is invoked when a SIGCHLD signal is received, indicating that one or more child processes
 * have changed state. It enters a loop to reap all terminated child processes.
 * For each terminated child, it removes the job from the job and frees any associated foreground job resources.
 * If waitpid() returns -1 and the error is not ECHILD (indicating no more children exist), it reports the error
 * using unix_error().
 *
 * @param sig The signal number (expected to be SIGCHLD).
 */
void handler_sigchild(int sig);

/**
 * @brief Signal handler for SIGINT.
 *
 * This function is invoked when a SIGINT signal is received. It first checks if there is an active
 * foreground job. If one exists, it retrieves the head of the foreground job list, sends the SIGINT signal to the
 * entire process group of the foreground job and frees the foreground job list.
 *
 * @param sig The signal number (expected to be SIGINT).
 */
void handler_sigint(int sig);

/**
 * @brief Signal handler for SIGTSTP.
 *
 * This function is invoked when a SIGTSTP signal is received. It checks if there is an active
 * foreground job. If one exists, it retrieves the foreground job, sends the SIGTSTP
 * signal to the entire process group, and adds the job to the job list as a stopped job. 
 * The corresponding entry is removed from the foreground list.
 *
 * @param sig The signal number (expected to be SIGTSTP).
 */
void handler_sigtstp(int sig);

#endif