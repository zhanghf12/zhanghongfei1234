#ifndef _NOISE_H
#define _NOISE_H
#include"global.h"

namespace zhanghongfei {
	class Noise {
	public:
		Noise();
		~Noise();
		float ahaq_rand();
#ifdef WIN32
		static int rand_r(unsigned int* seed);
#endif
	public:
		int** f_print1_2Dmat;
		int** f_print2_2Dmat;
		//int** f_print3_2Dmat;
		/*int f_print1_2Dmat[400][275];
		int f_print2_2Dmat[400][275];*/
		//int f_print3_2Dmat[400][275];
		IplImage* f_print1;
		IplImage* f_print2;
		unsigned int rand_seed;
	};
}


#endif
