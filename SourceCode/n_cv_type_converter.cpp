#include "n_cv_type_converter.h"
namespace noise
{
    // Convert 3-channel IplImage to 32bit floating point matrix
    void ipl8uc3_to_cv32fc1(IplImage* src, int chn, CvMat* dst) {
        int r, c;
        cvSetImageCOI(src, chn);

        assert(src->width == dst->cols);
        assert(src->height == dst->rows);

        for (r = 0; r < dst->rows; r++)
            for (c = 0; c < dst->cols; c++)
                cvmSet(dst, r, c, (float)cvGetReal2D(src, r, c));
    }

    // Convert 1-channel IplImage to 32bit floating point matrix
    void ipl8uc1_to_cv32fc1(IplImage* src, CvMat* dst) {
        int r, c;

        assert(src->width == dst->cols);
        assert(src->height == dst->rows);

        for (r = 0; r < dst->rows; r++)
            for (c = 0; c < dst->cols; c++)
                cvmSet(dst, r, c, (float)cvGetReal2D(src, r, c));
    }

    // Convert 3-channel IplImage to 8bit unsigned matrix
    void ipl8uc3_to_cv8uc1(IplImage* src, int chn, CvMat* dst) {
        assert(src->width == dst->cols);
        assert(src->height == dst->rows);

        cvSetImageCOI(src, chn);
        cvCopy(src, dst);
    }

    // Convert 1-channel IplImage to 8bit unsigned matrix
    void ipl8uc1_to_cv8uc1(IplImage* src, CvMat* dst) {
        assert(src->width == dst->cols);
        assert(src->height == dst->rows);

        cvCopy(src, dst);
    }

    // Convert 32bit float matrix to 8bit matrix
    void cv32fc1_to_cv8uc1(CvMat* src, CvMat* dst) {
        int r, c;
        for (r = 0; r < dst->rows; r++)
            for (c = 0; c < dst->cols; c++)
                cvSetReal2D(dst, r, c, (unsigned char)cvmGet(src, r, c));
    }

    // Convert 8bit matrix to 32bit float matrix
    void cv8uc1_to_cv32fc1(CvMat* src, CvMat* dst) {
        int r, c;
        for (r = 0; r < dst->rows; r++)
            for (c = 0; c < dst->cols; c++)
                cvmSet(dst, r, c, cvGetReal2D(src, r, c));
    }

    // find minimum in 8bit matrix
    unsigned char min8(CvMat* img) {
        int r, c;
        unsigned char tmp, res = (unsigned char)cvGetReal2D(img, 0, 0);

        for (r = 0; r < img->rows; r++)
            for (c = 0; c < img->cols; c++) {
                tmp = (unsigned char)cvGetReal2D(img, r, c);
                if (tmp < res)
                    res = tmp;
            }

        return res;
    }

    // find minumum in 32bit float matrix
    float min32(CvMat* img) {
        int r, c;
        float tmp, res = cvmGet(img, 0, 0);

        for (r = 0; r < img->rows; r++)
            for (c = 0; c < img->cols; c++) {
                tmp = cvmGet(img, r, c);
                if (tmp < res)
                    res = tmp;
            }

        return res;
    }

    // find maximum in 8bit matrix
    unsigned char max8(CvMat* img) {
        int r, c;
        unsigned char tmp, res = (unsigned char)cvGetReal2D(img, 0, 0);

        for (r = 0; r < img->rows; r++)
            for (c = 0; c < img->cols; c++) {
                tmp = (unsigned char)cvGetReal2D(img, r, c);
                if (tmp > res)
                    res = tmp;
            }

        return res;
    }

    // find maximum in 32bit float matrix
    float max32(CvMat* img) {
        int r, c;
        float tmp, res = cvmGet(img, 0, 0);

        for (r = 0; r < img->rows; r++)
            for (c = 0; c < img->cols; c++) {
                tmp = cvmGet(img, r, c);
                if (tmp > res)
                    res = tmp;
            }

        return res;
    }

    // add val to all matrix elements
    void add32(CvMat* img, float val) {
        int r, c;
        float tmp;

        for (r = 0; r < img->rows; r++)
            for (c = 0; c < img->cols; c++) {
                tmp = cvmGet(img, r, c) + val;
                cvmSet(img, r, c, tmp);
            }
    }

