/**
 * @file foreground.h
 * @brief Header file for managing foreground processes.
 */

#ifndef __FOREGROUND_H
#define __FOREGROUND_H

#include "csapp.h"

/**
 * @struct Foreground
 * @brief Structure representing a foreground process.
 *
 * This structure holds information about a foreground process, including its
 * process ID, process group ID, command, and a pointer to the next foreground
 * process in the list.
 */
typedef struct Foreground {
    pid_t pid;      /**< Process ID of the foreground process. */
    pid_t pgid;     /**< Process group ID of the foreground process. */
    char* cmd;      /**< Command associated with the foreground process. */
    struct Foreground *next; /**< Pointer to the next foreground process in the list. */
} Foreground;
/**
 * @brief Global list of foreground processes.
 */
extern Foreground *foreground_list;

/**
 * @brief Adds a new foreground process to the list.
 *
 * @param pid The process ID of the foreground process.
 * @param pgid The process group ID of the foreground process.
 * @param cmd The command associated with the foreground process.
 */
void add_foreground(pid_t pid, pid_t pgid, char* cmd);

/**
 * @brief Removes a foreground process from the list by its process group ID.
 *
 * @param pgid The process group ID of the foreground process to remove.
 */
void pop_foreground(pid_t pgid);

/**
 * @brief Frees the memory allocated for the foreground process list.
 */
void free_foreground();

/**
 * @brief Gets the head of the foreground process list.
 *
 * @return The head of the foreground process list.
 */
Foreground* get_foreground_head();

#endif