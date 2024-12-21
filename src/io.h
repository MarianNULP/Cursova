#ifndef IO_H
#define IO_H

#include <stdint.h>

int read_input_data(const char* filename, int32_t *real_samples, int32_t *imag_samples, int length);
int write_output_data(const char* filename, int32_t *real_samples, int32_t *imag_samples, int length);

#endif
