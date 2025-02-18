#include "handlers.h"

/* Handler for the SIGCHILD signal */
void handler_sigchild(int sig)
{
    pid_t pid;
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) remove_job(pid);

    if (pid == -1 && errno != ECHILD) { // a bien comprendre le errno -> no more children left -> with ECHILD is not a ERROR
    	unix_error("waitpid error");
    }
    return;
}

