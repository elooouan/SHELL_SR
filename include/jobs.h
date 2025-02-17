#ifndef __JOBS_H
#define __JOBS_H

#include "csapp.h"

typedef enum{
    STOPPED,
    RUNNING,
    DONE
} State;

typedef struct Processes {
    pid_t pid;
    struct Processes *next;
} Processes;

typedef struct Jobs {
    int id, nbJobs, background;
    Processes *process;
    State state;
    struct Jobs *next;    
} Jobs;

#endif