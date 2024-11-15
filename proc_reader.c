#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/sysinfo.h>
#include "proc_reader.h"
#include "hash_table.h"

#define HASH_TABLE_SIZE 1024  // 해시 테이블 크기 설정

// 단일 프로세스의 정보를 읽는 함수
// TODO Process CMD도 받아올 수 있도록 추가
int read_process_info(Process *proc) {
    char path[256];
    FILE *file;

    // /proc/[PID]/stat 파일 열고 읽기
    snprintf(path, sizeof(path), "/proc/%d/stat", proc->pid);
    file = fopen(path, "r");
    if (!file) return -1;

    char comm[256];  // 커널 스레드 이름 또는 프로세스 이름 저장용
    unsigned long utime, stime, starttime;
    fscanf(file, "%*d (%[^)]) %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %d %d %*d %*u %lu",
        comm, &proc->state, &utime, &stime, &proc->priority, &proc->nice, &starttime);

    // 커널 스레드 이름 또는 프로세스 이름을 command에 저장
    strncpy(proc->command, comm, sizeof(proc->command));
    proc->time = (utime + stime) / sysconf(_SC_CLK_TCK);
    fclose(file);



    // /proc/[PID]/status 파일 열고 읽기
    snprintf(path, sizeof(path), "/proc/%d/status", proc->pid);
    file = fopen(path, "r");
    if (!file) return -1;

    char line[256];
    int uid = -1;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Uid:", 4) == 0) {    // user name
            sscanf(line, "Uid: %d", &uid);
            struct passwd *pw = getpwuid(uid);
            if (pw) strncpy(proc->user, pw->pw_name, sizeof(proc->user));
        } else if (strncmp(line, "VmSize:", 7) == 0) {  // VIRT
            sscanf(line, "VmSize: %lu", &proc->virt);
        } else if (strncmp(line, "VmRSS:", 6) == 0) {   // RES
            sscanf(line, "VmRSS: %lu", &proc->res); 
        } else if (strncmp(line, "RssFile:", 8) == 0) { // SHR
            sscanf(line, "RssFile: %lu", &proc->shr);
        }
    }
    fclose(file);

    struct sysinfo info;        // memory usage
    if (sysinfo(&info) == 0) {
        proc->mem_usage = ((float)proc->res / (info.totalram / 1024)) * 100.0;
    }


    // /proc/[PID]/cmdline 파일 열고 읽기
    snprintf(path, sizeof(path), "/proc/%d/cmdline", proc->pid);
    file = fopen(path, "r");
    if (file) {
        if (fgets(proc->command, sizeof(proc->command), file) == NULL) {
            proc->command[0] = '\0'; // 파일이 비었을 경우 빈 문자열로 초기화
        }
        fclose(file);
    } else {
        strncpy(proc->command, "[unknown]", sizeof(proc->command)); // 파일을 열 수 없는 경우 기본값
    }

    return 0;
}

// 모든 프로세스를 해시 테이블에 저장
int get_all_processes(HashTable *table) {
    DIR *proc_dir = opendir("/proc");
    struct dirent *entry;
    int count = 0;

    if (!proc_dir) return -1;

    while ((entry = readdir(proc_dir)) != NULL) {
        if (isdigit(entry->d_name[0])) {
            Process *proc = malloc(sizeof(Process));
            proc->pid = atoi(entry->d_name);
            if (read_process_info(proc) == 0) {
                insert_process(table, proc);
                count++;
            } else {
                free(proc);
            }
        }
    }

    closedir(proc_dir);
    return count;
}
