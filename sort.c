#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

// Comparator for sorting by PID
int compare_by_pid_asc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return pa->pid - pb->pid;
}

int compare_by_pid_desc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return pb->pid - pa->pid;
}


// Comparator for sorting by User
int compare_by_user_asc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return strcmp(pa->user, pb->user);
}

int compare_by_user_desc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return strcmp(pb->user, pa->user);
}


// Comparator for sorting by Priority
int compare_by_priority_asc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return pa->priority - pb->priority;
}

int compare_by_priority_desc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return pb->priority - pa->priority;
}


// Comparator for sorting by Command
int compare_by_command_asc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return strcmp(pa->command, pb->command);
}

int compare_by_command_desc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return strcmp(pb->command, pa->command);
}


// Sorting list
void sort_list(Process **processes, int count, int (*comparator)(const void *, const void *)) {
    qsort(processes, count, sizeof(Process *), comparator);
}


// Sorting tree
void sort_tree(Process *proc, int (*comparator)(const void *, const void *)) {
    // 자식 프로세스가 없는 경우
    if (proc == NULL || proc->child_count == 0) return;

    // 자식 프로세스(배열)를 정렬
    qsort(proc->children, proc->child_count, sizeof(Process *), comparator);

    // 각 자식의 자식들도 재귀적으로 정렬
    for (int i = 0; i < proc->child_count; i++) {
        sort_tree(proc->children[i], comparator);
    }
}
