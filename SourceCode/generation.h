#ifndef _GENERATION_H
#define _GENERATION_H

#include "global.h"
#include "filter_image.h"
#include "g_cap.h"
//#include "Noise.h"
#include"n_noise.h"
//#include "orientmap.h"
//#include "densitymap.h"
namespace zhanghongfei
{
    class Generator : public Filter_Image
    {
    public:
        Generator();
        ~Generator();
        void generation(void);
        /*void gg();*/
        void impression(IplImage* f_print, int n_impr);
        void set_low_den_para();
        void seed_pos();
        void set_param();
        //void transformation(void);
        void init_para(void);
    public:
        float** temp_fp_2Dmat;
        IplImage* f_print_para;
        unsigned int generation_seed;
    private:
        noise::Noise* noise_obj;
    };
}
#endif

