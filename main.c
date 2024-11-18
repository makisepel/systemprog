#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "process.h"
#include "proc_reader.h"
#include "sort.h"
#include "tree.h"
#include "system_reader.h"

#define MAX_PROCESSES 1024 

int main() {
    Process *processes[MAX_PROCESSES] = {0};
    // unsigned long mem_used, mem_total, swap_used, swap_total;
    // float load_by_1min, load_by_5min, load_by_15min;
    // double uptime;

    while (1) {
        for (int i = 0; i < MAX_PROCESSES; i++) {
            if (processes[i] != NULL) {
                free(processes[i]);
                processes[i] = NULL;
            }
        }

        // 프로세스 리소스 읽어오는 함수
        // read_memory_usage(&mem_used, &mem_total);
        // read_swap_usage(&swap_used, &swap_total);
        // read_load_average(&load_by_1min, &load_by_5min, &load_by_15min);
        // read_uptime(&uptime);

        int process_count = get_all_processes(processes, MAX_PROCESSES);
        if (process_count < 0) {
            perror("Failed to fetch processes");
            exit(EXIT_FAILURE);
        }
        
        // PID 기준으로 정렬
        sort_processes(processes, process_count, compare_by_pid);

        // USER 기준으로 정렬
        // sort_processes(processes, process_count, compare_by_user);

        // PRIORITY 기준으로 정렬
        // sort_processes(processes, process_count, compare_by_priority);

        // COMMAND 기준으로 정렬
        // sort_processes(processes, process_count, compare_by_command);

        // USER 기준으로 정렬
        // sort_processes(processes, process_count, compare_by_user);

        // printf("Memory: %lu/%lu kB\n", mem_used, mem_total);
        // printf("Swap: %lu/%lu kB\n", swap_used, swap_total);
        // printf("Load average: %.2f, %.2f, %.2f\n", load_by_1min, load_by_5min, load_by_15min);
        // printf("Uptime: %.2f seconds\n", uptime);
        
        // printf("==================\n\n");
        // print_processes(processes, process_count);

        // printf("==================\n\n");

         for (int i = 0; i < MAX_PROCESSES; i++)
        {
            if (processes[i] == NULL)
                continue;
            else if (processes[i]->pid == 1)
            { // 루트 프로세스 (PID가 0인 프로세스)인 경우
                sort_children(processes[i]);
            }
        }
        print_tree(processes[0], 0);
        sleep(2);
    }
    return 0;
}
