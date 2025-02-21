/**
 * @file job.h
 * @brief Header file for managing jobs.
 */

#ifndef __JOBS_H
#define __JOBS_H

#include "execcmd.h"

/**
 * @brief Enumeration representing the state of a job.
 */
typedef enum {
    STOPPED, /**< Job is stopped. */
    RUNNING, /**< Job is running. */
    DONE     /**< Job is done. */
} State;

/**
 * @brief Global variable to keep track of the number of jobs.
 */
extern int nbJobs;

/**
 * @brief Global list of jobs.
 */
extern struct Jobs *job_list;

/**
 * @struct Jobs
 * @brief Structure representing a job.
 *
 * This structure holds information about a job, including its ID, process group ID,
 * command, state, and a pointer to the next job in the list.
 */
typedef struct Jobs {
    int id;       /**< Unique ID of the job. */
    pid_t pgid;   /**< Process group ID of the job. */
    char* cmd;    /**< Command associated with the job. */
    State state;  /**< State of the job. */
    struct Jobs *next; /**< Pointer to the next job in the list. */
} Jobs;

/**
 * @brief Global variable to keep track of the number of jobs.
 */
extern int nbJobs;

/**
 * @brief Global list of jobs.
 */
extern Jobs* job_list;

/**
 * @brief Converts a job state to a string representation.
 *
 * @param state The state of the job.
 * @return A string representation of the job state.
 */
char* stateToString(State state);

/**
 * @brief Deep copies the command line.
 *
 * @param cmd The command line structure to copy.
 * @return A deep copy of the command line string.
 */
char* copy_cmdline(struct cmdline* cmd);

/**
 * @brief Adds a new job to the job list.
 *
 * @param pgid The process group ID of the job.
 * @param cmd The command associated with the job.
 */
void add_job(pid_t pgid, char* cmd);

/**
 * @brief Removes a job from the job list by its process group ID.
 *
 * @param pgid The process group ID of the job to remove.
 */
void remove_job(pid_t pgid);

/**
 * @brief Prints the list of jobs.
 */
void print_jobs();

/**
 * @brief Prints jobs that are in the 'Done' state.
 *
 * @param pid The process ID of the job.
 */
void print_done_job(pid_t pid);

/**
 * @brief Gets the default (latest) job.
 *
 * @return The default job.
 */
Jobs* get_default_job();

/**
 * @brief Handles the 'fg' command to bring a job to the foreground.
 *
 * @param cmd The command line structure.
 */
void fg_command(struct cmdline* cmd, int id);

/**
 * @brief Handles the 'bg' command to send a job to the background.
 *
 * @param cmd The command line structure.
 */
void bg_command(struct cmdline* cmd, int id);

#endif