#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdint.h>
#include "math.h"
#include "io.h"

// Читання вхідних даних з файлу (імітуючи HOST)
// Формат: length 32-бітних значень (int32_t), у Q12.12
int read_input_data(const char* filename, int32_t *real_samples, int32_t *imag_samples, int length) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        // Файл відсутній, згенеруємо тестовий сигнал
        double fs = 48000.0;
        double f_signal = 1000.0;
        for (int i = 0; i < length; i++) {
            double t = (double)i / fs;
            double val = sin(2.0 * M_PI * f_signal * t);
            real_samples[i] = (int32_t)(val * (1 << 12));
            imag_samples[i] = 0;
        }
        return 0; // 0 - означає, що ми згенерували, а не прочитали
    }

    size_t read_count = fread(real_samples, sizeof(int32_t), length, f);
    fclose(f);
    if (read_count != (size_t)length) {
        return -1;
    }
    // Уявимо, що всі дані - реальна частина, уявна = 0
    for (int i = 0; i < length; i++) {
        imag_samples[i] = 0;
    }
    return 1; // 1 - файл прочитаний
}

// Запис вихідних даних (імітуючи USB)
int write_output_data(const char* filename, int32_t *real_samples, int32_t *imag_samples, int length) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return -1;
    }
    fwrite(real_samples, sizeof(int32_t), length, f);
    fwrite(imag_samples, sizeof(int32_t), length, f);
    fclose(f);
    return 0;
}
