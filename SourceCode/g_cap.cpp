#include "g_cap.h"
#include "global.h"
#include "cv_type_converter.h"
//#include "fstream"
using namespace zhanghongfei;

/*
  Parameters for various class of fingerprints.
  */

G_Cap::G_Cap()
{

}

G_Cap::~G_Cap()
{

}

float G_Cap::g_loop1(float alpha) {
    float alpha_temp = M_PI + alpha;
    int q = floor(4 * alpha_temp / pi);

    if (q == 8)
        q = 7;

    float alpha_i = -M_PI + (M_PI * q) / 4;

    return (g_cap[1][1][q + 1] + ((4 * (alpha - alpha_i)) / pi) * (g_cap[1][1][q + 2] - g_cap[1][1][q + 1]));
}

float G_Cap::g_delta1(float alpha) {
    float alpha_temp = M_PI + alpha;
    int q = floor(4 * alpha_temp / pi);

    if (q == 8)
        q = 7;

    float alpha_i = -M_PI + (M_PI * q) / 4;

    return (g_cap[1][2][q + 1] + ((4 * (alpha - alpha_i)) / pi) * (g_cap[1][2][q + 2] - g_cap[1][2][q + 1]));

}

float G_Cap::g_loop2(float alpha) {
    float alpha_temp = M_PI + alpha;
    int q = floor(4 * alpha_temp / pi);

    if (q == 8)
        q = 7;

    float alpha_i = -M_PI + (M_PI * q) / 4;

    return (g_cap[2][1][q + 1] + ((4 * (alpha - alpha_i)) / pi) * (g_cap[2][1][q + 2] - g_cap[2][1][q + 1]));
}

float G_Cap::g_delta2(float alpha) {
    float alpha_temp = M_PI + alpha;
    int q = floor(4 * alpha_temp / pi);

    if (q == 8)
        q = 7;

    float alpha_i = -M_PI + (M_PI * q) / 4;
    return (g_cap[2][2][q + 1] + ((4 * (alpha - alpha_i)) / pi) * (g_cap[2][2][q + 2] - g_cap[2][2][q + 1]));
}