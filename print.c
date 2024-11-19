/*
    print.c는 Process 구조체들을 main.c에서 저장하는 processes 배열에 대해,
    Sorting된 결과 구현 여부를 시각적으로 확인하기 위한 코드입니다.
    TUI 구현에 있어서는 필요하지 않아, 자세한 설명은 생략합니다.
*/

#include <stdio.h>
#include "process.h"

// tree 형태를 재귀적으로 출력
void print_tree(Process *process, int level) {
    for (int i = 0; i < level; i++) {
        printf("\t");
    }

    printf("%d, Command: %s\n", process->pid, process->command);

    for (int i = 0; i < process->child_count; i++) {
        print_tree(process->children[i], level + 1);
    }
}

// 정렬된 processes 배열에 대해 list 형태를 출력
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