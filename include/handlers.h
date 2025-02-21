/**
 * @file handlers.h
 * @brief Header file for signal handlers.
 */

#ifndef __HANDLER_H_
#define __HANDLER_H_

#include "jobs.h"

/**
 * @brief Handler for the SIGCHLD signal.
 *
 * This function handles the SIGCHLD signal by waiting for child processes
 * to change state and then performing necessary cleanup operations.
 *
 * @param sig The signal number (SIGCHLD).
 */
void handler_sigchild(int sig);

/**
 * @brief Handler for the SIGINT signal.
 *
 * This function handles the SIGINT signal by sending a SIGINT signal to the
 * foreground process group and performing necessary cleanup operations.
 *
 * @param sig The signal number (SIGINT).
 */
void handler_sigint(int sig);

/**
 * @brief Handler for the SIGTSTP signal.
 *
 * This function handles the SIGTSTP signal by sending a SIGTSTP signal to the
 * foreground process group, adding the job to the job list, and performing
 * necessary cleanup operations.
 *
 * @param sig The signal number (SIGTSTP).
 */
void handler_sigtstp(int sig);

#endif