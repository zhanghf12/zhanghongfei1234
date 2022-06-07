#ifndef _N_NOISE_H
#define _N_NOISE_H

#include "GL.h"
namespace noise
{
    class Noise
    {
    public:
        Noise();
        ~Noise();
        void paste_noise_blob(int i, int j, int k, IplImage* f_print);
        void random_noise(IplImage* f_print);
        void fixed_noise(IplImage* f_print);
        void distortion(const char* filename);
    public:
        int** f_print1_2Dmat;
        int** f_print2_2Dmat;
        int** f_mask_2Dmat;
        IplImage* f_print1;
        IplImage* f_print2;
        IplImage* mask;
        float transmin;
        float transmax;
        float rotmin;
        float rotmax;
        int min_noiseLevel;
        int max_noiseLevel;
    };
}
#endif