    // subtract val from all matrix elements
    void subtract32(CvMat* img, float val) {
        int r, c;
        float tmp;

        for (r = 0; r < img->rows; r++)
            for (c = 0; c < img->cols; c++) {
                tmp = cvmGet(img, r, c) - val;
                cvmSet(img, r, c, tmp);
            }
    }

    // divide all matrix elements with val
    void divide32(CvMat* img, float val) {
        int r, c;
        float tmp;

        for (r = 0; r < img->rows; r++)
            for (c = 0; c < img->cols; c++) {
                tmp = cvmGet(img, r, c) / val;
                cvmSet(img, r, c, tmp);
            }
    }

    // normalize 32bit float matrix to values in the interval [0..1]
    void normalize32(CvMat* img) {
        float maxval = max32(img);
        float minval = min32(img);
        float tmp;

        if (minval < 0.0) {
            tmp = fabsf(minval);
            add32(img, tmp);
        }
        else {
            tmp = minval;
            subtract32(img, tmp);
        }

        maxval = max32(img);
        divide32(img, maxval);
    }

    void normalize32contrast(CvMat* img) {
        int r, c;
        float maxval = max32(img);
        float minval = min32(img);
        float tmp;

        if (minval < 0.0) {
            tmp = fabsf(minval);
            add32(img, tmp);
        }
        else {
            tmp = minval;
            subtract32(img, tmp);
        }

        maxval = max32(img);
        divide32(img, maxval);

        for (r = 0; r < img->rows; r++)
            for (c = 0; c < img->cols; c++) {
                tmp = cvGetReal2D(img, r, c);
                if (tmp < 0)
                    cvSetReal2D(img, r, c, 0);
            }

    }

    // find maximum in 32bit float Image
    float max32Ipl(IplImage* img) {
        int r, c;
        float tmp, res = (float)cvGetReal2D(img, 0, 0);

        for (r = 0; r < img->height; r++)
            for (c = 0; c < img->width; c++) {
                tmp = cvGetReal2D(img, r, c);
                if (tmp > res)
                    res = tmp;
            }

        return res;
    }

    // find minimum in 32bit float Image
    float min32Ipl(IplImage* img) {
        int r, c;
        float tmp, res = (float)cvGetReal2D(img, 0, 0);

        for (r = 0; r < img->height; r++)
            for (c = 0; c < img->width; c++) {
                tmp = cvGetReal2D(img, r, c);
                if (tmp < res)
                    res = tmp;
            }

        return res;
    }

    // add val to all matrix elements
    void add32Ipl(IplImage* img, float val) {
        int r, c;
        float tmp;

        for (r = 0; r < img->height; r++)
            for (c = 0; c < img->width; c++) {
                tmp = cvGetReal2D(img, r, c) + val;
                cvSetReal2D(img, r, c, tmp);
            }
    }

    // subtract val from all matrix elements
    void subtract32Ipl(IplImage* img, float val) {
        int r, c;
        float tmp;

        for (r = 0; r < img->height; r++)
            for (c = 0; c < img->width; c++) {
                tmp = cvGetReal2D(img, r, c) - val;
                cvSetReal2D(img, r, c, tmp);
            }
    }


    // divide all matrix elements with val
    void divide32Ipl(IplImage* img, float val) {
        int r, c;
        float tmp;

        for (r = 0; r < img->height; r++)
            for (c = 0; c < img->width; c++) {
                tmp = cvGetReal2D(img, r, c) / val;
                cvSetReal2D(img, r, c, tmp);
            }
    }

    // normalize 32bit float IplImage to values in the interval [0..1]
    void normalize32Ipl(IplImage* img) {
        float maxval = max32Ipl(img);
        float minval = min32Ipl(img);
        float tmp;
        if (minval < 0.0) {
            tmp = fabsf(minval);
            add32Ipl(img, tmp);
        }
        else {
            tmp = minval;
            subtract32Ipl(img, tmp);
        }

        maxval = max32Ipl(img);
        divide32Ipl(img, maxval);
    }


    // Binarize  Ipl Image
    void binarize32Ipl(IplImage* img, float val) {
        int r, c;

        for (r = 0; r < img->height; r++)
            for (c = 0; c < img->width; c++) {
                if (cvGetReal2D(img, r, c) > val)
                    cvSetReal2D(img, r, c, 1);
                else
                    cvSetReal2D(img, r, c, 0);

            }
    }

