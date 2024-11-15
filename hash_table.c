#include <stdlib.h>
#include <stdio.h>
#include "hash_table.h"

// 간단한 해시 함수
int hash(int pid, int size) {
    return pid % size;
}

// 해시 테이블 생성
HashTable *create_hash_table(int size) {
    HashTable *table = malloc(sizeof(HashTable));
    table->size = size;
    table->table = malloc(sizeof(HashNode *) * size);

    for (int i = 0; i < size; i++) {
        table->table[i] = NULL;
    }

    return table;
}

// 해시 테이블 해제
void free_hash_table(HashTable *table) {
    for (int i = 0; i < table->size; i++) {
        HashNode *node = table->table[i];
        while (node) {
            HashNode *next = node->next;
            free(node->process);
            free(node);
            node = next;
        }
    }
    free(table->table);
    free(table);
}

// 프로세스를 해시 테이블에 삽입
void insert_process(HashTable *table, Process *process) {
    int index = hash(process->pid, table->size);
    HashNode *new_node = malloc(sizeof(HashNode));
    new_node->pid = process->pid;
    new_node->process = process;
    new_node->next = table->table[index];
    table->table[index] = new_node;
}

// 해시 테이블에서 PID로 프로세스 찾기
Process *get_process(HashTable *table, int pid) {
    int index = hash(pid, table->size);
    HashNode *node = table->table[index];
    while (node) {
        if (node->pid == pid) {
            return node->process;
        }
        node = node->next;
    }
    return NULL;
}

// 해시 테이블에서 프로세스 제거
void remove_process(HashTable *table, int pid) {
    int index = hash(pid, table->size);
    HashNode *node = table->table[index];
    HashNode *prev = NULL;

    while (node) {
        if (node->pid == pid) {
            if (prev) {
                prev->next = node->next;
            } else {
                table->table[index] = node->next;
            }
            free(node->process);
            free(node);
            return;
        }
        prev = node;
        node = node->next;
    }
}
