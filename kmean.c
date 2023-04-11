#include <stdlib.h>
#include "helperFun.h"
#include <math.h>

struct ListPoint{
    double **list;
    unsigned char pointSize;
    int size;
};

void destroyListPoint(struct ListPoint listPoint){
    for (int i = 0; i < listPoint.size; ++i) {
        free(listPoint.list[i]);
        free(listPoint.list);
    }
}

void Point(double *dst, double* vals, int size, int x, int y){
    dst = malloc(sizeof (double) * (size + 2));
    dst[0] = (double) x;
    dst[1] = (double) y;
    for (int i = 2; i < size + 2; ++i) {
        dst[i] = vals[i];
    }
}

void ListPoint(struct ListPoint *dst, int pointSize, int size){
    dst->list = malloc(sizeof(double*) * size);
    dst -> pointSize = pointSize;
    dst -> size = size;
}

void IMGToListPoint(Img *img, struct ListPoint *dst){

    int listSize = img->width * img->height;
    ListPoint(dst, 1, listSize);
    double val[1];
    for(int i = 0; i < img -> height; i++){
        for(int j = 0; j < img -> width; j++){
            val[0] = (double) getVal(img, j, i, 0);
            Point(&dst->list[i * (img -> width) + j],
                  val,
                  1, j, i);
        }
    }
}

void RgbIMGToListPoint(rgbImg *img, struct ListPoint *dst){

    int listSize = img->rImg -> width * img->rImg -> height;
    ListPoint(dst, 1, listSize);
    double val[3];
    for(int i = 0; i < img -> rImg -> height; i++){
        for(int j = 0; j < img -> rImg -> width; j++){

            val[0] = (double) getVal(img -> rImg, j, i, 0);
            val[1] = (double) getVal(img -> gImg, j, i, 0);
            val[2] = (double) getVal(img -> bImg, j, i, 0);
            Point(&dst->list[i * (img -> rImg -> width) + j],
                  val,
                  3, j, i);
        }
    }
}

inline double distancePoints(double *a, double *b, int size, int withPos){
    double sum = 0;

    if(withPos)
        withPos = 0;
    else
        withPos = 2;

    for(; withPos < size + 2; withPos++){
        sum += pow(a[withPos] - b[withPos], 2);
    }
    return sqrt(sum);
}

inline double addPoints(double *src, double *dst, int size){
    for (int i = 0; i < size; ++i) {
        dst[i] += src[i];
    }
}

inline int minimumDistance(double *point, struct ListPoint list, int withPos){
    double min = INFINITY;
    double temp;
    int p = 0;
    for (int i = 0; i < list . size; ++i) {
        temp = distancePoints(point , list.list[i], list.pointSize, withPos);
        if(temp < min){
            min = temp;
            p = i;
        }
    }
    return p;
}

double drand ( double low, double high ) //took for a random forum https://cboard.cprogramming.com/c-programming/17939-random-double-numbers.html :)
{
    return ( (double)rand() * ( high - low ) ) / (double)RAND_MAX + low;
}

inline void nullDPoint(double *dst, int size){
    for (int i = 0; i < size; ++i) {
        dst[i] = 0;
    }
}

inline void nullIPoint(int *dst, int size){
    for (int i = 0; i < size; ++i) {
        dst[i] = 0;
    }
}

struct ListPoint * ImgKmean(Img *img, int k, int withPos, int numberGen){
    struct ListPoint list;
    IMGToListPoint(img, &list);

    struct ListPoint *cluster = malloc(sizeof (struct ListPoint));
    ListPoint(cluster, 1, k);
    double random[1];

    for (int i = 0; i < k; ++i) {
        random[0] = drand(0, 255);
        Point(&cluster->list[i], random, 1,
              drand(0, img->width),
              drand(0, img->height));
    }

    int clusterNum[k], temp;
    double tempD[3];

    for (int i = 0; i < numberGen; ++i) {

        nullDPoint(tempD, 3);
        nullIPoint(clusterNum, k);

        for (int j = 0; j < list.size; ++j) {

            temp = minimumDistance(list.list[i], *cluster, withPos);
            addPoints( , tempD);

        }
    }

    destroyListPoint(list);
    return cluster;
}