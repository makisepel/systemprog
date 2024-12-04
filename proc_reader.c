/*
    proc_reader.c는 /proc directory에서 process 단위로 읽어와 구조체를 저장하기 위한 코드이며,
    kernal thread인지 userland thread인지에 따라 process를 저장합니다.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/sysinfo.h>
#include "proc_reader.h"

/**
 * @brief 부모 Process 구조체의 children 배열에 자식 Process를 연결하는 함수
 *
 * @param parent The parent process.
 * @param child The child process to be added.
 */
void add_child(Process *parent, Process *child) {
  parent->children = realloc(parent->children, sizeof(Process *) * (parent->child_count + 1));
  if (!parent->children) {
    perror("Failed to allocate memory for children");
    exit(EXIT_FAILURE);
  }
  parent->children[parent->child_count++] = (struct Process *)child;
}

/**
 * @brief 해당 process의 Userland Thread를 /proc/[PID]/task 에서 가져오는 함수
 * Processes 배열에서 모든 PID에 대해 /proc/[PID]/task 내부의 Userland Thread를 읽어오며
 * 이 떄 Processes 배열에 해당 Userland Thread를 추가하고, 부모 Process에 자식 Process를 연결
 *
 * @param proc The parent process to which threads will be added.
 * @param processes The array of processes where the threads will also be stored.
 * @param cnt The pointer to the current count of processes in the array.
 * @param total_ram The total RAM of the system (for calculating memory usage).
 * @param total_cpu_time The total CPU time of the system (for calculating CPU usage).
 */
void link_userthread(Process *proc, Process **processes, int *cnt, unsigned long total_ram, unsigned long long total_cpu_time) {
  if (!proc)  return;
  if (proc->children == NULL)
    proc->child_count = 0;

  char path[256];
  snprintf(path, sizeof(path), "/proc/%d/task", proc->pid);
  DIR *task_dir = opendir(path);
  if (!task_dir) {
    perror("Failed to open task directory");
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(task_dir)) != NULL) {
    if (isdigit(entry->d_name[0])) {
      int tid = atoi(entry->d_name);
      if (tid != proc->pid) {
        Process *child_thread = malloc(sizeof(Process));
        if (!child_thread) {
          perror("Failed to allocate memory for child thread");
          closedir(task_dir);
          return;
        }

        // child_thread 초기화
        memset(child_thread, 0, sizeof(Process));

        // child_thread 정보 저장
        child_thread->pid = tid;
        if (read_process_info(child_thread) == 0) {
          processes[++(*cnt)] = child_thread; // processes 배열에 Userland Thread 추가
          child_thread->mem_usage = ((float)child_thread->res / (total_ram / 1024)) * 100.0;
          child_thread->cpu_usage = ((float)child_thread->time / total_cpu_time) * 100.0;
          add_child(proc, child_thread);
          child_thread->parent = proc;
        }
        else {
          free(child_thread);
          child_thread = NULL;
        }
      }
    }
  }

  closedir(task_dir);
  return;
}

/**
 * @brief /proc/[PID] 내부 directory에서 해당 pid의 process를 구조체로 저장하는 함수
 * kernel thread인 경우, CPU 및 MEM은 0으로 초기화
 *
 * @param proc The process structure to fill.
 * @return int Returns 0 on success, -1 on failure.
 */
