#include "helperFun.h"
#include "stdlib.h"
#include <math.h>

void linearFilter(Img* img, void* pixelTransformation(Img *img, unsigned char *fun),
                  void* transformation(unsigned char *arr, double a, double b), double a, double b){
    unsigned char arr[256] = {0};
    transformation(arr, a, b);
    pixelTransformation(img, arr);
}

inline void powerLawTransform(unsigned char *arr, double c, double velar){
    double max = 255;
    for (int i = 0; i < 256; ++i)
        arr[i] =(unsigned char) ((c * pow((i / max), velar)) * max);
}

int cmpfunc (const void * a, const void * b) {
    return ( *(unsigned char*)a - *(unsigned char*)b );
}

void medianFilter(Img *img){
    double sum;
    Img *data = copyImg(img);
    char arr[9];


    for(int i = 0; i < img -> height; i++) {
        for (int j = 0; j < img->width; j++) {
            for (int k = -1; k <= 1; ++k) {
                for (int l = -1; l <= 1; ++l) {
                    arr[(l + 1) + 3 * (k + 1)] = getVal(data, j + l, i + k, 0);
                }
            }

            qsort(arr, 9, sizeof (char), cmpfunc);//Yes, this is comes standard in stdlib.c :)
            putVal(img, j, i, *(arr + 5));
        }
    }
    destroyImg(data);
}

inline void histogramStretching(unsigned char *arr, double a, double b){
    for (int i = 0; i < 256; ++i)
        arr[i] = (unsigned char) ((a*i + b));
}

void linearConvolution(Img *img, int size){
    double sum;
    size /= 2;
    Img *data = copyImg(img);

    for(int i = 0; i < img -> height; i++){
        for(int j = 0; j < img -> width; j++){
            sum = 0;

            for (int k = -size; k <= size; k++) {
                for (int l = -size; l <= size; l++) {
                    sum += getVal(data, j + l, i + k, 0);
                }
            }

            sum /= 9;
            putVal(img, j, i, (unsigned char)sum);

        }
    }
    destroyImg(data);
}

void gaussianConvolution(Img *img, int size, double c, double sigma){
    double sum, temp, fraction;
    double arr[size][size];
    size /= 2;

    Img *data = copyImg(img);

    for (int k = -size; k <= size; k++) {
        for (int l = -size; l <= size; l++) {

            arr[k + size][l + size] = c * exp(((pow(k, 2) + pow(l, 2))) / pow(sigma, 2) * -0.5);
            fraction += arr[k + size][l + size];
        }
    }
    for(int i = 0; i < img -> height; i++){
        for(int j = 0; j < img -> width; j++){

            sum = 0;

            for (int k = -size; k <= size; k++) {
                for (int l = -size; l <= size; l++) {

                    temp = (double) getVal(data, j + l, i + k, 0) / 255;

                    sum += temp * arr[k + size][l + size];
                }
            }
            sum /= fraction;
            putVal(img, j, i, (unsigned char)(sum * 255));
        }
    }
    destroyImg(data);
}

