// proc_reader.h
#ifndef PROC_READER_H
#define PROC_READER_H

#include "process.h"
#include "hash_table.h"

int read_process_info(Process *proc);
int get_all_processes(HashTable *table);

#endif // PROC_READER_H
