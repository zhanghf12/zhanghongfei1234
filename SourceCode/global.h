#ifndef _GLOBAL_H
#define _GLOBAL_H

#include<opencv.hpp>
//#include <opencv/cv.h>
//#include <opencv/cvaux.h>
//#include <opencv/highgui.h>
#include<string>
#include<qdatetime>

using namespace std;
//using namespace cv;
#define _USE_MATH_DEFINES
#include <math.h>

namespace zhanghongfei {
#define PARA
#define WRITE
#define WRITE_REF
#define GEN
#define ADD_NOISE
#define ADD_MASK
#define ADD_BLUR

#ifdef MAIN
#define EXTERN
#else
#define EXTERN extern
#endif

    EXTERN QDateTime start_time;
    EXTERN int flag_plot;
    EXTERN int flag_debug;
    EXTERN int k;
    EXTERN int k_fact;
    EXTERN int block_size;
    EXTERN int W;
    EXTERN int H;
    EXTERN int margin;
    EXTERN int padding;
    EXTERN int line_length;
    EXTERN int distnct_f;
    EXTERN int distnct_o;
    EXTERN int max_filter_size;

    EXTERN int density_map_count;

    EXTERN float k_arch;

#ifdef MAIN
    float pi = M_PI;
    float rad_deg_fact = 180.0 / M_PI;
    float deg_rad_fact = M_PI / 180.0;
    bool noise_blobs_loaded = false;
    bool filters_loaded = false;
#else
    extern float pi, rad_deg_fact, deg_rad_fact;
    extern bool noise_blobs_loaded;
    extern bool filters_loaded;
#endif

    EXTERN int archN, tarchN, rloopN, lloopN, dloopN, whorlN;

    EXTERN IplImage* noise_blob[11];

    EXTERN string path_o_f, path_o_ref, path_M_F, path_impr, cfg_path;
    EXTERN string path_filter, path_density_maps, path_fing;
    EXTERN string path_noise_in;
    EXTERN int n_impr, no_fin, size_fo, n_threads;
    EXTERN bool create_filters, create_den_maps, save_meta, is_GUI, noise_only, in_dir_used, use_cfg;
    EXTERN int generated, start_from_finger, class_distribution, Wid;
    EXTERN int**** filterbank_4Dmat;
    EXTERN int filter_size_2Dmat[100][180];
    EXTERN string image_type;
    EXTERN unsigned int custom_seed;
    EXTERN bool use_custom_seed;
    EXTERN int rotation;
    EXTERN int translation;
    EXTERN int min_noise, max_noise;
    EXTERN int min_scratches, max_scratches;
}

#endif
