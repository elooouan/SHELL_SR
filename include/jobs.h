#ifndef __JOBS_H
#define __JOBS_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include "readcmd.h"
#include "execcmd.h"
#include "csapp.h"

typedef enum{
    STOPPED,
    RUNNING,
    DONE
} State;

extern int nbJobs;
extern struct Jobs *job_list;

typedef struct Jobs {
    int id;
    pid_t pgid;
    struct cmdline* cmd;
    State state;
    struct Jobs *next;    
} Jobs; 

void add_job(pid_t pgid, struct cmdline* cmd);

#endif