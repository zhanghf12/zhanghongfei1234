#ifndef _FILTER_IMAGE_H
#define _FILTER_IMAGE_H

#include "global.h"
#include "orientmap.h"
#include "densitymap.h"
namespace zhanghongfei
{
    class Filter_Image : public OrientMap
    {
    public:
        Filter_Image();
        ~Filter_Image();
        void pre_filtering_o(void);
        void pre_filtering_f(void);
        void filter_image(void);
        void filter_image_firstpass(void);
        void filter_image_withmap(void);
        void set_filter_area(void);
        //int f_frint_3[400][275];
    public:
        int** orient_ind_2Dmat;
        int** freq_ind_2Dmat;
        float** filt_map;
        CvMat* mat;
        IplImage* filter;
    };
}
#endif
