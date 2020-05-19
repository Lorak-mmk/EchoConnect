#ifndef ECHOCONNECT_MATH_H
#define ECHOCONNECT_MATH_H

#include <cstdint>

#define PI 3.1415926535

double dft(const int16_t *samples, int len, double ratio);
void dft_slide(const int16_t *samples, int win_len, double ratio, double *out, int len);

#endif
