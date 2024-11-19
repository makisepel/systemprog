/*
    sort.c 함수는 qsort함수를 기반으로, list 또는 tree형태를 각각 정렬하는 코드이며
    이 때 qsort에 필요한 comparator 함수, 즉 정렬 기준이 구현되어 있습니다.
    일부 기준들은 오버플로우를 피하기 위해 비교 연산자를 연산하였습니다.
*/

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

// Comparator for sorting by Nice
int compare_by_nice_asc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return pa->nice - pb->nice;
}
int compare_by_nice_desc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return pb->nice - pa->nice;
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

// Comparator for sorting by Virtual Memory (virt)
int compare_by_virt_asc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return (pa->virt > pb->virt) - (pa->virt < pb->virt); // Avoiding overflow
}
int compare_by_virt_desc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return (pb->virt > pa->virt) - (pb->virt < pa->virt);
}

// Comparator for sorting by Resident Memory (res)
int compare_by_res_asc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return (pa->res > pb->res) - (pa->res < pb->res);
}
int compare_by_res_desc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return (pb->res > pa->res) - (pb->res < pa->res);
}

// Comparator for sorting by Shared Memory (shr)
int compare_by_shr_asc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return (pa->shr > pb->shr) - (pa->shr < pb->shr);
}
int compare_by_shr_desc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return (pb->shr > pa->shr) - (pb->shr < pa->shr);
}

// Comparator for sorting by State
int compare_by_state_asc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return pa->state - pb->state;
}
int compare_by_state_desc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return pb->state - pa->state;
}

// Comparator for sorting by CPU Usage
int compare_by_cpu_usage_asc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return (pa->cpu_usage > pb->cpu_usage) - (pa->cpu_usage < pb->cpu_usage);
}
int compare_by_cpu_usage_desc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return (pb->cpu_usage > pa->cpu_usage) - (pb->cpu_usage < pa->cpu_usage);
}

// Comparator for sorting by Memory Usage
int compare_by_mem_usage_asc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return (pa->mem_usage > pb->mem_usage) - (pa->mem_usage < pb->mem_usage);
}
int compare_by_mem_usage_desc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return (pb->mem_usage > pa->mem_usage) - (pb->mem_usage < pa->mem_usage);
}

// Comparator for sorting by Time
int compare_by_time_asc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return (pa->time > pb->time) - (pa->time < pb->time);
}
int compare_by_time_desc(const void *a, const void *b) {
    Process *pa = *(Process **)a;
    Process *pb = *(Process **)b;
    return (pb->time > pa->time) - (pb->time < pa->time);
}


/**
 * list
 * @brief main 함수에 선언된 processes 배열을 comparator에 따라 정렬하는 함수
 * 
 * @param processes main 함수에 선언된 process 구조체들을 담는 배열
 * @param count processes 배열의 길이
 * @param comparator processes 배열을 정렬하는 기준
 */
void sort_list(Process **processes, int count, int (*comparator)(const void *, const void *)) {
    qsort(processes, count, sizeof(Process *), comparator);
}

/**
 * tree
 * @brief tree 형태로 연결된 process들을 재귀적으로 정렬하는 함수
 * 
 * @param proc 정렬할 부모 process
 * @param comparator 부모 process에 연결된 children 배열을 정렬하는 기준
 */
void sort_tree(Process *proc, int (*comparator)(const void *, const void *)) {
    if (proc == NULL || proc->child_count == 0) return;

    qsort(proc->children, proc->child_count, sizeof(Process *), comparator);
    for (int i = 0; i < proc->child_count; i++) {
        sort_tree(proc->children[i], comparator);
    }
}
