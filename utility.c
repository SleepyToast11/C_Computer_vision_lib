#include <stdlib.h>
#include "helperFun.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

Img* getImg(){
    printf("enter full name of file: ");
    char arg[20];
    memset(arg, 0, 20);
    if(0 == scanf("%s", arg))
        exit(-1);
    FILE *fp;
    fp = fopen(arg, "r");
    Img *data = pgmToImg(fp);
    fclose(fp);
    return data;
}

rgbImg *getRgbImg(){
    printf("enter full name of file: ");
    char arg[20];
    memset(arg, 0, 20);
    if(0 == scanf("%s", arg))
        exit(-1);
    FILE *fp;
    fp = fopen(arg, "r");
    rgbImg *data = ppmToImg(fp);
    fclose(fp);
    return data;
}

inline void addPoints( double *src, double *dst, int size){
    for (int i = 0; i < size+2; ++i) {
        dst[i] += src[i];
    }
}

inline void multiplyPoints( double src, double *dst, int size){
    for (int i = 0; i < size; ++i) {
        dst[i] *= src;
    }
}

inline void dividePointCons( double src, double *dst, int size){
    if(src == 0)
            return; //is the same as letting the point be
    for (int i = 0; i < size + 2; ++i) {
        dst[i] /= src;
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

inline void copyPoint( double *src, double *dst, int size){
    for (int i = 0; i < size+2; ++i) {
        dst[i] = src[i];
    }
}

double drand ( double low, double high )
    {
    double range = (high - low);
    double div = RAND_MAX / range;
    if(low + (rand() / div) == -NAN) {

        printf("crap");
        exit(2);
    }
    return low + (((double) rand()) / div);
}

inline void nullDPoint(double *dst, int size){
    for (int i = 0; i < size; ++i) {
        dst[i] = 0;
    }
}

inline void nullList(struct ListPoint *list){
    for (int i = 0; i < list->size; ++i) {
        nullDPoint(list->list[i], list->pointSize + 2);
    }
}

