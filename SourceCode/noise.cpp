#include "noise.h"
#include "cv_type_converter.h"
#include <string>
#include <iostream>
using namespace std;

using namespace zhanghongfei;
Noise::Noise()
{
    int i;
    f_print1 = cvCreateImage(cvSize(W + margin, H + margin), IPL_DEPTH_32F, 1);
    f_print2 = cvCreateImage(cvSize(W + margin, H + margin), IPL_DEPTH_32F, 1);

    f_print1_2Dmat = new int* [H + margin];
    for (i = 0; i < H + margin; i++)
        f_print1_2Dmat[i] = new int[W + margin];
   
    f_print2_2Dmat = new int* [H + margin];
    for (i = 0; i < H + margin; i++)
        f_print2_2Dmat[i] = new int[W + margin];

    /*f_print3_2Dmat = (int**)malloc((H + margin) * sizeof(int*));
    for (i = 0; i < H + margin; i++)
        f_print3_2Dmat[i] = (int*)malloc((W + margin) * sizeof(int));*/
}

Noise::~Noise()
{
    int i;
    cvReleaseImage(&f_print1);
    cvReleaseImage(&f_print2);

    for (i = 0; i < H + margin; i++)
        delete[] f_print1_2Dmat[i];
    delete[] f_print1_2Dmat;

    for (i = 0; i < H + margin; i++)
        delete[] f_print2_2Dmat[i];
    delete[] f_print2_2Dmat;

    /*for (i = 0; i < H + margin; i++)
        free(f_print3_2Dmat[i]);
    free(f_print3_2Dmat);*/

    //printf("distructor Noise \n");
    /*int freq_ind, orient_ind, filter_size;
    for (freq_ind = 0; freq_ind < distnct_f; freq_ind++) {
        for (orient_ind = 0; orient_ind < distnct_o; orient_ind++) {

            filter_size = filter_size_2Dmat[freq_ind][orient_ind];

            for (k = 0; k < filter_size; k++)
                delete(filterbank_4Dmat[freq_ind][orient_ind][k]);

            delete(filterbank_4Dmat[freq_ind][orient_ind]);
        }
    }

    for (i = 0; i < distnct_f; i++)
        delete(filterbank_4Dmat[i]);

    delete(filterbank_4Dmat);*/
}

#ifdef WIN32
/*
  rand_r function for windows.
  */
int Noise::rand_r(unsigned int* seed)
{
    unsigned int next = *seed;
    int result;

    next *= 1103515245;
    next += 12345;
    result = (unsigned int)(next / 65536) % 2048;

    next *= 1103515245;
    next += 12345;
    result <<= 10;
    result ^= (unsigned int)(next / 65536) % 1024;

    next *= 1103515245;
    next += 12345;
    result <<= 10;
    result ^= (unsigned int)(next / 65536) % 1024;

    *seed = next;

    return result;
}
#endif

/*
  Generates random number from 0-1.
  */
float Noise::ahaq_rand()
{
    return((float)(rand_r(&rand_seed) % 1000) / 999.0);
}