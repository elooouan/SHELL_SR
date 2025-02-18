#include "foreground.h"

Foreground *foreground_list = NULL;

void add_foreground(pid_t pid){
    Foreground *new_foreground = malloc(sizeof(Foreground));

    new_foreground->pid = pid;
    new_foreground->next = foreground_list;
    foreground_list = new_foreground;
}

void pop_foreground(pid_t pid){
    if(foreground_list){
        if(foreground_list->pid == pid){
            Foreground *old_head = foreground_list;
            foreground_list = foreground_list->next;
            old_head = NULL;
            free(old_head);
            return;
        }

        Foreground *current = foreground_list;
        Foreground *prev = NULL;

        while(current != NULL && current->pid != pid){
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