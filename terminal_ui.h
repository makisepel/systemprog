#ifndef TERMINAL_UI_H
#define TERMINAL_UI_H

#include "process.h"
#include <ncurses.h>

void print_upper(WINDOW *win);
void print_info(WINDOW *win, int highlight, int num_info);
void print_processes(WINDOW *win,int highlight, Process *processes[], int process_count, int count);
void print_bottom(WINDOW *win, int num_option);
void run_ui(Process *processes[], int max_processes);

#endif // TERMINAL_UI_H