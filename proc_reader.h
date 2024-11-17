// proc_reader.h
#ifndef PROC_READER_H
#define PROC_READER_H

#include "process.h"

void add_child(Process *parent, Process *child);

int read_process_info(Process *proc);
int get_all_processes(Process **processes, int max_count);

#endif
