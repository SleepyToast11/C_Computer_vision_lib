#include "helperFun.h"
#include <math.h>

#define DIRECTION_BOTH 3
#define DIRECTION_VERTICAL 2
#define DIRECTION_HORIZONTAL 1
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

void edgeDetector(Img* img, int direction, int reversed, int centered, int sobel){

    Img *data = copyImg(img);

    int so = 0;

    if (sobel) {
        so = 1;
        centered = 1;
    }
    else {
        sobel = 1;
    }

    if(!reversed)
        reversed = 1;
    else
        reversed = -1;

    int sobelArr[] = {reversed, reversed * sobel, reversed};

    int centeredArr[3] = {-1, -1, 1};

    if(centered) {
        centered = 1;
        centeredArr[1] = 0;
    }

    int sumx, sumy, l;

        for(int i = 0; i < img -> height; i++){
            l = l;
            for(int j = 0; j < img -> width; j++){

                sumx = 0;
                sumy = 0;

                for(int k = -centered; k <= 1; ++k) {

                    if(!centeredArr[centered + 1])
                        continue;

                    l = -so; //will only be 0 or 1

                    do{
                        if (direction & 1)
                            sumx += centeredArr[k + 1] * sobelArr[l + 1] *
                                    getVal(data, j + k, i + l, 0);

                        if (direction & 2)
                            sumy += centeredArr[k + 1] * sobelArr[l + 1] *
                                    getVal(data, j + l, i + k, 0);

                        l++;
                    } while (l <= 1 && so != 0);
                    }
                putVal(img, j, i, MAX(sumy, sumx));
                }
            }
    destroyImg(data);
}



void gaussianGradientEdge(Img *img, double sigma, int reversed, int direction){

    if(!reversed)
        reversed = -1;
    else
        reversed = 1;

    double sumx, sumy, temp, fraction;
    double arrx[3][3];
    double arry[3][3];
    int tempInt;

    Img *data = copyImg(img);

    for (int k = -1; k <= 1; ++k) {
        for (int l = -1; l <= 1; ++l) {

            arrx[k + 1][l + 1] = (double) reversed * (double) l / -2 / M_PI / pow(sigma, 4) *
                    exp((-1 * (pow(k, 2) + pow(l, 2))) / pow(sigma, 2) / 2);

            arry[k + 1][l + 1] = (double) reversed * (double) k / -2 / M_PI / pow(sigma, 4) *
                    exp((-1 * (pow(k, 2) + pow(l, 2))) / pow(sigma, 2) / 2);

        }
    }

    for(int i = 0; i < img -> height; i++){
        for(int j = 0; j < img -> width; j++){

            sumx = 0;
            sumy = 0;

            for (int k = -1; k <= 1; ++k) {
                for (int l = -1; l <= 1; ++l) {

                    temp = (double) getVal(data, j + l, i+ k, 0);

                    if(direction & 1)
                        sumx += temp * arrx[k + 1][l + 1];

                    if(direction & 2)
                        sumy += temp * arry[k + 1][l + 1];
                }
            }

            sumx = pow(sumx, 2);
            sumy = pow(sumy, 2);

            putVal(img, j, i, (sqrt(sumx + sumy)));
        }
    }

    destroyImg(data);
}