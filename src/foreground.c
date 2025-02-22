#include "foreground.h"

Foreground *foreground_list = NULL;

void add_foreground(pid_t pid, pid_t pgid, char* cmd)
{
    Foreground *new_foreground = malloc(sizeof(Foreground));

    new_foreground->pid = pid;
    new_foreground->pgid = pgid;
    new_foreground->cmd = cmd;
    new_foreground->next = foreground_list;
    foreground_list = new_foreground;
}

void pop_foreground(pid_t pgid)
{
    if (foreground_list) {
        if(foreground_list->pgid == pgid){
            Foreground *old_head = foreground_list;
            foreground_list = foreground_list->next;
            old_head = NULL;
            free(old_head);
            return;
        }

        Foreground *current = foreground_list;
        Foreground *prev = NULL;

        while(current != NULL && current->pgid != pgid){
            prev = current;
            current = current->next;
        }

        if(current){
            prev->next = current->next;
            current = NULL;
            free(current);
        }
    }
}

void free_foreground()
{
    Foreground *curr = foreground_list;
    Foreground *next;

    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }

    foreground_list = NULL;
}


Foreground* get_foreground_head()
{
    return foreground_list;
}