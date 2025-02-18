#include "jobs.h"

int nbJobs = 0;

Jobs *job_list = NULL;

/* Function to add job to joblist */
void add_job(int pgid, struct cmdline* cmd)
{
    Jobs *new_job = malloc(sizeof(Jobs));

    new_job->id = nbJobs++; // need to double check
    new_job->pgid = pgid;
    new_job->cmd = cmd;
    new_job->state = RUNNING;
    new_job->next = job_list;

    job_list = new_job;
}

