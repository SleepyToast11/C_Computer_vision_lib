#include "helperFun.h"
#include "stdlib.h"
#include <math.h>

void histogramManipulation(Img* img, void* pixelTransformation(Img *img, unsigned char *fun),
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

int medianFilter(Img* img, int i, int j, int size){
    unsigned char arr[size][size];
    int temp = size;
    size /= 2;
            for (int k = -size; k <= size; ++k) {
                for (int l = -size; l <= size; ++l) {
                    arr[k+size][l+size] = getVal(img, j + l, i + k, 0);
                }
            }

            qsort(arr, 9, sizeof (char), cmpfunc); //Yes, this is comes standard in stdlib.c :)
    return arr[size][size];
}

struct filter{
    double* filter;
    int size;
    int halfSize;
    double divider;
};

struct filter filter(struct filter *ptr, double* filter, int size){

    double divider = 0;
    malloc(size * size * sizeof (double));

    for (int i = 0; i < size * size; ++i) {

        ptr -> filter[i] = filter[i];

        divider += filter[i];

    }
    ptr -> size = size;
    ptr -> halfSize = size/2;
    ptr -> divider = divider;
}

void destroyfilter(struct filter *ptr){
    free(ptr->filter);
}

int applyToPixel(Img *img, int i, int j, struct filter filter){ //one could simplify by calculating divider for each, but across 1000th of iteration, its problematic
    double sum;

    sum = 0;

    //improved accuracy could be achieved here by adding logic and making the default return -1 and removing the value at that index and the divider
    for (int k = -filter.halfSize; k <= filter.halfSize; k++) {

        for (int l = -filter.halfSize; l <= filter.halfSize; l++) {

            sum += filter.filter[(k+filter.halfSize) * filter.size + (l + filter.halfSize)] *
                    getVal(img, j + l, i + k, 0);
        }
    }
    sum /= filter.divider;
    return (int) sum;
}

void averagingFilter(struct filter* filter1, int size){

    double arr[size * size];
    for (int i = 0; i < size*size; ++i) {
        arr[i] = 1;
    }

    filter(filter1, arr, size);
}

void smoothingFilter(Img *img, int size, void (function)(), double *var){
    
    struct filter fil;

    if(!var)
        function(&fil, size);
    else
        function(&fil, size, var);

    Img *data = copyImg(img);

    for(int i = 0; i < img -> height; i++){
        for(int j = 0; j < img -> width; j++) {

            putVal(img, j, i, applyToPixel(data, i, j, fil));

        }
    }

    destroyImg(data);
}

inline void histogramStretching(unsigned char *arr, double a, double b){
    for (int i = 0; i < 256; ++i)
        arr[i] = (unsigned char) ((a*i + b));
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