    //******************  FOR 2D fprint  *************
    //f_print : 2d mat with global variables W nd H

    // find maximum in 32bit float Image
    void copy2Df_print(int** f_print1, int** f_print2) {
        int r, c;

        for (r = 0; r < H + margin; r++)
            for (c = 0; c < W + margin; c++) {
                f_print2[r][c] = f_print1[r][c];

            }

    }


    // find maximum in 32bit float Image
    int max2Df_print(int** f_print) {
        int r, c;
        int tmp, res = f_print[0][0];

        for (r = 0; r < H + margin; r++)
            for (c = 0; c < W + margin; c++) {
                tmp = f_print[r][c];
                if (tmp > res)
                    res = tmp;
            }

        return res;
    }

    // find minimum in 32bit float Image


    int min2Df_print(int** f_print) {
        int r, c;
        int tmp, res = f_print[0][0];

        for (r = 0; r < H + margin; r++)
            for (c = 0; c < W + margin; c++) {
                tmp = f_print[r][c];
                if (tmp < res)
                    res = tmp;
            }

        return res;
    }


    // add val to all matrix elements
    void add2Df_print(int** f_print, int val) {
        int r, c;
        int tmp;

        for (r = 0; r < H + margin; r++)
            for (c = 0; c < W + margin; c++) {
                tmp = f_print[r][c] + val;
                f_print[r][c] = tmp;
            }
    }


    // subtract val from all matrix elements
    void subtract2Df_print(int** f_print, int val) {
        int r, c;
        int tmp;

        for (r = 0; r < H + margin; r++)
            for (c = 0; c < W + margin; c++) {
                tmp = f_print[r][c] - val;
                f_print[r][c] = tmp;
            }
    }


    // divide all matrix elements with val
    void divide2Df_print(int** f_print, float val) {
        int r, c;
        int tmp;

        for (r = 0; r < H + margin; r++)
            for (c = 0; c < W + margin; c++) {
                tmp = f_print[r][c] / val;
                f_print[r][c] = tmp;
            }
    }

    // normalize 32bit float IplImage to values in the interval [0..1]
    void normalize2Df_print(int** f_print) {
        int maxval = max2Df_print(f_print);
        int minval = min2Df_print(f_print);
        int tmp;
        if (minval < 0.0) {
            tmp = fabsf(minval);
            add2Df_print(f_print, tmp);
        }
        else {
            tmp = minval;
            subtract2Df_print(f_print, tmp);
        }

        maxval = max2Df_print(f_print);
        divide2Df_print(f_print, (maxval / 100.0));
    }


    // Binarize  Ipl Image
    void binarize2Df_print(int** f_print, int val) {
        int r, c;

        for (r = 0; r < H + margin; r++)
            for (c = 0; c < W + margin; c++) {
                if (f_print[r][c] > val)
                    f_print[r][c] = 100;
                else
                    f_print[r][c] = 0;

            }
    }

    // Binarize  Ipl Image
    void invert32Ipl(IplImage* img) {
        int r, c;
        float tmp;

        for (r = 0; r < img->height; r++) {
            for (c = 0; c < img->width; c++) {
                tmp = cvGetReal2D(img, r, c);
                cvSetReal2D(img, r, c, 1 - tmp);

            }
        }
    }

    // Invert f_print 2D mat float
    void invert2Df_print(int** f_print) {
        int r, c;

        for (r = 0; r < H + margin; r++)
            for (c = 0; c < W + margin; c++)
                f_print[r][c] = 1 - f_print[r][c];

    }


    // Binarize  Ipl Image
    void Ipltomat2D(IplImage* img, int** dst) {
        int r, c;

        for (r = 0; r < img->height; r++)
            for (c = 0; c < img->width; c++) {
                dst[r][c] = (int)cvGetReal2D(img, r, c);
                //cout<<dst[r][c];getchar();
            }
    }



    // Binarize  Ipl Image
    void Ipltomat2D_float(IplImage* img, float** dst) {
        int r, c;

        for (r = 0; r < img->height; r++)
            for (c = 0; c < img->width; c++) {
                dst[r][c] = (float)cvGetReal2D(img, r, c);
                //cout<<dst[r][c];getchar();
            }
    }




