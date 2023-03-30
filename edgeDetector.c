#include "helperFun.h"
#include <math.h>

void horizontalEdge(Img* img, int derivation){

    unsigned char x0, x1;

    for (int i = 0; i < derivation; ++i) {
        for(int i = 0; i < img -> height; i++){
            for(int j = 0; j < img -> width; j++){

                x0 = getVal(img, j, i, 0);
                x1 = getVal(img, j + 1, i, 0);

                putVal(img, j, i, (unsigned char)x1 - x0);
            }
        }
    }
}

void verticalEdge(Img* img, int derivation){

    int x0, x1;

    for (int i = 0; i < derivation; ++i) {
        for(int i = 0; i < img -> height; i++){
            for(int j = 0; j < img -> width; j++){

                x0 = (int) getVal(img, j, i, 0);
                x1 = (int) getVal(img, j, i + 1, 0);

                putVal(img, j, i, (unsigned char)x1 - x0);
            }
        }
    }
}

void gradientEdge(Img *img, double sigma){

    double sum, temp, fraction;
    double arrx[3][3];
    double arry[3][3];

    int size = 1;

    Img *data = copyImg(img);

    for (int k = -size; k < size; ++k) {
        for (int l = -size; l < size; ++l) {
            arrx[k + size + 1][l + size + 1] = k / 2 / M_PI / pow(sigma, 4) * exp((-1 * (pow(k, 2) + pow(l, 2))) / pow(sigma, 2) / 2);

            arrx[k + size + 1][l + size + 1] = k * exp((-1 * (pow(k, 2) + pow(l, 2))) / pow(sigma, 2) / 2);
            fraction += arrx[k + size + 1][l + size + 1];
        }
    }
    for(int i = 0; i < img -> height; i++){
        for(int j = 0; j < img -> width; j++){
            sum = 0;
            for (int k = -size; k < size; ++k) {
                for (int l = -size; l < size; ++l) {
                    temp = (double) getVal(data, j, i, 0);
                    sum += temp * arrx[k + size + 1][l + size + 1];
                }
            }
            sum /= fraction;
            putVal(img, j, i, (unsigned char)sum);
        }
    }
    destroyImg(data);
}