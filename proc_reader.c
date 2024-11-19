#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/sysinfo.h>
#include "proc_reader.h"

// Add a child to the parent's children array
void add_child(Process *parent, Process *child) {
    parent->children = realloc(parent->children, sizeof(Process *) * (parent->child_count + 1));
    if (!parent->children) {
        perror("Failed to allocate memory for children");
        exit(EXIT_FAILURE);
    }

    parent->children[parent->child_count++] = (struct Process *)child;
}

// Read one process
int read_process_info(Process *proc) {
    char path[256];
    FILE *file;
    proc->isKernalThread = 0;

    /**
    * /proc/[PID]/cmdline
    * To check whether process is kernal thread or not
    */
    snprintf(path, sizeof(path), "/proc/%d/cmdline", proc->pid);
    file = fopen(path, "r");
    if (file) {
        if (fgets(proc->command, sizeof(proc->command), file) == NULL) {
            proc->isKernalThread = 1;
        }
        fclose(file);
    } else {
        strncpy(proc->command, "[unknown]", sizeof(proc->command));
    }

    /**
    * /proc/[PID]/stat
    * pid, priority, nice, state, cpu_usage, command 
    */
    snprintf(path, sizeof(path), "/proc/%d/stat", proc->pid);
    file = fopen(path, "r");
    if (!file) return -1;

    char comm[256];
    unsigned long utime, stime, starttime;
    fscanf(file, "%d (%[^)]) %c %d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %d %d %*d %*u %lu",
            &proc->pid,          // PID: 1st field
            comm,                // Command: 2nd field (wrapped in parentheses)
            &proc->state,        // State: 3rd field
            &proc->ppid,         // PPID: 4th field
            &utime,              // utime: 14th field
            &stime,              // stime: 15th field
            &proc->priority,     // Priority: 18th field
            &proc->nice,         // Nice: 19th field
            &starttime);         // Start time: 22nd field
    strncpy(proc->command, comm, sizeof(proc->command));
    proc->time = (utime + stime) / sysconf(_SC_CLK_TCK);
    fclose(file);



    /**
    * /proc/[PID]/status
    * user, time, virt, res, shr
    */
    snprintf(path, sizeof(path), "/proc/%d/status", proc->pid);
    file = fopen(path, "r");
    if (!file) return -1;

    char line[256];
    int uid = -1;
    /**
    * Kernal Thread's UID is generally 0 (that points root) 
    * To reduce using memory, if Process is Kernal Thread 
    * avoiding iteration will reduce memory resources?
    */
    if (proc->isKernalThread) {
            strncpy(proc->user, "root", sizeof(proc->user));
            proc->virt = 0;
            proc->res = 0;
            proc->shr = 0;
        }
    else {
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
    }
    fclose(file);

    return 0;
}

// Read total CPU time
unsigned long long read_total_cpu_time() {
    FILE *file = fopen("/proc/stat", "r");
    if (!file) return 0;

    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    fscanf(file, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
           &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
    fclose(file);

    return user + nice + system + idle + iowait + irq + softirq + steal;
}

// Read all processes by using read_process_info function
int get_all_processes(Process **processes, int max_count) {
    DIR *proc_dir = opendir("/proc");
    if (!proc_dir) return -1;

    struct dirent *entry;
    int count = 0;
    struct sysinfo info;
    unsigned long long total_cpu_time = read_total_cpu_time();
    if (sysinfo(&info) == -1) {
        perror("sysinfo");
    }

    while ((entry = readdir(proc_dir)) != NULL && count < max_count) {
        if (isdigit(entry->d_name[0])) {
            processes[count] = malloc(sizeof(Process));
            if (!processes[count]) {
                perror("malloc error");
                closedir(proc_dir);
                return -1;
            }

            processes[count]->children = NULL;
            processes[count]->child_count = 0;

            processes[count]->pid = atoi(entry->d_name);
            if (read_process_info(processes[count]) == 0) {
                for (int i = 0; i < count; i++) {
                    if (processes[i]->pid == processes[count]->ppid) {
                        processes[i]->children = realloc(processes[i]->children, 
                                                         sizeof(Process *) * (processes[i]->child_count + 1));
                        if (!processes[i]->children) {
                            perror("Failed to allocate memory for children array");
                            free(processes[count]);
                            closedir(proc_dir);
                            return -1;
                        }
                        processes[i]->children[processes[i]->child_count++] = processes[count];
                        break;
                    }
                }
                add_children_from_tasks(processes[count], processes, &count);

                processes[count]->mem_usage = ((float)processes[count]->res / (info.totalram / 1024)) * 100.0;
                processes[count]->cpu_usage = ((float)processes[count]->time / total_cpu_time) * 100.0;
                count++;
            } else {
                free(processes[count]);
            }
        }
    }

    closedir(proc_dir);
    return count;
}

void add_children_from_tasks(Process *proc, Process **processes, int *cnt)
{
    if (!proc) return;

    // 초기화 보장
    if (proc->children == NULL) proc->child_count = 0;

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
            if (tid != proc->pid) { // 자기 자신 제외
                Process *child_thread = malloc(sizeof(Process));
                if (!child_thread) {
                    perror("Failed to allocate memory for child thread");
                    closedir(task_dir);
                    return;
                }

                // 새 child_thread 초기화
                memset(child_thread, 0, sizeof(Process));
                child_thread->pid = tid;
                if (read_process_info(child_thread) == 0) {
                    processes[++(*cnt)] = child_thread;
                    // add_child 함수 사용
                    add_child(proc, child_thread);
                }
                else {
                    free(child_thread);
                }
            }
        }
    }

    closedir(task_dir);
}