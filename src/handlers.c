#include "handlers.h"
#include "foreground.h"

/* Handler for the SIGCHILD signal */
void handler_sigchild(int sig)
{
    pid_t pid;
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        print_done_job(pid);
        remove_job(pid);
        pop_foreground(pid);
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
    Kill(-(fg->pgid), SIGTERM);
    free_foreground();

}

void handler_sigtstp(int sig)
{

}

