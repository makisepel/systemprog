// terminal_ui.h

#ifndef TERMINAL_UI_H
#define TERMINAL_UI_H

#include "process.h"
#include <ncurses.h>
char *formatSize(unsigned long int size);
int *choose_comparator(int highlight, int sort_order);
void print_upper(WINDOW *win);
void print_system(WINDOW *win);
void print_info(WINDOW *win, int highlight, int num_info);
void print_processes(WINDOW *win, int highlight, Process *processes[], int process_count, int count);
void run_ui(Process *processes[]);
void initialize_ncurses_mode();

#endif // TERMINAL_UI_H