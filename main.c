// main.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "process.h"
#include "terminal_ui.h"
#include "proc_reader.h"
#include "sort.h"
#include "print.h"
#include "system_reader.h"
#include "control.h"

#define MAX_PROCESSES 1024

extern Process *processes;

int main()
{
  Process *processes[1024] = {0};

  initialize_ncurses_mode();

  while (1)
  {
    run_ui(processes);
    // print_list(processes, process_count);

    // tree 정렬
    // for (int i = 0; i < MAX_PROCESSES; i++) {
    //     if (processes[i] == NULL) continue;
    //     else if (processes[i]->pid == 1) { // 루트 프로세스 (PID가 0인 프로세스)인 경우
    //         sort_tree(processes[i], compare_by_pid_asc);
    //         break;
    //     }
    // }

    // tree 출력
    // print_tree(processes[0], 0);
  }
  return 0;
}
