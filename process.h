// process.h
#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

typedef struct {
    pid_t pid;          // 프로세스 ID
    char user[32];      // 사용자 이름
    int priority;       // 우선순위
    int nice;           // NICE 값
    char state;         // 프로세스 상태
    float cpu_usage;    // CPU 사용률
    unsigned long int time; // CPU 사용 시간, status
    unsigned long int virt; // 가상 메모리 크기, status
    unsigned long int res;  // 물리 메모리 크기, status
    unsigned long int shr;  // 공유 메모리 크기, status
    float mem_usage;    // 메모리 사용률, sysinfo 명령어
    char command[256];  // 실행 명령어
} Process;

#endif // PROCESS_H
