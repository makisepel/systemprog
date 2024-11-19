// sort.h
#ifndef SORT_H
#define SORT_H

#include "process.h"

int compare_by_pid_asc(const void *a, const void *b);
int compare_by_pid_desc(const void *a, const void *b);

int compare_by_user_asc(const void *a, const void *b);
int compare_by_user_desc(const void *a, const void *b);

int compare_by_priority_asc(const void *a, const void *b);
int compare_by_priority_desc(const void *a, const void *b);

int compare_by_command_asc(const void *a, const void *b);
int compare_by_command_desc(const void *a, const void *b);

void sort_list(Process **processes, int count, int (*comparator)(const void *, const void *));
void sort_tree(Process *proc, int (*comparator)(const void *, const void *));

#endif
