// sort.h
#ifndef SORT_H
#define SORT_H

#include "process.h"

int compare_by_pid(const void *a, const void *b);
int compare_by_user(const void *a, const void *b);
int compare_by_priority(const void *a, const void *b);
int compare_by_command(const void *a, const void *b);

void sort_processes(Process **processes, int count, int (*comparator)(const void *, const void *));
void print_processes(Process **processes, int count);

#endif
