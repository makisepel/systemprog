// print.h
#ifndef PRINT_H
#define PRINT_H

#include "process.h"

void print_tree(Process *process, int level);
void print_list(Process **processes, int count);

#endif