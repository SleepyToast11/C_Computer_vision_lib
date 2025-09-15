#include "helperFun.h"
#include "stdlib.h"
#include <math.h>

void histogramManipulation(Img* img, void* pixelTransformation(Img *img, unsigned char *fun),
                  void* transformation(unsigned char *arr, double a, double b), double a, double b){
    unsigned char arr[256];
    transformation(arr, a, b);
    pixelTransformation(img, arr);
}

inline void powerLawTransform(unsigned char *arr, double c, double velar){
    double max = 255;
    for (int i = 0; i < 256; ++i)
        arr[i] = (unsigned char) ((c * pow((i / max), velar)) * max);
}

inline void linearTransform(unsigned char *arr, double a, double b){
    for (int i = 0; i < 256; ++i)
        arr[i] =(unsigned char) (a * i + b);
}

int cmpfunc (const void * a, const void * b) {
    return ( *(unsigned char*)a - *(unsigned char*)b );
}

int medianFilter(Img* img, int i, int j, struct filter fil, int size){
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


struct filter filter(double* arr, int size){

    struct filter *ret;
    ret = malloc(sizeof(struct filter));

    double divider = 0;
    ret -> filter = malloc(size * size * sizeof (double));

    for (int i = 0; i < size * size; ++i) {

        ret -> filter[i] = arr[i];

        divider += arr[i];

    }

    ret -> size = size;
    ret -> halfSize = size/2;
    ret -> divider = divider;

    return *ret;
}


void destroyfilter(struct filter *ptr){
    free(ptr->filter);
}


int applyLinearFilterToPixel(Img *img, int i, int j, struct filter filter, int a){ //one could simplify by calculating divider for each, but across 1000th of iteration, its problematic
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


void averagingFilter(struct filter* filter1, int size, double a, double b){

    double arr[size * size];
    for (int i = 0; i < size*size; ++i) {
        arr[i] = 1;
    }

    *filter1 = filter(arr, size);
}

void smoothingFilter(Img *img, int size, int function(Img* img, int i, int j, struct filter fil, int size), void (fil)(struct filter* filter1, int size, double a, double b),
                     double a, double b){
    if(fil){
        struct filter myFilter;
        fil(&myFilter, size, a, b);

        Img *data = copyImg(img);

        for(int i = 0; i < img -> height; i++){
            for(int j = 0; j < img -> width; j++) {

                putVal(img, j, i, function(data, i, j, myFilter, 0));

            }
        }

        destroyImg(data);
    }
    else{
        struct filter myFilter;
        Img *data = copyImg(img);

        for(int i = 0; i < img -> height; i++){
            for(int j = 0; j < img -> width; j++) {

                putVal(img, j, i, (unsigned char)function(data, i, j, myFilter, size));

            }
        }
        destroyImg(data);
    }

}

inline void histogramStretching(unsigned char *arr, double a, double b){
    for (int i = 0; i < 256; ++i)
        arr[i] = (unsigned char) ((a*i + b));
}


void gaussianConvolution(struct filter* fil, int size, double c, double sigma){

    double *arr = malloc(size * size * sizeof(double) );
    int halfSize = size / 2;

    for (int k = -halfSize; k <= halfSize; k++) {
        for (int l = -halfSize; l <=halfSize; l++) {

            arr[(halfSize + k) * size + (l + halfSize)] =
                    c * exp(((pow(k, 2) + pow(l, 2))) / pow(sigma, 2) * -0.5);

        }
    }
    *fil = filter(arr, size);
}

