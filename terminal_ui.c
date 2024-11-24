#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "print.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

char *info[] = {
    "PID",
    "USER",
    "Exit",
    "NI",
    "VIRT",
    "RES",
    "S",
    "CPU%",
    "MEM%",
    "TIME+",
    "COMMAND",
};

char *option[] = {
    "F1 HELP ",
    "F2 SETUP ",
    "F3 Search ",
    "F4 Tree ",
    "F5 SortBy ",
    "F6 NICE + ",
    "F7 NICE - ",
    "F8 KILL ",
    "F9 QUIT ",
};

WINDOW *upper_win, *info_win, *process_win, *bottom_win;
int highlight = 1;
int choice = 0;
int c;
int num_info = ARRAY_SIZE(info);
int num_option = ARRAY_SIZE(option);
int selected_row = 0;
int current_window = 0; // 0 for info_win, 1 for process_win

void print_upper(WINDOW *win)
{
  box(win, 0, 0);
  int x = 1; // X-coordinate padding
  int y = 1; // Y-coordinate padding

  mvwprintw(win, y, x, "1");
  mvwprintw(win, y + 1, x, "2");
  mvwprintw(win, y + 2, x, "3");
  mvwprintw(win, y + 3, x, "Mem");
  mvwprintw(win, y + 4, x, "Swp");

  wrefresh(win); // Refresh to display changes
}

void print_info(WINDOW *win, int highlight, int num_info)
{
  int x = 0; // X-coordinate padding
  int y = 0; // Y-coordinate padding
  // box(win, 0, 0);
  for (int i = 0; i < num_info; ++i)
  {
    if (highlight == i + 1)
    {
      wattron(win, A_REVERSE); // Highlight selected menu item
      mvwprintw(win, y, x, "%s", info[i]);
      wattroff(win, A_REVERSE);
    }
    else
    {
      mvwprintw(win, y, x, "%s", info[i]);
    }
    x += strlen(info[i]) + 2; // Add padding between menu items
  }
  wrefresh(win); // Refresh to display changes
}

void print_processes(WINDOW *win, int selected_row, Process *processes[], int height)
{
  werase(win);
  int x = 0;
  int y = 0;
  for (int i = 0; i < height; ++i)
  {
    if (selected_row == i + 1)
    {
      wattron(win, A_REVERSE);
      mvwprintw(win, y, x, "%d", processes[i]->pid);
      mvwprintw(win, y, x + 5, "%s", processes[i]->user);
      mvwprintw(win, y, x + 10, "%d", processes[i]->priority);
      mvwprintw(win, y, x + 15, "%d", processes[i]->nice);
      mvwprintw(win, y, x + 20, "%lu", processes[i]->virt);
      mvwprintw(win, y, x + 25, "%lu", processes[i]->res);
      mvwprintw(win, y, x + 30, "%c", processes[i]->state);
      mvwprintw(win, y, x + 35, "%.2f", processes[i]->cpu_usage);
      mvwprintw(win, y, x + 40, "%.2f", processes[i]->mem_usage);
      mvwprintw(win, y, x + 45, "%lu", processes[i]->time);
      mvwprintw(win, y, x + 50, "%s", processes[i]->command);
      wattroff(win, A_REVERSE);
    }
    else
    {
      mvwprintw(win, y, x, "%d", processes[i]->pid);
      mvwprintw(win, y, x + 5, "%s", processes[i]->user);
      mvwprintw(win, y, x + 10, "%d", processes[i]->priority);
      mvwprintw(win, y, x + 15, "%d", processes[i]->nice);
      mvwprintw(win, y, x + 20, "%lu", processes[i]->virt);
      mvwprintw(win, y, x + 25, "%lu", processes[i]->res);
      mvwprintw(win, y, x + 30, "%c", processes[i]->state);
      mvwprintw(win, y, x + 35, "%.2f", processes[i]->cpu_usage);
      mvwprintw(win, y, x + 40, "%.2f", processes[i]->mem_usage);
      mvwprintw(win, y, x + 45, "%lu", processes[i]->time);
      mvwprintw(win, y, x + 50, "%s", processes[i]->command);
    }
    y++;
  }
  wrefresh(win);
}

void print_bottom(WINDOW *win, int num_option)
{
  werase(win);
  int x, y;
  x = 0;
  y = 0;
  for (int i = 0; i < num_option; ++i)
  {
    mvwprintw(win, y, x, "%s", option[i]);
    x += strlen(info[i]) + 7; // Add padding between info columns
  }
  wrefresh(win); // Refresh to display changes
}

void run_ui(Process *processes[], int process_count)
{

  // Initialize ncurses mode
  initscr();
  start_color();                          // Start color functionality
  init_pair(1, COLOR_GREEN, COLOR_BLACK); // Initialize color pair 1 with green text on black background
  clear();
  noecho();
  cbreak();             // Line buffering disabled
  keypad(stdscr, TRUE); // Enable function keys

  // Get terminal dimensions
  int term_height, term_width;
  getmaxyx(stdscr, term_height, term_width);

  // Calculate window heights
  int upper_height = 7;
  int info_height = 1;
  int bottom_height = 1;
  int process_height = term_height - (upper_height + info_height + bottom_height);

  // Create windows
  upper_win = newwin(upper_height, term_width, 0, 0);
  info_win = newwin(info_height, term_width, upper_height, 0);
  process_win = newwin(process_height, term_width, upper_height + info_height, 0);
  bottom_win = newwin(bottom_height, term_width, upper_height + info_height + process_height, 0);

  // Enable keypad
  keypad(info_win, TRUE);
  keypad(process_win, TRUE);

  // Print initial content for windows
  print_upper(upper_win);
  wattron(info_win, COLOR_PAIR(1)); // Turn on color pair 1
  print_info(info_win, highlight, num_info);
  wattroff(info_win, COLOR_PAIR(1)); // Turn off color pair 1
  print_processes(process_win, selected_row, processes, process_height);
  print_bottom(bottom_win, num_option);

  if (current_window == 0)
  {
    c = wgetch(info_win);
  }
  else
  {
    c = wgetch(process_win);
  }

  switch (c)
  {
  case KEY_UP:
    if (current_window != 0)
    {
      if (highlight == 1)
        highlight = num_info;
      else
        --highlight;
    }
    else
    {
      if (selected_row > 0)
      {
        selected_row--;
      }
    }
    break;
  case KEY_DOWN:
    if (current_window != 0)
    {
      if (highlight == num_info)
        highlight = 1;
      else
        ++highlight;
    }
    else
    {
      if (selected_row < process_count - 1 && selected_row < process_height)
      {
        selected_row++;
      }
    }
    break;
  case KEY_LEFT:
    if (current_window == 0)
    {
      if (highlight == 1)
        highlight = num_info;
      else
        --highlight;
    }
    break;
  case KEY_RIGHT:
    if (current_window == 0)
    {
      if (highlight == num_info)
        highlight = 1;
      else
        ++highlight;
    }
    break;
  case 9: // Tab key to switch windows
    current_window = (current_window + 1) % 2;
    break;
  case KEY_F(9): // Enter key
    endwin();
    exit(EXIT_SUCCESS);
    break;
  default:
    break;
  }
  wattron(info_win, COLOR_PAIR(1)); // Turn on color pair 1
  print_info(info_win, highlight, num_info);
  wattroff(info_win, COLOR_PAIR(1)); // Turn off color pair 1
  print_processes(process_win, selected_row, processes, process_height);
  print_bottom(bottom_win, num_option);
}