#include "Dft.h"
#include <cmath>
static constexpr double PI = 3.14159265358979323846;

static double mag(double re, double im) {
    return sqrt(re * re + im * im);
}

double dft(const int16_t *samples, int len, double ratio) {
    double re = 0;
    double im = 0;
    double angle = 0;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    double d_angle = 2.0 * PI * ratio;

    for (int i = 0; i < len; i++) {
        double val = samples[i];
        re += val * std::cos(angle);
        im += val * std::sin(angle);
        angle += d_angle;
    }

    return mag(re, im) / static_cast<double>(len);
}

void dft_slide(const int16_t *samples, int win_len, double ratio, double *out, int len) {
    double re = 0;
    double im = 0;
    double angle0 = 0;
    double angle1 = 0;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    double d_angle = 2.0 * PI * ratio;

    for (int i = 0; i < win_len; i++) {
        re += samples[i] * std::cos(angle1);
        im += samples[i] * std::sin(angle1);
        angle1 += d_angle;
    }
    out[0] = mag(re, im) / static_cast<double>(win_len);

    for (int i = 0; i < len - 1; i++) {
        re -= samples[i] * std::cos(angle0);
        im -= samples[i] * std::sin(angle0);
        re += samples[i + win_len] * std::cos(angle1);
        im += samples[i + win_len] * std::sin(angle1);
        out[i + 1] = mag(re, im) / static_cast<double>(win_len);
        angle0 += d_angle;
        angle1 += d_angle;
    }
}
