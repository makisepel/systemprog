#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/sysinfo.h>
#include "proc_reader.h"


int read_process_info(Process *proc) {
    char path[256];
    FILE *file;
    int isKernalThread = 0;

    /**
    * /proc/[PID]/cmdline
    * To check whether process is kernal thread or not
    */
    snprintf(path, sizeof(path), "/proc/%d/cmdline", proc->pid);
    file = fopen(path, "r");
    if (file) {
        if (fgets(proc->command, sizeof(proc->command), file) == NULL) {
            isKernalThread = 1;
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
    fscanf(file, "%*d (%[^)]) %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %d %d %*d %*u %lu",
        comm, &proc->state, &utime, &stime, &proc->priority, &proc->nice, &starttime);

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
    if (isKernalThread) {
            /**
             * Kernal Thread's UID is generally 0 (that points root) 
             * To reduce using memory, if Process is Kernal Thread 
             * avoiding iteration will reduce memory resources?
             */
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

unsigned long long read_total_cpu_time() {
    FILE *file = fopen("/proc/stat", "r");
    if (!file) return 0;

    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    fscanf(file, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
           &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
    fclose(file);

    return user + nice + system + idle + iowait + irq + softirq + steal;
}

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
            processes[count]->pid = atoi(entry->d_name);
            if (read_process_info(processes[count]) == 0) {
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
