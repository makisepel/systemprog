// proc_reader.h
#ifndef PROC_READER_H
#define PROC_READER_H

#include "process.h"

void add_child(Process *parent, Process *child);
void add_children_from_tasks(Process *proc, Process **processes, int *cnt, unsigned long totalram, unsigned long long total_cpu_time);

int read_process_info(Process *proc);
int get_all_processes(Process **processes, int max_count);

#endif
