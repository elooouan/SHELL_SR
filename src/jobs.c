#include "jobs.h"
#include "foreground.h"

int nbJobs = 0;

Jobs* job_list = NULL;

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

/* Function to deep copy the cmdline */
char* copy_cmdline(struct cmdline* cmd) {
    if (cmd->seq[0] && cmd->seq[0][0]) {
        return strdup(cmd->seq[0][0]);
    }
    return strdup("");
}

/* Function to add job to joblist */
void add_job(pid_t pgid, char* cmd)
{
    Jobs *new_job = malloc(sizeof(Jobs));

    new_job->id = ++nbJobs; // need to double check
    new_job->pgid = pgid;
    new_job->cmd = cmd;
    new_job->state = RUNNING;
    new_job->next = job_list;

    job_list = new_job;
}

/* Function to remove job from job list (pile) */
void remove_job(pid_t pgid)
{
    if (!job_list) return;

    Jobs *current = job_list, *prev = NULL;

    while (current && current->pgid != pgid) {
        prev = current;
        current = current->next;
    }

    if (!current) return;

    /* If found */
    int removed_id = current->id;

    if (!prev) job_list = current->next;
    else prev->next = current->next;

    free(current);
    nbJobs--;

    Jobs *iter = job_list;
    while (iter) {
        if (iter->id > removed_id) iter->id--;
        iter = iter->next;
    }
}

/* Function to print to list of jobs */
void print_jobs()
{
    Jobs *job_temp = job_list;

    while (job_temp) { 
        printf("[%d] %s %s %s\n", job_temp->id, job_temp->id == nbJobs ? "+" : (job_temp->id == nbJobs - 1 ? "-" : " "), stateToString(job_temp->state), job_temp->cmd);
        job_temp = job_temp->next;
    }
}
 
 /* Function to print jobs that are in the 'Done* state */
void print_done_job(pid_t pid)
{
    Jobs* job_temp = job_list, *current = job_list;

    while(current != NULL && current->pgid != pid){
        current = current->next;
    }

    if(current){
        printf("[%d] %s %s %s\n", job_temp->id, job_temp->id == nbJobs ? "+" : (job_temp->id == nbJobs - 1 ? "-" : " "), stateToString(DONE), job_temp->cmd);
    }
}

/* Function to return the default (latest) job */
Jobs* get_default_job()
{
    return job_list;
}

/* Function to handle the fg command */
void fg_command(struct cmdline* cmd)
{
    Jobs* job = get_default_job();
    pid_t pgid = job->pgid;


    job->state = RUNNING;
    kill(-pgid, SIGCONT);
    cmd->background = 0; 

    add_foreground(pgid, pgid, job->cmd);
    remove_job(pgid);
    
    while(foreground_list) sleep(1);
}

/* Function to handle the bg command */
void bg_command(struct cmdline* cmd)
{
    Jobs* job = get_default_job();
    pid_t pgid = job->pgid;
    kill(-pgid, SIGCONT);
    cmd->background = 1;
    job_list->state = RUNNING;
    pop_foreground(pgid);
}
