#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "hash_table.h"
#include "proc_reader.h"

#define HASH_TABLE_SIZE 1024  // 해시 테이블의 크기

void print_all_processes(HashTable *table) {
    // 모든 해시 테이블 엔트리를 순회하며 프로세스 정보를 출력합니다.
    for (int i = 0; i < table->size; i++) {
        HashNode *node = table->table[i];
        while (node) {
            Process *proc = node->process;
            // printf("PID: %d, User: %s, Priority: %d, Nice: %d, State: %c, CPU Usage: %.2f, Time: %lu, Virt: %lu, Res: %lu, Shr: %lu, Mem Usage: %.2f\n",
            //        proc->pid, proc->user, proc->priority, proc->nice, proc->state, proc->cpu_usage, proc->time,
            //        proc->virt, proc->res, proc->shr, proc->mem_usage);
            printf("PID: %d, User: %s, Priority: %d, Nice: %d, State: %c, Command: %s\n",
                    proc->pid, proc->user, proc->priority, proc->nice, proc->state, proc->command);

            node = node->next;
        }
    }
}

int main() {
    // 해시 테이블 생성
    HashTable *table = create_hash_table(HASH_TABLE_SIZE);

    // 프로세스 정보를 해시 테이블에 저장
    int process_count = get_all_processes(table);
    if (process_count < 0) {
        fprintf(stderr, "Failed to retrieve processes.\n");
        free_hash_table(table);
        return 1;
    }

    printf("Total processes retrieved: %d\n\n", process_count);
    printf("PID\tUSER\tPRI\tNI\tS\t%%CPU\tTIME+\tVIRT\tRES\tSHR\t%%MEM\n");
    printf("-------------------------------------------------------------------\n");

    // 해시 테이블에 저장된 모든 프로세스 정보를 출력
    print_all_processes(table);

    // 해시 테이블 해제
    free_hash_table(table);

    return 0;
}
