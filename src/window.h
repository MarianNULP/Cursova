#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

void apply_blackman_window(int32_t *real_samples, int32_t *imag_samples, int length);

#endif
