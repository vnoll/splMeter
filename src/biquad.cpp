#include "../inc/biquad.h"

double Biquad::filter(double in) {
    double out = z1 + b0* in;
    z1 = z2 + b1 * in - a1 * out;
    z2 = b2 * in - a2 * out;
    return out;
}