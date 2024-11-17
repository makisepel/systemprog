// proc_reader.h
#ifndef PROC_READER_H
#define PROC_READER_H

#include "process.h"

int read_process_info(Process *proc);
int get_all_processes(Process **processes, int max_count);

#endif
