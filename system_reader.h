// system_reader.h
#ifndef SYSTEM_READER_H
#define SYSTEM_READER_H

void read_memory_usage(unsigned long *used, unsigned long *total);
void read_swap_usage(unsigned long *used, unsigned long *total);
void read_load_average(float *load1, float *load5, float *load15);
void read_uptime(double *uptime);

#endif
