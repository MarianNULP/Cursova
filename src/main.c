#include <stdio.h>
#include <stdint.h>
#include "fft.h"
#include "window.h"
#include "io.h"

#define N 1024

int main() {
    int32_t real_samples[N], imag_samples[N];

    // Імітуємо отримання даних з HOST (читаємо файл, або генеруємо)
    int status = read_input_data("data/input_data.bin", real_samples, imag_samples, N);
    if (status == 1) {
        printf("Data read from file successfully.\n");
    } else if (status == 0) {
        printf("No input file found. Generated test signal.\n");
    } else {
        printf("Error reading input file.\n");
        return -1;
    }

    // Застосовуємо вікно Блекмана
    apply_blackman_window(real_samples, imag_samples, N);

    // Виконуємо FFT
    fft_fixed(real_samples, imag_samples, N);

    // Записуємо результати, імітуючи USB вивід
    if (write_output_data("output/output_spectrum.bin", real_samples, imag_samples, N) == 0) {
        printf("Output data written to output/output_spectrum.bin.\n");
    } else {
        printf("Error writing output data.\n");
    }

    return 0;
}
