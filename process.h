// process.h
#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

typedef struct Process Process;

struct Process {
    pid_t pid;          // 프로세스 ID, stat
    pid_t ppid;         // ppid, stat
    char user[32];      // 사용자 이름, status
    int priority;       // 우선순위, stat
    int nice;           // NICE 값, stat
    char state;         // 프로세스 상태, stat
    float cpu_usage;    // CPU 사용률
    float mem_usage;    // 메모리 사용률
    unsigned long int time; // CPU 사용 시간, status
    unsigned long int virt; // 가상 메모리 크기, status
    unsigned long int res;  // 물리 메모리 크기, status
    unsigned long int shr;  // 공유 메모리 크기, status
    char command[256];  // 실행 명령어, stat (cmdline에서 확인)
    Process **children;     // 자식노드
    int child_count;        // 자식노드 갯수
};

#endif
