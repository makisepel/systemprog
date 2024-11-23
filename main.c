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

int main() {
    // process들을 받는 배열
    Process *processes[MAX_PROCESSES] = {0};

    // 시스템 리소스 변수
    unsigned long mem_used, mem_total, swap_used, swap_total;
    float load_by_1min, load_by_5min, load_by_15min;
    double uptime;

    while (1) {
        // processes 배열 초기화
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

        // 프로세스 출력 예시
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
        
        // list일 경우, PID 기준으로 정렬하며 모든 comparator에 대해서는 sort.c 참조
        sort_list(processes, process_count, compare_by_pid_asc);

        // list 출력
        run_ui(processes, process_count);
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
        
        sleep(10);
    }
    return 0;
}
