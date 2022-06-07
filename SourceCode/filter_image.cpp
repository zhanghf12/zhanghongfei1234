#include "filter_image.h"
#include "global.h"
#include "cv_type_converter.h"
//#include "fstream"
using namespace zhanghongfei;

/*
  Constructor
  */
Filter_Image::Filter_Image()
{
    int i, j;

    orient_ind_2Dmat = new int* [H + margin];
    for (i = 0; i < H + margin; i++)
        orient_ind_2Dmat[i] = new int[W + margin];

    freq_ind_2Dmat = new int* [H + margin];
    for (i = 0; i < H + margin; i++)
        freq_ind_2Dmat[i] = new int[W + margin];


    filt_map = new float* [H + margin];
    for (i = 0; i < H + margin; i++)
        filt_map[i] = new float[W + margin];


    for (i = 0; i < H + margin; i++)
        for (j = 0; j < W + margin; j++)
            filt_map[i][j] = 0;

    /*for (i = 0; i < 101; i++)
        for (j = 0; j < 101; j++)
            mul_mat[i][j] = (i) * (j - 46);*/
    /*f_print3_2Dmat = new int* [H + margin];
    for (i = 0; i < H + margin; i++)
        f_print3_2Dmat[i] = new int[W + margin];*/
}

/*
  Destructor
  */
Filter_Image::~Filter_Image()
{
    int i;

    for (i = 0; i < H + margin; i++)
        delete[] orient_ind_2Dmat[i];
    delete[] orient_ind_2Dmat;

    for (i = 0; i < H + margin; i++)
        delete[] freq_ind_2Dmat[i];
    delete[] freq_ind_2Dmat;

    for (i = 0; i < H + margin; i++)
        delete[] filt_map[i];
    delete[] filt_map;

    /*for (i = 0; i < H + margin; i++)
        delete[] f_print3_2Dmat[i];
    delete[] f_print3_2Dmat;*/
}

/* Filters the image with gabor filters.
   */
void Filter_Image::pre_filtering_o(void) {
    int i, j;

    for (i = 0; i < H + margin; i++) {
        for (j = 0; j < W + margin; j++) {
            orient_ind_2Dmat[i][j] = (int(orient[i][j] * 180 / M_PI)) % distnct_o;
            //freq_ind_2Dmat[i][j] = 100 - (int(f_den_2Dmat[i][j] * 100)) % distnct_f;
        }
    }
}

void Filter_Image::pre_filtering_f(void) {
    int i, j;

    for (i = 0; i < H + margin; i++) {
        for (j = 0; j < W + margin; j++) {
            //orient_ind_2Dmat[i][j] = (int(orient[i][j] * 180 / M_PI)) % distnct_o;
            freq_ind_2Dmat[i][j] = 100 - (int(f_den_2Dmat[i][j] * 100)) % distnct_f;
        }
    }
}

/*
  Applies Gabor Filter.
 */
void Filter_Image::filter_image(void) {
    int i, j;
    int filt_size_half;
    int r, c;
    int tmp1;
    int filter_size;

    //for each point in image
    for (i = floor(max_filter_size / 2.0); i < H + margin - ceil(max_filter_size / 2.0); i++) {

        for (j = floor(max_filter_size / 2.0); j < W + margin - ceil(max_filter_size / 2.0); j++) {
                {
                    filter_size = filter_size_2Dmat[freq_ind_2Dmat[i][j]][orient_ind_2Dmat[i][j]];
                    filt_size_half = (int)floor(filter_size / 2.0);

                    tmp1 = 0;
                    for (r = 0; r < filter_size; r++) {
                        for (c = 0; c < filter_size; c++) {
//#ifndef IDX
                            int ind1 = (i + r - filt_size_half) > 0 ? (i + r - filt_size_half) : 0;
                            int ind2 = (j + c - filt_size_half) > 0 ? (j + c - filt_size_half) : 0;
                            ind1 = ind1 < H + margin ? ind1 : H + margin - 1;
                            ind2 = ind2 < W + margin ? ind2 : W + margin - 1;
                            tmp1 = tmp1 + f_print1_2Dmat[ind1][ind2] * filterbank_4Dmat[freq_ind_2Dmat[i][j]][orient_ind_2Dmat[i][j]][r][c];


                        }
                    }
                    f_print2_2Dmat[i][j] = tmp1;
                }
        }
    }

    copy2Df_print(f_print2_2Dmat, f_print1_2Dmat);
    normalize2Df_print(f_print1_2Dmat);//[0..100]

    binarize2Df_print(f_print1_2Dmat, 55);
    normalize2Df_print(f_print1_2Dmat);//[0..100]
}


