/**
 * @file job.h
 * @brief Header file for managing jobs.
 */

#ifndef __JOBS_H
#define __JOBS_H

#include "readcmd.h"
#include "foreground.h"

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
 * @brief Copies the command line.
 *
 * @param cmd The command line structure to copy.
 * @return A copy of the command line string.
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
 * @brief Bring a background / suspended job to the foreground.
 *
 * This function resumes a job by sending a SIGCONT signal to the process group associated with
 * the job. It updates the job's state to RUNNING, transfers it to the foreground, and waits until
 * the foreground job has completed. If the job was already running in the background,
 * it is transferred to the foreground.
 * If there are no jobs or if the given job id is invalid, an error message is displayed.
 *
 * @param cmd Pointer to the command line structure.
 * @param id The job identifier for the job to be transferred to the foreground.
 */
void fg_command(struct cmdline* cmd, int id);

/**
 * @brief Resume a suspended job in the background.
 *
 * This function resumes a job in the background by sending a SIGCONT signal to the process group
 * associated with the job. It sets the job's state to RUNNING and ensures that it remains in the
 * background. 
 * If no jobs are available or if the given job id is invalid, an error message is displayed.
 *
 * @param cmd Pointer to the command line structure.
 * @param id The job identifier for the job to be resumed in the background.
 */
void bg_command(struct cmdline* cmd, int id);

#endif