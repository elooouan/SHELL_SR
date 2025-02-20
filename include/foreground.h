#ifndef __FOREGROUND_H
#define __FOREGROUND_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include "readcmd.h"
#include "execcmd.h"
#include "csapp.h"

extern struct Foreground *foreground_list;

typedef struct Foreground {
    pid_t pid, pgid;
    struct Foreground *next;    
} Foreground;

void add_foreground(pid_t pid, pid_t pgid);
void pop_foreground(pid_t pid);
void free_foreground ();

Foreground* get_foreground_head();

#endif