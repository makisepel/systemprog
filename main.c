#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "hash_table.h"
#include "proc_reader.h"
#include "sort.h"

#define HASH_TABLE_SIZE 1024  // 해시 테이블의 크기

int main() {
    HashTable *table = create_hash_table(HASH_TABLE_SIZE);

    // 모든 프로세스 읽기
    int process_count = get_all_processes(table);

    // 동적 배열 생성 (Process 포인터 배열)
    Process **processes = malloc(sizeof(Process *) * process_count);

    // 해시 테이블에서 프로세스 복사
    int index = 0;
    for (int i = 0; i < table->size; i++) {
        HashNode *node = table->table[i];
        while (node) {
            processes[index++] = node->process;  // 포인터 복사
            node = node->next;
        }
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





    // 출력
    print_processes(processes, process_count);


    // 메모리 해제
    free(processes);
    free_hash_table(table);

    return 0;
}
