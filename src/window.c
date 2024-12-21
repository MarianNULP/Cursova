#define _USE_MATH_DEFINES
#include <math.h>
#include <stdint.h>
#include "window.h"

// Застосування вікна Блекмана до реальних даних (imag_samples = 0)
void apply_blackman_window(int32_t *real_samples, int32_t *imag_samples, int length) {
    for (int i = 0; i < length; i++) {
        double w = 0.42 
                   - 0.5 * cos((2.0 * M_PI * i) / (length - 1)) 
                   + 0.08 * cos((4.0 * M_PI * i) / (length - 1));
        double val = (double)real_samples[i] / (1 << 12);
        val *= w;
        real_samples[i] = (int32_t)(val * (1 << 12));
        imag_samples[i] = 0; 
    }
}
