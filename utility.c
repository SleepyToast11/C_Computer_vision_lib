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

inline void addPoints(const double *src, double *dst, int size){
    for (int i = 0; i < size; ++i) {
        dst[i] += src[i];
    }
}

inline void multiplyPoints(const double src, double *dst, int size){
    for (int i = 0; i < size; ++i) {
        dst[i] *= src;
    }
}

inline void dividePointCons(const double src, double *dst, int size){
    for (int i = 0; i < size; ++i) {
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

double drand ( double low, double high ) //took for a random forum https://cboard.cprogramming.com/c-programming/17939-random-double-numbers.html :)
{
    return ( (double)rand() * ( high - low ) ) / (double)RAND_MAX + low;
}

inline void nullDPoint(double *dst, int size){
    for (int i = 0; i < size; ++i) {
        dst[i] = 0;
    }
}

inline void nullList(struct ListPoint *list){
    for (int i = 0; i < list->size; ++i) {
        nullDPoint(list->list[i], list->pointSize);
    }
}

