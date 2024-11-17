#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

// Comparator for sorting by PID
int compare_by_pid(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return pa->pid - pb->pid;
}
// Comparator for sorting by User
int compare_by_user(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return strcmp(pa->user, pb->user);
}

// Comparator for sorting by Priority
int compare_by_priority(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return pa->priority - pb->priority;
}

// Comparator for sorting by Command
int compare_by_command(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return strcmp(pa->command, pb->command);
}

// Sort the processes using the chosen comparator
void sort_processes(Process **processes, int count, int (*comparator)(const void *, const void *)) {
    qsort(processes, count, sizeof(Process *), comparator);
}

// Print sorted processes
void print_processes(Process **processes, int count) {
    for (int i = 0; i < count; i++) {
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

