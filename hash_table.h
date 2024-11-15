#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "process.h"

// 해시 테이블 엔트리 구조체
typedef struct HashNode {
    int pid;                  // 프로세스 ID (키)
    Process *process;         // 프로세스 정보 (값)
    struct HashNode *next;    // 충돌 시 연결 리스트 형태로 연결
} HashNode;

// 해시 테이블 구조체
typedef struct {
    int size;                 // 해시 테이블 크기
    HashNode **table;         // 해시 테이블 배열
} HashTable;

// 해시 테이블 함수 선언
HashTable *create_hash_table(int size);
void free_hash_table(HashTable *table);
void insert_process(HashTable *table, Process *process);
Process *get_process(HashTable *table, int pid);
void remove_process(HashTable *table, int pid);

#endif // HASH_TABLE_H