void Filter_Image::filter_image_firstpass(void) {
    int i, j;
    int filt_size_half;
    int r, c;
    int tmp1;
    int filter_size;
    //int s, v;
    //int f_frint_3[400][275];
    /*int** f_print3_2Dmat;
    f_print3_2Dmat = (int**)malloc((H + margin) * sizeof(int*));
    for (i = 0; i < H + margin; i++)
        f_print3_2Dmat[i] = (int*)malloc((W + margin) * sizeof(int));
    copy2Df_print(f_print1_2Dmat, f_print3_2Dmat);*/
    //int freq_ind, orient_ind;
    //filterbank_4Dmat = (int****)malloc((distnct_f) * sizeof(int***));

    //for (int i = 0; i < distnct_f; i++)
    //    filterbank_4Dmat[i] = (int***)malloc((distnct_o) * sizeof(int**));
    //for (freq_ind = 0; freq_ind < distnct_f; freq_ind++) {
    //    for (orient_ind = 0; orient_ind < distnct_o; orient_ind++) {

    //        //sprintf(s1, "%s/%d/%d.bmp", path_filter, freq_ind + 1, orient_ind + 1);
    //        //std::stringstream s1;
    //        //s1 << path_filter << "/" << freq_ind + 1 << "/" << orient_ind + 1 << ".bmp";
    //        //filter = cvLoadImage(s1.str().c_str(), CV_LOAD_IMAGE_GRAYSCALE);
    //        //mat = cvCreateMat(filter->height, filter->width, CV_32FC1);
    //        //ipl8uc1_to_cv32fc1(filter, mat);

    //        //cvConvertScale(mat, mat, 1 / 255.0, 0);
    //        //normalize32(mat);
    //        //cvConvertScale(mat, mat, 100, -46);


    //        filter_size = 31;
    //        //filter_size_2Dmat[freq_ind][orient_ind] = filter_size;


    //        filterbank_4Dmat[freq_ind][orient_ind] = (int**)malloc((filter_size) * sizeof(float*));
    //        for (int k = 0; k < filter_size; k++)
    //            filterbank_4Dmat[freq_ind][orient_ind][k] = (int*)malloc((filter_size) * sizeof(float));
    //        for (r = 0; r < 31; r++) {
    //            for (c = 0; c < 31; c++) {
    //                filterbank_4Dmat[freq_ind][orient_ind][r][c] = 0;
    //            }
    //        }
    //        //cvmattomat2D(mat, filterbank_4Dmat[freq_ind][orient_ind]);

    //        /*if (filter_size > max_filter_size)
    //            max_filter_size = filter_size;

    //        cvReleaseImage(&filter);
    //        cvReleaseMat(&mat);*/
    //    }
    //}
    binarize2Df_print(f_print1_2Dmat, 65);
    normalize2Df_print(f_print1_2Dmat);//[0..100]

    for (i = floor(max_filter_size / 2.0); i < H + margin - ceil(max_filter_size / 2.0); i++) {
        for (j = floor(max_filter_size / 2.0); j < W + margin - ceil(max_filter_size / 2.0); j++) {

            if (filt_map[i][j] == 1) {
                /*freq_ind_2Dmat[i][j] = 1;
                orient_ind_2Dmat[i][j] = 1;*/
                filter_size = filter_size_2Dmat[freq_ind_2Dmat[i][j]][orient_ind_2Dmat[i][j]];
                filt_size_half = (int)floor(filter_size / 2.0);

                tmp1 = 0;
                for (r = 0; r < filter_size; r++) {
                    for (c = 0; c < filter_size; c++) {
//#ifndef IDX
                        int ind1 = (i + r - filt_size_half) > 0 ? (i + r - filt_size_half) : 0;
                        int ind2 = (j + c - filt_size_half) > 0 ? (j + c - filt_size_half) : 0;
                        ind1 = ind1 < H + margin ? ind1 : H + margin - 1;
                        ind2 = ind2 < W + margin ? ind2 : W + margin - 1;
                        //s = f_print1_2Dmat[ind1][ind2];
                        //v = filterbank_4Dmat[freq_ind_2Dmat[i][j]][orient_ind_2Dmat[i][j]][r][c];
                        //filterbank_4Dmat[freq_ind_2Dmat[i][j]][orient_ind_2Dmat[i][j]][r][c] = 0;
                        tmp1 = tmp1 + f_print1_2Dmat[ind1][ind2] * filterbank_4Dmat[freq_ind_2Dmat[i][j]][orient_ind_2Dmat[i][j]][r][c];
                        //tmp1 = tmp1 + s * v;
                        
//#endif

                    }
                }
                f_print2_2Dmat[i][j] = tmp1;
                //f_print3_2Dmat[i][j] = tmp1;
                //f_frint_3[i][j] = tmp1;
                //f_print2_2Dmat[i][j] = f_frint_3[i][j];
            }
        }
    }
    /*for (r = 0; r < H + margin; r++) {
        for (c = 0; c < W + margin; c++) {
            f_print1_2Dmat[r][c] = f_frint_3[r][c];

        }
    }*/
    copy2Df_print(f_print2_2Dmat, f_print1_2Dmat);
    normalize2Df_print(f_print1_2Dmat);//[0..100]
    binarize2Df_print(f_print1_2Dmat, 55);

}

