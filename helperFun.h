//
// Created by jerome on 2/1/23.
//

#ifndef FUNTIMES_HELPERFUN_H

#define FUNTIMES_HELPERFUN_H


#include <bits/types/FILE.h>

typedef struct {

        unsigned char *data;
        int height;
        int width;
        char *header;
        int headerSize;
        int numOfComp;
        double centroidr;
        double centroidc;
        int area;
        int boundboxPresent;
        int *boundBox; //clockwise starting from top left
        double SOMonentr;
        double SOMonentc;
        double SOMonentm;
        double perimeter;
        double C1;
        double C2;
    } Img;



#endif //FUNTIMES_HELPERFUN_H
void ImgToPgm(FILE *file, Img *img1);
void outImg(FILE *file, Img *img);
void destroyImg(Img *img);
void thresholdHelper(Img* img1);
unsigned char getVal(Img *img, int x, int y, unsigned char defaultVal);
void putVal(Img *img, int x, int y, unsigned char val);
int checkMask(Img *img, int x, int y, int *arr, int xsize, int ysize);
unsigned char checkMaskGeneric(Img *img, int x, int y, int size);
void errode(Img *img, int size);
void dilate(Img *img, int size);
int numberOfObj(Img *img);
void thresholdImage(Img *img1, int limit);
void perPixelTransformation(Img *img1, unsigned char *fun);
void invertImage(Img *img1);
void blowOutColour(Img *img1);
void toZeroAndOne(Img *img1);

Img * copyImg(Img *img);

//takes a ptr to an opened and  pgm file and returns a ptr to an array and
unsigned char* pgmToArray(FILE **img, int height, int width);

Img* pgmExtractor(FILE **pFile);

Img* pgmToImg(FILE *img);

unsigned char findRoot(unsigned char *tree, unsigned char node);

void simplify(Img *img, unsigned char *tree, unsigned char *ptr);

int label(Img *img);

void compsToPGN(Img *img, Img *original);

Img * getComp(Img *img, unsigned char compNum);
Img * setAllComponent(Img *img, unsigned char comp);
