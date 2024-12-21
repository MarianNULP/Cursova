import numpy as np

N = 1024
data = np.fromfile("output/output_spectrum.bin", dtype=np.int32, count=2*N)
real_part = data[0:N]
imag_part = data[N:2*N]

# Перетворимо з Q12.12 у плаваючу точку
real_float = real_part / (2**12)
imag_float = imag_part / (2**12)

# Обчислимо амплітудний спектр
amplitude = np.sqrt(real_float**2 + imag_float**2)
phase = np.angle(real_float + 1j*imag_float)

print("Перші 10 значень амплітудного спектру:")
print(amplitude[:10])