    // Binarize  Ipl Image
    void Ipltomat2D_bin(IplImage* img, float** dst) {
        int r, c;

        for (r = 0; r < img->height; r++)
            for (c = 0; c < img->width; c++) {
                dst[r][c] = cvGetReal2D(img, r, c) / 255;
                //cout<<dst[r][c];getchar();
            }
    }


    // Binarize  Ipl Image
    void mat2DtoIpl(float** dst, IplImage* img) {
        int r, c;

        for (r = 0; r < img->height; r++)
            for (c = 0; c < img->width; c++)
                cvSetReal2D(img, r, c, dst[r][c]);
    }

    // Binarize  Ipl Image
    void mat2DtoIpl(int** dst, IplImage* img) {
        int r, c;

        for (r = 0; r < img->height; r++)
            for (c = 0; c < img->width; c++)
                cvSetReal2D(img, r, c, dst[r][c]);
    }

    // Binarize  Ipl Image
    void cvmattomat2D(CvMat* img, int** dst) {
        int r, c;

        for (r = 0; r < img->height; r++)
            for (c = 0; c < img->width; c++)
                dst[r][c] = (int)cvmGet(img, r, c);
    }


    // Binarize  Ipl Image
    void mat2Dtocvmat(float** dst, CvMat* img) {
        int r, c;

        for (r = 0; r < img->height; r++)
            for (c = 0; c < img->width; c++)
                cvmSet(img, r, c, dst[r][c]);
    }

    // multiply all matrix elements with val
    void multiply32(CvMat* img, float val) {
        int r, c;
        float tmp;

        for (r = 0; r < img->rows; r++)
            for (c = 0; c < img->cols; c++) {
                tmp = cvmGet(img, r, c) * val;
                cvmSet(img, r, c, tmp);
            }
    }

    // convert float matrix to iplimage (NO normalization!)
    void cv32fc1_to_ipl8uc1(CvMat* src, IplImage* dst) {
        CvMat* tmp8 = cvCreateMat(src->rows, src->cols, CV_8UC1);
        cv32fc1_to_cv8uc1(src, tmp8);
        cvCopy(tmp8, dst);
        cvReleaseMat(&tmp8);
    }

    // normalize and save 32f matrix as image
    void save_normalize_32fc1(CvMat* m, char* filename) {
        CvMat* tmp32 = cvCloneMat(m);
        normalize32(tmp32);
        multiply32(tmp32, 255);

        IplImage* img = cvCreateImage(cvSize(m->cols, m->rows), IPL_DEPTH_8U, 1);
        cv32fc1_to_ipl8uc1(tmp32, img);
        cvSaveImage(filename, img);

        cvReleaseImage(&img);
        cvReleaseMat(&tmp32);
    }

    // save 32f as image (no normalization)
    void save_32fc1(CvMat* m, char* filename) {
        IplImage* img = cvCreateImage(cvSize(m->cols, m->rows), IPL_DEPTH_8U, 1);
        cv32fc1_to_ipl8uc1(m, img);
        cvSaveImage(filename, img);
        cvReleaseImage(&img);
    }

    // save 8uc1 as image
    void save_8uc1(CvMat* m, char* filename) {
        IplImage* img = cvCreateImage(cvSize(m->cols, m->rows), IPL_DEPTH_8U, 1);
        cvCopy(m, img);
        cvSaveImage(filename, img);
        cvReleaseImage(&img);
    }

    // Multiply a scalar on all matrix elements
    void muls(CvMat* src, double s, CvMat* dst) {
        int r, c;
        float temp;

        for (r = 0; r < src->rows; r++)
            for (c = 0; c < src->cols; c++) {
                temp = cvmGet(src, r, c) * s;
                cvmSet(dst, r, c, (float)temp);
            }
    }

    // Divide a scalar on all matrix elements
    void divs(CvMat* src, double s, CvMat* dst) {
        int r, c;
        float temp;

        for (r = 0; r < src->rows; r++)
            for (c = 0; c < src->cols; c++) {
                temp = cvmGet(src, r, c) / s;
                cvmSet(dst, r, c, (double)temp);
            }
    }

    void picturize(float** f_print) {
        int r, c;
        float tmp;
        for (r = 0; r < H + margin; r++)
            for (c = 0; c < W + margin; c++) {
                tmp = f_print[r][c] * 255;
                f_print[r][c] = tmp;
            }
    }

    float max_float(float x, float y) {
        if (x > y)
            return (x);
        else
            return (y);
    }
}