/*
    print.c는 Process 구조체들을 main.c에서 저장하는 processes 배열에 대해,
    Sorting된 결과를 시각적으로 확인하기 위한 코드입니다.
*/

#include <stdio.h>
#include "process.h"

// Print the tree recursively
void print_tree(Process *process, int level) {
    // Print indentation using '\t' for each level
    for (int i = 0; i < level; i++) {
        printf("\t"); // Add tab for indentation
    }

    // Print process details
    printf("%d, Command: %s\n", process->pid, process->command);

    // 재귀 호출
    for (int i = 0; i < process->child_count; i++) {
        print_tree(process->children[i], level + 1);
    }
}

// Print processes by list
void print_list(Process **processes, int count) {
    for (int i = 0; i < count; i++) {
        if (processes[i]->isKernalThread == 1) continue;
        printf("PID: %d, User: %s, Priority: %d, Nice: %d, State: %c, CPU Usage: %.2f%%, Mem Usage: %.2f%%, "
               "Time: %lu, VIRT: %lu, RES: %lu, SHR: %lu, Command: %s\n",
               processes[i]->pid,              // PID
               processes[i]->user,             // User
               processes[i]->priority,         // Priority
               processes[i]->nice,             // Nice
               processes[i]->state,            // State
               processes[i]->cpu_usage,        // CPU Usage
               processes[i]->mem_usage,        // Mem Usage
               processes[i]->time,             // Time
               processes[i]->virt,             // VIRT
               processes[i]->res,              // RES
               processes[i]->shr,              // SHR
               processes[i]->command           // Command
        );
    }
}