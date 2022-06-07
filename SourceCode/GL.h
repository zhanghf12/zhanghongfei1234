#ifndef _N_GLOBAL_H
#define _N_GLOBAL_H

//#include <opencv/cv.h>
//#include <opencv/cvaux.h>
//#include <opencv/highgui.h>
#include<opencv.hpp>
//using namespace cv;
namespace noise
{
#define WRITE
#define GEN
#define ADD_NOISE
#define ADD_MASK
#define ADD_BLUR
#define FINE_EDGE
#define WRITE_REF

#ifdef MAIN
#define EXTERN

	bool noise_blobs_loaded = false;
	float ahaq_rand()
	{
		return(rand() % 1000 / 999.0);
	}
#else
#define EXTERN extern
	extern float ahaq_rand();
	extern bool noise_blobs_loaded;
#endif

	//integer variables

#ifdef MAIN
	int k = 16;
	int k_fact = k;
	int block_size = k;

	int margin = 30;
	int padding = 15;

	int W = 275 - margin;
	int H = 400 - margin;
#else
	extern int k;
	extern int k_fact;
	extern int block_size;
	extern int W;
	extern int H;
	extern int margin;
	extern int padding;
#endif

#ifdef MAIN
	float pi = M_PI;
	float i_pi = 1 / pi;
	float i_180 = 1 / 180.0;
	float rad_deg_fact = 180.0 / M_PI;
	float deg_rad_fact = M_PI / 180.0;
#else
	extern float pi;
	extern float i_pi;
	extern float i_180;
	extern float rad_deg_fact;
	extern float deg_rad_fact;
#endif
	EXTERN IplImage* noise_blob[11];
}
#endif
