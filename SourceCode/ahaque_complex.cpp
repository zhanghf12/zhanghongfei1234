#include "ahaque_complex.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "fstream"
/*
    Methods for handing complex numbers.
*/
namespace zhanghongfei
{
    float length(complex* v) {
        return (sqrt(v->x * v->x + v->y * v->y));
    }


    float dot(complex* v, complex* w) {
        return (v->x * w->x + v->y * w->y);
    }

    float angle(complex* u, complex* v) {
        float result, l1, l2, dotproduct;

        l1 = length(u);
        l2 = length(v);
        dotproduct = dot(u, v);

        result = dotproduct / (l1 * l2);
        result = acos(result);

        return (result);
    }

    float arg(complex* u) {
        float result;
        result = atan2(u->y, u->x);
        /*result = atan(u->y / u->x);*/
        return (result);
    }
}