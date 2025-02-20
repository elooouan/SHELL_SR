#include "handlers.h"
#include "foreground.h"
#include "jobs.h"

/* Handler for the SIGCHILD signal */
void handler_sigchild(int sig)
{
    pid_t pid;
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        print_done_job(pid);
        remove_job(pid);
        free_foreground();
    }
    if (pid == -1 && errno != ECHILD) { // a bien comprendre le errno -> no more children left -> with ECHILD is not a ERROR
    	unix_error("waitpid error");
    }
    return;
}

void handler_sigint(int sig)
{
    if (!foreground_list) return;
    
    Foreground* fg = get_foreground_head();

    Kill(-(fg->pgid), SIGINT);
    free_foreground();

    write(STDOUT_FILENO, "\n", 1);
}

void handler_sigtstp(int sig)
{
    if (!foreground_list) return;
    
    Foreground* fg = get_foreground_head();
    pid_t pgid = fg->pgid;

    Kill(-(pgid), SIGTSTP);
    add_job(pgid, fg->cmd);
    Jobs* added_job = get_default_job();
    added_job->state = STOPPED;
    pop_foreground(fg->pid);

    write(STDOUT_FILENO, "\n", 1);
}

