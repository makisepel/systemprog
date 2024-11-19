/*
    system_reader.c는 시스템 리소스를 읽어오는 코드입니다.
    main 함수에서 선언된 변수들의 값을 변경합니다.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <string.h>
#include "system_reader.h"

/**
 * @brief Reads memory usage from /proc/meminfo
 * 
 * @param used Pointer to store the used memory.
 * @param total Pointer to store the total memory.
 */
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

/**
 * @brief Reads swap usage from /proc/meminfo
 * 
 * @param used Pointer to store the used swap memory.
 * @param total Pointer to store the total swap memory.
 */
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

/**
 * @brief Reads system load averages from /proc/loadavg.
 * 
 * @param load_by_1min Pointer to store the 1-minute load average.
 * @param load_by_5min Pointer to store the 5-minute load average.
 * @param load_by_15min Pointer to store the 15-minute load average.
 */
void read_load_average(float *load_by_1min, float *load_by_5min, float *load_by_15min) {
    FILE *file = fopen("/proc/loadavg", "r");
    if (!file) {
        *load_by_1min = *load_by_5min = *load_by_15min = 0.0;
        return;
    }

    fscanf(file, "%f %f %f", load_by_1min, load_by_5min, load_by_15min);
    fclose(file);
}

/**
 * @brief Reads system uptime from /proc/uptime.
 * 
 * @param uptime Pointer to store the system uptime in seconds.
 */
void read_uptime(double *uptime) {
    FILE *file = fopen("/proc/uptime", "r");
    if (!file) {
        *uptime = 0.0;
        return;
    }

    fscanf(file, "%lf", uptime);
    fclose(file);
}

/**
 * @brief Unified function to call individual resource-reading functions.
 * 
 * @param mem_used Pointer to store the used memory.
 * @param mem_total Pointer to store the total memory.
 * @param swap_used Pointer to store the used swap memory.
 * @param swap_total Pointer to store the total swap memory.
 * @param load_by_1min Pointer to store the 1-minute load average.
 * @param load_by_5min Pointer to store the 5-minute load average.
 * @param load_by_15min Pointer to store the 15-minute load average.
 * @param uptime Pointer to store the system uptime in seconds.
 */
void read_resource(
    unsigned long *mem_used, unsigned long *mem_total,
    unsigned long *swap_used, unsigned long *swap_total,
    float *load_by_1min, float *load_by_5min, float *load_by_15min,
    double *uptime) {

    read_memory_usage(mem_used, mem_total);
    read_swap_usage(swap_used, swap_total);
    read_load_average(load_by_1min, load_by_5min, load_by_15min);
    read_uptime(uptime);
}