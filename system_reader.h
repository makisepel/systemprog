// system_reader.h
#ifndef SYSTEM_READER_H
#define SYSTEM_READER_H

void read_memory_usage(unsigned long *used, unsigned long *total);
void read_swap_usage(unsigned long *used, unsigned long *total);
void read_load_average(float *load1, float *load5, float *load15);
void read_uptime(double *uptime);

void read_resource(
    unsigned long *mem_used, unsigned long *mem_total,
    unsigned long *swap_used, unsigned long *swap_total,
    float *load1, float *load5, float *load15,
    double *uptime);

#endif
