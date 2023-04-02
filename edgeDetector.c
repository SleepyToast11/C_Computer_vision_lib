#include "helperFun.h"
#include <math.h>

#define DIRECTION_BOTH 0
#define DIRECTION_VERTICAL 2
#define DIRECTION_HORIZONTAL 1

//0 both, 1 horizontal, 2 vertical
void edgeDetector(Img* img, int derivation, int direction, int reversed, int centered){

    unsigned char x0, x1, x2, x3;
    int rev;

    if(!reversed)
        rev = 1;
    else
        rev = -1;

    if(centered)
        centered = 1;

    for (int k = 0; k < derivation; ++k) {
        for(int i = 0; i < img -> height; i++){
            for(int j = 0; j < img -> width; j++){

                if(!(direction)) {
                        x0 = getVal(img, j - centered, i, 0);
                        x1 = getVal(img, j + 1, i, 0);
                        x2 = getVal(img, j, i - centered, 0);
                        x3 = getVal(img, j, i+1, 0);

                        if(rev * (x0 - x1) < rev * (x3 - x2))
                            putVal(img, j, i, rev * (x3 - x2));
                        else
                            putVal(img, j, i, rev * (x0 - x1));
                }
                else{
                    if (direction - 1){
                        x0 = getVal(img, j, i - centered, 0);
                        x1 = getVal(img, j, i + 1, 0);
                        putVal(img, j, i, rev * (x0 - x1));
                    }

                    else{
                        x0 =  getVal(img, j - centered, i, 0);
                        x1 =  getVal(img, j + 1, i, 0);
                        putVal(img, j, i, rev * (x0 - x1));
                    }
                }

            }
        }
    }
}



void gaussianGradientEdge(Img *img, double sigma, double c){

    double sumx, sumy, temp, fraction;
    double arrx[3][3];
    double arry[3][3];

    int size = 1;

    Img *data = copyImg(img);

    for (int k = -size; k <= size; ++k) {
        for (int l = -size; l <= size; ++l) {

            arrx[k + size][l + size] = c * (double) l / -2 / M_PI / pow(sigma, 4) *
                    exp((-1 * (pow(k, 2) + pow(l, 2))) / pow(sigma, 2) / 2);

            arry[k + size][l + size] = c * (double) k / -2 / M_PI / pow(sigma, 4) *
                    exp((-1 * (pow(k, 2) + pow(l, 2))) / pow(sigma, 2) / 2);

        }
    }

    for(int i = 0; i < img -> height; i++){
        for(int j = 0; j < img -> width; j++){

            sumx = 0;
            sumy = 0;

            for (int k = -size; k <= size; ++k) {
                for (int l = -size; l <= size; ++l) {

                    temp = (double) getVal(data, j, i, 0) / 255;

                    sumx += temp * arrx[k + size + 1][l + size + 1];
                    sumy += temp * arry[k + size + 1][l + size + 1];
                }
            }



            sumx = pow(sumx, 2);
            sumy = pow(sumy, 2);

            putVal(img, j, i, (sqrt(sumx + sumy) * 255));
        }
    }

    destroyImg(data);

}