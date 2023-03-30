#include <stdlib.h>
#include "helperFun.h"
#include <math.h>

void linearFilter(Img* img, void* pixelTransformation(Img *img, unsigned char *fun),
                  void* transformation(unsigned char *arr, double a, double b), double a, double b){
    unsigned char arr[256] = {0};
    transformation(arr, a, b);
    pixelTransformation(img, arr);
}

void powerLawTransform(unsigned char *arr, double c, double velar){
    double max = 255;
    for (int i = 0; i < 256; ++i)
        arr[i] =(unsigned char) ((c * pow((i / max), velar)) * max);
}

void histogramStretching(unsigned char *arr, double a, double b){
    double max = 255;
    for (int i = 0; i < 256; ++i)
        arr[i] = (unsigned char) ((a*i/max + b) * max);
}

void linearConvolution(Img *img, int size){
    double sum;
    size /= 2;
    Img *data = copyImg(img);

    for(int i = 0; i < img -> height; i++){
        for(int j = 0; j < img -> width; j++){
            sum = 0;

            for (int k = -size; k < size; ++k) {
                for (int l = -size; l < size; ++l) {
                    sum += getVal(data, j, i, 0);
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

    for (int k = -size; k < size; ++k) {
        for (int l = -size; l < size; ++l) {

            arr[k + size + 1][l + size + 1] = c * exp((-1 * (pow(k, 2) + pow(l, 2))) / pow(sigma, 2) / 2);
            fraction += arr[k + size + 1][l + size + 1];
        }
    }
    for(int i = 0; i < img -> height; i++){
        for(int j = 0; j < img -> width; j++){

            sum = 0;

            for (int k = -size; k < size; ++k) {
                for (int l = -size; l < size; ++l) {

                    temp = (double) getVal(data, j, i, 0);

                    sum += temp * arr[k + size + 1][l + size + 1];
                }
            }
            sum /= fraction;
            putVal(img, j, i, (unsigned char)sum);
        }
    }
    destroyImg(data);
}

