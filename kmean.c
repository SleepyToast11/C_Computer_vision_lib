#include <stdlib.h>
#include "helperFun.h"
#include <math.h>
#include "stdio.h"

void destroyListPoint(struct ListPoint listPoint){
    for (int i = 0; i < listPoint.size; ++i) {
        free(listPoint.list[i]);
    }
    free(listPoint.list);
}

void Point(double *dst, const double* vals, int size, int x, int y){
    dst[0] = (double) x;
    dst[1] = (double) y;
    for (int i = 2; i < size + 2; ++i) {
        dst[i] = vals[i-2];
    }
}

void initZeroList(struct ListPoint *dst, int pointSize, int size){
    dst->list = malloc(sizeof(double*) * (size));
    dst -> pointSize = pointSize;
    dst -> size = size;
    for (int i = 0; i < size; ++i) {
        dst->list[i] = malloc(sizeof (double) * (pointSize+2));
        for (int j = 0; j < pointSize+2; ++j) {
            dst->list[i][j] = 0.0;
        }
    }
}
/*
void calla(struct ListPoint *dst, int pointSize, int size){
    dst
}
*/
void ListPoint(struct ListPoint *dst, int pointSize, int size){
    dst->list = (double**) malloc(sizeof(double*) * (size));
    for (int i = 0; i < size; ++i) {
        dst->list[i] = malloc(sizeof (double ) * (pointSize + 2));
    }
    dst -> pointSize = pointSize;
    dst -> size = size;
}

void IMGToListPoint(Img *img, struct ListPoint *dst){

    int listSize = img->width * img->height;
    ListPoint(dst, 1, listSize);
    double val[1];
    for(int i = 0; i < img -> height; i++){
        for(int j = 0; j < img -> width; j++){
            Point(dst->list[i * (img -> width) + j],
                  val,
                  1, j, i);
        }
    }
}

inline int minimumDistance(double *point, struct ListPoint list, int withPos){
    double min = INFINITY;
    double temp;
    int p = 0;
    for (int i = 0; i < list . size; ++i) {
        temp = distancePoints(point , list.list[i], list.pointSize, withPos);
        if(temp < min) {
            min = temp;
            p = i;
        }
    }
    return p;
}

void rgbIMGToListPoint(rgbImg *img, struct ListPoint *dst){

    double *val = malloc(3 * sizeof (double));



    for(int i = 0; i < (img -> rImg -> height); ++i){
        for(int j = 0; j < (img -> rImg -> width); ++j){

            val[0] = (double) *((img ->rImg)->data + (i*(img -> rImg -> width)+j));
            val[1] = (double) *((img ->gImg)->data + (i*(img -> rImg -> width)+j));
            val[2] = (double) *((img ->bImg)->data + (i*(img -> rImg -> width)+j));

            Point(dst->list[i * (img -> rImg -> width) + j],
                  val,
                  3, j, i);
        }
    }
    free(val);
}

void genRandClusterImg(Img *img, struct ListPoint *dst, int k, int pointSize){

    initZeroList(dst, pointSize, k);
    double random[pointSize];

    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < pointSize; ++j) {
            random[j] = drand(1, 255);
        }
        Point(dst->list[i], random, pointSize,
              (int)drand(1, img->width),
              (int)drand(1, img->height));
    }
}


void kmean(struct ListPoint *Imglist, struct ListPoint *cluster, int withPos, int numberGen, double genDelta) {

    struct ListPoint tempList;
    initZeroList(&tempList, cluster->pointSize, cluster->size);


    int temp;
    double tempD[3], zeroPoint[3];
    nullDPoint(zeroPoint, 3);

    for (int i = 0; i < numberGen; ++i) {

        nullDPoint(tempD, 3);
        nullList(&tempList);

        for (int j = 0; j < Imglist->size; ++j) {

            temp = minimumDistance(Imglist->list[j], *cluster, withPos);
            addPoints(Imglist->list[temp], tempList.list[temp], Imglist->pointSize);

        }

        for (int j = 0; j < cluster->size; ++j) {
            //normalize
            dividePointCons(distancePoints(tempList.list[j], zeroPoint, tempList.pointSize, withPos),
                            tempList.list[j],
                            cluster->pointSize);
            //multiply
            multiplyPoints(genDelta, tempList.list[j], tempList.pointSize);
            //move cluster centroid
            addPoints(cluster->list[j], tempList.list[j], tempList.pointSize);

        }
        printf("%d\n", i);
    }

    destroyListPoint(tempList);
}

void listPointToRgbImg(struct ListPoint *src, rgbImg *dst, struct ListPoint *cluster, int withPos){

    struct ListPoint tempList;
    initZeroList(&tempList, cluster->pointSize, cluster->size);

    int temp;

    int pointNum[cluster->size];
    for (int i = 0; i < cluster->size; ++i) {
        pointNum[i] = 0;
    }

    for (int j = 0; j < src->size; ++j) {
        temp = minimumDistance(src->list[j], *cluster, withPos);
        addPoints(src->list[j], tempList.list[temp], 3);
        pointNum[temp]++;
    }

    for (int i = 0; i < tempList.size; ++i) {
        dividePointCons(pointNum[i], tempList.list[i], 3);
    }

    struct ListPoint outList;
    ListPoint(&outList, src->pointSize, src->size);

    for (int j = 0; j < src->size; ++j) {
        temp = minimumDistance(src->list[j], *cluster, withPos);
        copyPoint(tempList.list[temp], outList.list[j], 3);
    }

    for(int i = 0; i < dst -> rImg -> height; ++i){
        for(int j = 0; j < dst -> rImg -> width; ++j){
            putVal(dst -> rImg, j, i,(unsigned char) outList.list[i*dst -> rImg -> width + j][2]);
            putVal(dst -> gImg, j, i,(unsigned char) outList.list[i*dst -> rImg -> width + j][3]);
            putVal(dst -> bImg, j, i,(unsigned char) outList.list[i*dst -> rImg -> width + j][4]);
        }

    }

    destroyListPoint(outList);
    destroyListPoint(tempList);
}

void listPointToImg(struct ListPoint *src, Img *dst, struct ListPoint *cluster, int withPos){

    struct ListPoint tempList;
    ListPoint(&tempList, cluster->pointSize, cluster->size);
    initZeroList(&tempList, cluster->pointSize, cluster->size);

    int temp;

    int pointNum[cluster->size];
    for (int i = 0; i < cluster->size; ++i) {
        pointNum[i] = 0;
    }

    for (int j = 0; j < src->size; ++j) {
        temp = minimumDistance(src->list[j], *cluster, withPos);
        addPoints(src->list[j], tempList.list[temp], 1);
        pointNum[temp]++;
    }

    for (int i = 0; i < tempList.size; ++i) {
        dividePointCons(pointNum[i], tempList.list[i], 3);
    }

    struct ListPoint outList;
    ListPoint(&outList, src->pointSize, src->size);

    for (int j = 0; j < src->size; ++j) {
        temp = minimumDistance(src->list[j], *cluster, withPos);
        copyPoint(tempList.list[temp], outList.list[j], 1);
    }

    for(int i = 0; i < dst -> height; ++i){
        for(int j = 0; j < dst -> width; ++j){
            putVal(dst, j, i,(unsigned char) outList.list[i*dst -> width + j][2]);
        }
    }

    destroyListPoint(outList);
    destroyListPoint(tempList);
}

