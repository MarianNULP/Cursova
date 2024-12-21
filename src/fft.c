#define _USE_MATH_DEFINES
#include <math.h>
#include <stdint.h>
#include "fft.h"

/*
  Фіксована кома: припустимо формат Q12.12: 1 << 12 = 4096.
  Це означає, що int32_t зберігає значення з точкою після 12-го біта.
  Наприклад, щоб отримати double з int32_t (Q12.12):
     double val = (double)fixed_val / (1 << 12);

  Для множення двох Q12.12:
     (A * B) / (1 << 12)  - треба врахувати зсув.

  Тут для обчислення коренів одиниці будемо генерувати їх «на льоту» у double,
  потім перетворювати у Q12.12. У реальній реалізації краще зробити попередні таблиці.

  Реалізація FFT (Cooley-Tukey, Radix-2, прорідження по часу):
  1. Переставляємо індекси бітовим дзеркалюванням.
  2. Виконуємо "метелики".
*/

static void bit_reverse(int32_t *real, int32_t *imag, int length) {
    int j = 0;
    for (int i = 0; i < length; i++) {
        if (i < j) {
            int32_t tr = real[j];
            int32_t ti = imag[j];
            real[j] = real[i];
            imag[j] = imag[i];
            real[i] = tr;
            imag[i] = ti;
        }
        int m = length >> 1;
        while (m > 0 && j >= m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
}

void fft_fixed(int32_t *real, int32_t *imag, int length) {
    // length = 1024 припускаємо степінь 2
    bit_reverse(real, imag, length);

    for (int step = 1; step < length; step <<= 1) {
        int jump = step << 1;
        double angle = -M_PI / (double)step;
        // Коефіцієнти для обчислення експоненти
        int32_t w_real = (int32_t)((cos(angle) * (1 << 12)));
        int32_t w_imag = (int32_t)((sin(angle) * (1 << 12)));

        int32_t wr = (1 << 12);
        int32_t wi = 0;

        for (int group = 0; group < step; group++) {
            for (int pair = group; pair < length; pair += jump) {
                int match = pair + step;
                int32_t r_match = real[match];
                int32_t i_match = imag[match];

                // Множимо (r_match + i_match*j) на (wr + wi*j)
                // Комплексне множення: (r_match*wr - i_match*wi) + j(r_match*wi + i_match*wr)
                int32_t temp_r = (int32_t)(((int64_t)r_match * wr - (int64_t)i_match * wi) >> 12);
                int32_t temp_i = (int32_t)(((int64_t)r_match * wi + (int64_t)i_match * wr) >> 12);

                real[match] = real[pair] - temp_r;
                imag[match] = imag[pair] - temp_i;

                real[pair] += temp_r;
                imag[pair] += temp_i;
            }

            // Оновлюємо wr, wi для наступної пари
            // wr_new = wr * w_real - wi * w_imag
            // wi_new = wr * w_imag + wi * w_real
            int64_t wr_new = ((int64_t)wr * w_real - (int64_t)wi * w_imag) >> 12;
            int64_t wi_new = ((int64_t)wr * w_imag + (int64_t)wi * w_real) >> 12;
            wr = (int32_t)wr_new;
            wi = (int32_t)wi_new;
        }
    }
}
