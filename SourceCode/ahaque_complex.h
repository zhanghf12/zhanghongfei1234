#ifndef _AHAQUE_COMPLEX_H
#define _AHAQUE_COMPLEX_H
namespace zhanghongfei {
	typedef struct {
		float x;
		float y;
	}complex;
	float length(complex *v);
	float dot(complex* v, complex* w);
	float angle(complex *u, complex *v);
	float arg(complex* u);
}

#endif