#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "process.h"
#include "proc_reader.h"
#include "sort.h"
#include "print.h"
#include "system_reader.h"
#include "control.h"

#define MAX_PROCESSES 1024 

int main() {
    Process *processes[MAX_PROCESSES] = {0};

    // 시스템 리소스 변수
    unsigned long mem_used, mem_total, swap_used, swap_total;
    float load_by_1min, load_by_5min, load_by_15min;
    double uptime;

    while (1) {
        // Initializing Proecss List (Processes)
        for (int i = 0; i < MAX_PROCESSES; i++) {
            if (processes[i] != NULL) {
                free(processes[i]);
                processes[i] = NULL;
            }
        }

        // 프로세스 리소스 읽어오는 함수
        read_resource(
            &mem_used, &mem_total,
            &swap_used, &swap_total,
            &load_by_1min, &load_by_5min, &load_by_15min,
            &uptime);

        // 출력 예시
        // printf("Memory: %lu/%lu kB\n", mem_used, mem_total);
        // printf("Swap: %lu/%lu kB\n", swap_used, swap_total);
        // printf("Load average: %.2f, %.2f, %.2f\n", load_by_1min, load_by_5min, load_by_15min);
        // printf("Uptime: %.2f seconds\n", uptime);

        // 모든 프로세스 읽어오기
        int process_count = get_all_processes(processes, MAX_PROCESSES);
        if (process_count < 0) {
            perror("Failed to fetch processes");
            exit(EXIT_FAILURE);
        }
        
        // PID 기준으로 정렬
        sort_list(processes, process_count, compare_by_pid_asc);

        // USER 기준으로 정렬
        // sort_list(processes, process_count, compare_by_user_asc);

        // PRIORITY 기준으로 정렬
        // sort_list(processes, process_count, compare_by_priority_asc);

        // COMMAND 기준으로 정렬
        // sort_list(processes, process_count, compare_by_command_asc);

        // List 출력문
        print_list(processes, process_count);

        // tree 정렬
        // for (int i = 0; i < MAX_PROCESSES; i++)
        // {
        //     if (processes[i] == NULL)
        //         continue;
        //     else if (processes[i]->pid == 1)
        //     { // 루트 프로세스 (PID가 0인 프로세스)인 경우
        //         sort_tree(processes[i], compare_by_pid_asc);
        //     }
        // }

        // tree 출력문
        // print_tree(processes[0], 0);
        sleep(10);


        // int pid;
        // printf("Enter PID to modify or kill: ");
        // scanf("%d", &pid);

        // int option;
        // printf("Choose an action:\n");
        // printf("1: Increase NICE value\n");
        // printf("2: Decrease NICE value\n");
        // printf("3: Kill process\n");
        // printf("Enter your option: ");
        // scanf("%d", &option);

        // int result;

        // switch (option) {
        //     case 1:
        //         result = increase_nice(pid);
        //         if (result == 0) {
        //             printf("Successfully increased NICE value for PID %d.\n", pid);
        //         } else {
        //             perror("Failed to increase NICE value");
        //         }
        //         break;
        //     case 2:
        //         result = decrease_nice(pid);
        //         if (result == 0) {
        //             printf("Successfully decreased NICE value for PID %d.\n", pid);
        //         } else {
        //             perror("Failed to decrease NICE value");
        //         }
        //         break;
        //     case 3:
        //         result = kill_process(pid);
        //         if (result == 0) {
        //             printf("Successfully killed process with PID %d.\n", pid);
        //         } else {
        //             perror("Failed to kill process");
        //         }
        //         break;
        //     default:
        //         printf("Invalid option. Please select a valid action.\n");
        //         break;
        // }


    }
    return 0;
}
