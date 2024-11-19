// proc_reader.h
#ifndef PROC_READER_H
#define PROC_READER_H

#include "process.h"

void add_child(Process *parent, Process *child);
void link_userthread(Process *proc, Process **processes, int *cnt, unsigned long total_ram, unsigned long long total_cpu_time);


int read_process_info(Process *proc);
int get_all_processes(Process **processes, int max_count);

#endif