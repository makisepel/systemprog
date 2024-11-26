#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "print.h"
#include "sort.h"

#define INFO_WINDOW 0
#define PROCESS_WINDOW 1
#define ASCENDING 1
#define DESCENDING -1
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

char *info[] = {
    "PID", 
    "USER", 
    "PRI", 
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
int (*comparator)(const void *, const void *);
int sort_order = 1; // 1 for ascending, -1 for descending

// choose comparator based on highlight and sort_order
int (*choose_comparator(int highlight, int sort_order))(const void *, const void *)
{
    switch (highlight)
    {
    case 1:
        return sort_order == ASCENDING ? compare_by_pid_asc : compare_by_pid_desc;
        break;
    case 2:
        return sort_order == ASCENDING ? compare_by_user_asc : compare_by_user_desc;
        break;
    case 3: 
        return sort_order == ASCENDING ? compare_by_priority_asc : compare_by_priority_desc;
        break;
    case 4:
        return sort_order == ASCENDING ? compare_by_nice_asc : compare_by_nice_desc;
        break;
    case 5:
        return sort_order == ASCENDING ? compare_by_virt_asc : compare_by_virt_desc;
        break;
    case 6:
        return sort_order == ASCENDING ? compare_by_res_asc : compare_by_res_desc;
        break;
    case 7:
        return sort_order == ASCENDING ? compare_by_state_asc : compare_by_state_desc;
        break;
    case 8:
        return sort_order == ASCENDING ? compare_by_cpu_usage_asc : compare_by_cpu_usage_desc;
        break;
    case 9:
        return sort_order == ASCENDING ? compare_by_mem_usage_asc : compare_by_mem_usage_desc;
        break;
    case 10:
        return sort_order == ASCENDING ? compare_by_time_asc : compare_by_time_desc;
        break;
    case 11:
        return sort_order == ASCENDING ? compare_by_command_asc : compare_by_command_desc;
        break;
    default:
        break;
    }   
}

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

void print_processes(WINDOW *win, int selected_row, Process *processes[], int process_count, int height)
{
  werase(win);
  int x = 0;
  int y = 0;

  int start_row = selected_row < height ? 0 : selected_row - height + 1;

  for (int i = 0; i < height; i++)
  {
    int index = start_row + i;
    if (index >= process_count) break;

    if (selected_row == index)
    {
      wattron(win, A_REVERSE);
      mvwprintw(win, y, x, "%d", processes[index]->pid);
      mvwprintw(win, y, x + 5, "%s", processes[index]->user);
      mvwprintw(win, y, x + 10, "%d", processes[index]->priority);
      mvwprintw(win, y, x + 15, "%d", processes[index]->nice);
      mvwprintw(win, y, x + 20, "%lu", processes[index]->virt);
      mvwprintw(win, y, x + 25, "%lu", processes[index]->res);
      mvwprintw(win, y, x + 30, "%c", processes[index]->state);
      mvwprintw(win, y, x + 35, "%.2f", processes[index]->cpu_usage);
      mvwprintw(win, y, x + 40, "%.2f", processes[index]->mem_usage);
      mvwprintw(win, y, x + 45, "%lu", processes[index]->time);
      mvwprintw(win, y, x + 50, "%s", processes[index]->command);
      wattroff(win, A_REVERSE);
    }
    else
    {
      mvwprintw(win, y, x, "%d", processes[index]->pid);
      mvwprintw(win, y, x + 5, "%s", processes[index]->user);
      mvwprintw(win, y, x + 10, "%d", processes[index]->priority);
      mvwprintw(win, y, x + 15, "%d", processes[index]->nice);
      mvwprintw(win, y, x + 20, "%lu", processes[index]->virt);
      mvwprintw(win, y, x + 25, "%lu", processes[index]->res);
      mvwprintw(win, y, x + 30, "%c", processes[index]->state);
      mvwprintw(win, y, x + 35, "%.2f", processes[index]->cpu_usage);
      mvwprintw(win, y, x + 40, "%.2f", processes[index]->mem_usage);
      mvwprintw(win, y, x + 45, "%lu", processes[index]->time);
      mvwprintw(win, y, x + 50, "%s", processes[index]->command);
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
    curs_set(0);          // Hide cursor

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
  keypad(stdscr, TRUE);
  keypad(info_win, TRUE);
  keypad(process_win, TRUE);

  // Print initial content for windows
  print_upper(upper_win);
  wattron(info_win, COLOR_PAIR(1)); // Turn on color pair 1
  print_info(info_win, highlight, num_info);
  wattroff(info_win, COLOR_PAIR(1)); // Turn off color pair 1
  print_processes(process_win, selected_row, processes, process_count, process_height);
  print_bottom(bottom_win, num_option);

  while(1){
    if (current_window == INFO_WINDOW){
            c = wgetch(info_win);
        }
        else{
            c = wgetch(process_win);
        }

        switch (c){
        case KEY_UP:
            if (current_window == PROCESS_WINDOW){
                if(selected_row > 0) selected_row--;
            }
            break;
        case KEY_DOWN:
            if (current_window == PROCESS_WINDOW){
                if (selected_row < process_count - 1) selected_row++;
            }
            break;
        case KEY_LEFT:
            if (current_window == INFO_WINDOW){
                if (highlight == 1) highlight = num_info;
                else --highlight;
            }
            break;
        case KEY_RIGHT:
            if (current_window == INFO_WINDOW){
                if (highlight == num_info) highlight = 1;
                else ++highlight;
            }
            break;
        case 9: // Tab key to switch windows
            current_window = (current_window + 1) % 2;
            break;
        case '\n': // Enter key
        case KEY_ENTER:
            if (current_window == INFO_WINDOW && highlight >= 1){
                comparator = choose_comparator(highlight, sort_order);
                sort_list(processes, process_count, comparator);
                sort_order = -sort_order;
                selected_row = 0;
            }
            break;
        case KEY_F(1): // F1 for help
            break;
        case KEY_F(2): // F2 for setup
            break;
        case KEY_F(3): // F3 for search
            break;
        case KEY_F(4): // F4 for tree   
            break;
        case KEY_F(5): // F5 for sort by  
            break;
        case KEY_F(6): // F6 for nice +
            break;
        case KEY_F(7): // F7 for nice -
            break;
        case KEY_F(8): // F8 for kill
            break;
        case KEY_F(9): // F9 to quit
            endwin();
            exit(EXIT_SUCCESS);
            break;
        default:
            break;
        }
        
        wattron(info_win, COLOR_PAIR(1)); // Turn on color pair 1
        print_info(info_win, highlight, num_info);
        wattroff(info_win, COLOR_PAIR(1)); // Turn off color pair 1
        print_processes(process_win, selected_row, processes, process_count, process_height);
        print_bottom(bottom_win, num_option);

  }
}