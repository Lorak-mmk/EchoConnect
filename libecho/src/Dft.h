#ifndef ECHOCONNECT_DFT_H
#define ECHOCONNECT_DFT_H

#include <cstdint>

double dft(const int16_t *samples, int len, double ratio);
void dft_slide(const int16_t *samples, int win_len, double ratio, double *out, int len);

#endif
