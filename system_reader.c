#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <string.h>
#include "system_reader.h"

// Read memory usage (used, total) from /proc/meminfo
void read_memory_usage(unsigned long *used, unsigned long *total) {
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) {
        *used = 0;
        *total = 0;
        return;
    }

    unsigned long mem_total = 0, mem_free = 0, buffers = 0, cached = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "MemTotal: %lu kB", &mem_total) == 1) continue;
        if (sscanf(line, "MemFree: %lu kB", &mem_free) == 1) continue;
        if (sscanf(line, "Buffers: %lu kB", &buffers) == 1) continue;
        if (sscanf(line, "Cached: %lu kB", &cached) == 1) continue;
    }
    fclose(file);

    *total = mem_total;
    *used = mem_total - mem_free - buffers - cached;
}

// Read swap usage (used, total) from /proc/meminfo
void read_swap_usage(unsigned long *used, unsigned long *total) {
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) {
        *used = 0;
        *total = 0;
        return;
    }

    unsigned long swap_total = 0, swap_free = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "SwapTotal: %lu kB", &swap_total) == 1) continue;
        if (sscanf(line, "SwapFree: %lu kB", &swap_free) == 1) continue;
    }
    fclose(file);

    *total = swap_total;
    *used = swap_total - swap_free;
}

// Read system load averages from /proc/loadavg
void read_load_average(float *load1, float *load5, float *load15) {
    FILE *file = fopen("/proc/loadavg", "r");
    if (!file) {
        *load1 = *load5 = *load15 = 0.0;
        return;
    }

    fscanf(file, "%f %f %f", load1, load5, load15);
    fclose(file);
}

// Read system uptime from /proc/uptime
void read_uptime(double *uptime) {
    FILE *file = fopen("/proc/uptime", "r");
    if (!file) {
        *uptime = 0.0;
        return;
    }

    fscanf(file, "%lf", uptime);
    fclose(file);
}