int read_process_info(Process *proc) {
  char path[256];
  FILE *file;
  proc->isKernalThread = 0;

  /**
   * /proc/[PID]/cmdline
   * 해당 프로세스가 kernel thread인지 확인
   */
  snprintf(path, sizeof(path), "/proc/%d/cmdline", proc->pid);
  file = fopen(path, "r");
  if (file) {
    if (fgets(proc->command, sizeof(proc->command), file) == NULL) {
      proc->isKernalThread = 1;
    }
    fclose(file);
  }
  else {
    strncpy(proc->command, "[unknown]", sizeof(proc->command));
  }

  /**
   * /proc/[PID]/stat
   * pid, command, state, ppid, time, priority, nice
   */
  snprintf(path, sizeof(path), "/proc/%d/stat", proc->pid);
  file = fopen(path, "r");
  if (!file)
    return -1;

  char comm[256];
  unsigned long utime, stime, starttime;
  fscanf(file, "%d (%[^)]) %c %d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %d %d %*d %*u %lu",
         &proc->pid,      // PID: 1st field
         comm,            // Command: 2nd field (wrapped in parentheses)
         &proc->state,    // State: 3rd field
         &proc->ppid,     // PPID: 4th field
         &utime,          // utime: 14th field
         &stime,          // stime: 15th field
         &proc->priority, // Priority: 18th field
         &proc->nice,     // Nice: 19th field
         &starttime);     // Start time: 22nd field
  if (!isalpha(proc->state)) // 괄호 2쌍으로 둘러싸인 예외 처리 state 값이 알파벳이 아닌 값(ex..괄호)면 ((커맨드값)) 와 같은 쌍괄호 임을 인지. 문자열 하나로 읽어줌. 
  {                          // 그리고 커널 스레드가 아니면 cmdline 에서 읽어 오기 때문에 stat 에서 읽은 comm 값을 쓰지 않음.
            fseek(file, 0, SEEK_SET);
    fscanf(file, "%d %s %c %d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %d %d %*d %*u %lu",
           &proc->pid,      // PID
           comm,            // 괄호 안 명령어
           &proc->state,    // 상태
           &proc->ppid,     // 부모 PID
           &utime,          // 사용자 CPU 시간
           &stime,          // 시스템 CPU 시간
           &proc->priority, // 우선순위
           &proc->nice,     // nice 값
           &starttime);     // 시작 시간
  }
  // 확인 후 삭제...
  // if (comm[0] == '(') {
  //     int i = 1, j = 0;
  //     while (comm[i] != '\0')
  //         comm[j++] = comm[i++];
  //     comm[j - 1] = '\0';
  // }

  if (proc->isKernalThread)
    strncpy(proc->command, comm, sizeof(proc->command));
  proc->time = (utime + stime) / sysconf(_SC_CLK_TCK);
  fclose(file);

  /**
   * /proc/[PID]/status
   * user, virt, res, shr
   */
  snprintf(path, sizeof(path), "/proc/%d/status", proc->pid);
  file = fopen(path, "r");
  if (!file)
    return -1;

  /**
   * Kernal Thread's UID is generally 0 (that points root)
   * To reduce using memory, if Process is Kernal Thread
   * avoiding iteration will reduce memory resources
   */
  char line[256];
  int uid = -1;
  if (proc->isKernalThread) {
    strncpy(proc->user, "root", sizeof(proc->user));
    proc->virt = 0;
    proc->res = 0;
    proc->shr = 0;
  }
  else {
    while (fgets(line, sizeof(line), file)) {
      if (strncmp(line, "Uid:", 4) == 0) { // user name
        sscanf(line, "Uid: %d", &uid);
        struct passwd *pw = getpwuid(uid);
        if (pw)
          strncpy(proc->user, pw->pw_name, sizeof(proc->user));
      }
      else if (strncmp(line, "VmSize:", 7) == 0) { // virt
        sscanf(line, "VmSize: %lu", &proc->virt);
      }
      else if (strncmp(line, "VmRSS:", 6) == 0) { // res
        sscanf(line, "VmRSS: %lu", &proc->res);
      }
      else if (strncmp(line, "RssFile:", 8) == 0) { // shr
        sscanf(line, "RssFile: %lu", &proc->shr);
      }
    }
  }
  fclose(file);

  return 0;
}

/**
 * @brief /proc/stat 에서 cpu time을 계산하여 반환하느 함수
 *
 * @return unsigned long long The total CPU time.
 */
unsigned long long read_total_cpu_time() {
  FILE *file = fopen("/proc/stat", "r");
  if (!file)
    return 0;

  unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
  fscanf(file, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
         &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
  fclose(file);

  return user + nice + system + idle + iowait + irq + softirq + steal;
}

/**
 * @brief /proc 내부의 PID들을 읽어와서 processes 배열에 저장하는 함수
 * 고정된 processes 배열을 반복문을 통해 메모리 할당, 초기화, process 저장 (read_process_info)
 *
 *
 * @param processes The array to store the processes.
 * @param max_count The maximum number of processes to read.
 * @return int The total number of processes read.
 */
int get_all_processes(Process **processes, int max_count) {
  DIR *proc_dir = opendir("/proc");
  if (!proc_dir)
    return -1;

  struct dirent *entry;
  int proc_count = 0;
  unsigned long long total_cpu_time = read_total_cpu_time();
  struct sysinfo info;
  if (sysinfo(&info) == -1) {
    perror("sysinfo");
  }

  while ((entry = readdir(proc_dir)) != NULL && proc_count < max_count) {
    if (isdigit(entry->d_name[0])) {
      processes[proc_count] = malloc(sizeof(Process));
      if (!processes[proc_count]) {
        perror("malloc error");
        closedir(proc_dir);
        return -1;
      }

      // 프로세스 초기화
      processes[proc_count]->children = NULL;
      processes[proc_count]->child_count = 0;
      processes[proc_count]->parent = NULL; // Add parent pointer initialization

      processes[proc_count]->pid = atoi(entry->d_name);
      if (read_process_info(processes[proc_count]) == 0) {
        for (int i = 0; i < proc_count; i++) {
          if (processes[i]->pid == processes[proc_count]->ppid) {
            processes[i]->children = realloc(processes[i]->children,
                                             sizeof(Process *) * (processes[i]->child_count + 1));
            if (!processes[i]->children) {
              perror("Failed to allocate memory for children array");
              free(processes[proc_count]);
              processes[proc_count] = NULL;
              closedir(proc_dir);
              return -1;
            }
            processes[i]->children[processes[i]->child_count++] = processes[proc_count];
            processes[proc_count]->parent = processes[i];

            break;
          }
        }
        processes[proc_count]->mem_usage = ((float)processes[proc_count]->res / (info.totalram / 1024)) * 100.0;
        processes[proc_count]->cpu_usage = ((float)processes[proc_count]->time / total_cpu_time) * 100.0;

        link_userthread(processes[proc_count], processes, &proc_count, info.totalram, total_cpu_time);
        proc_count++;
      }
      else {
        free(processes[proc_count]);
        processes[proc_count] = NULL;
      }
    }
  }

  closedir(proc_dir);
  return proc_count;
}