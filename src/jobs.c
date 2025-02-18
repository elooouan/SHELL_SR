#include "jobs.h"

int nbJobs = 0;

Jobs *job_list = NULL;

/* Function to be able to print the enum */
char* stateToString(State state)
{
    char* res;

    switch (state) {
        case STOPPED:
            res = "Stopped";
            break;
        case RUNNING:
            res = "Running";
            break;
        case DONE:
            res = "Done";
            break;
    }

    return res;
}

char* copy_cmdline(struct cmdline* cmd) {
    // Copie uniquement le premier token de la première commande
    if (cmd->seq[0] && cmd->seq[0][0]) {
        return strdup(cmd->seq[0][0]); // alloue et copie la chaîne
    }
    return strdup("");
}


/* Function to add job to joblist */
void add_job(int pgid, struct cmdline* cmd)
{
    Jobs *new_job = malloc(sizeof(Jobs));

    new_job->id = ++nbJobs; // need to double check
    new_job->pgid = pgid;
    new_job->cmd = copy_cmdline(cmd);
    new_job->state = RUNNING;
    new_job->next = job_list;

    job_list = new_job;

    printf("Nbjobs : %d\n", nbJobs);
}

/* Function to print to list of jobs */
void print_jobs()
{
    Jobs *job_copie = job_list;



    while (job_copie) { 
        printf("[%d] %s %s %s\n", job_copie->id, job_copie->id == nbJobs ? "+" : "-", stateToString(job_copie->state), job_copie->cmd); /* has to be seq[0] */
        job_copie = job_copie->next;
    }
}