void Filter_Image::filter_image_withmap(void) {
    int i, j;
    int filt_size_half;
    int r, c;
    int tmp1;
    int filter_size;

    for (i = floor(max_filter_size / 2.0); i < H + margin - ceil(max_filter_size / 2.0); i++) {
        for (j = floor(max_filter_size / 2.0); j < W + margin - ceil(max_filter_size / 2.0); j++) {
            if (filt_map[i][j] == 1) {
                filter_size = filter_size_2Dmat[freq_ind_2Dmat[i][j]][orient_ind_2Dmat[i][j]];
                filt_size_half = (int)floor(filter_size / 2.0);

                tmp1 = 0;
                for (r = 0; r < filter_size; r++) {
                    for (c = 0; c < filter_size; c++) {
//#ifndef IDX
                        int ind1 = (i + r - filt_size_half) > 0 ? (i + r - filt_size_half) : 0;
                        int ind2 = (j + c - filt_size_half) > 0 ? (j + c - filt_size_half) : 0;
                        ind1 = ind1 < H + margin ? ind1 : H + margin - 1;
                        ind2 = ind2 < W + margin ? ind2 : W + margin - 1;
                        tmp1 = tmp1 + f_print1_2Dmat[ind1][ind2] * filterbank_4Dmat[freq_ind_2Dmat[i][j]][orient_ind_2Dmat[i][j]][r][c];
//#endif
                    }
                }
                f_print2_2Dmat[i][j] = tmp1;
            }
        }
    }

    copy2Df_print(f_print2_2Dmat, f_print1_2Dmat);
    normalize2Df_print(f_print1_2Dmat);//[0..100]

    binarize2Df_print(f_print1_2Dmat, 55);
}

/*
  Initializes the are which is to be filtered.
  */
void Filter_Image::set_filter_area(void) {
    int i, j, k;
    int flag_in;

    for (i = 0; i < H + margin; i++) {
        flag_in = 0;
        for (j = 0; j < W + margin; j++) {
            if (filt_map[i][j] == 1 && flag_in == 0) {
                for (k = j - floor(max_filter_size / 2.0); k < j; k++) {
                    if (k > 0)
                    {
                        filt_map[i][k] = 1;
                    }
                }
                flag_in = 1;
            }

            if (filt_map[i][j] == 0 && flag_in == 1) {
                for (k = j; k <= j + floor(max_filter_size / 2.0); k++) {
                    if (k < W + margin) {
                        filt_map[i][k] = 1;
                    }
                }

                j = j + floor(max_filter_size / 2.0) + 1;
                flag_in = 0;
                continue;
            }
        }
    }

    //for each column .. do modification
    for (j = 0; j <= W + margin; j++) {
        flag_in = 0;

        for (i = 0; i < H + margin; i++) {

            if (filt_map[i][j] == 1 && flag_in == 0) {
                for (k = i - floor(max_filter_size / 2.0); k < i; k++) {
                    if (k > 0)
                    {
                        filt_map[k][j] = 1;
                    }
                }
                flag_in = 1;
            }

            if (filt_map[i][j] == 0 && flag_in == 1) {
                for (k = i; k <= i + floor(max_filter_size / 2.0); k++) {
                    if (k < H + margin) {
                        filt_map[k][j] = 1;
                    }
                }

                i = i + floor(max_filter_size / 2.0) + 1;
                flag_in = 0;
                continue;
            }
        }
    }
}
