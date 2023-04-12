//
// Created by jerome on 2/1/23.
//

#ifndef FUNTIMES_HELPERFUN_H

#define FUNTIMES_HELPERFUN_H
#define DIRECTION_BOTH 3
#define DIRECTION_VERTICAL 2
#define DIRECTION_HORIZONTAL 1

#include <bits/types/FILE.h>

typedef struct {
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
        unsigned char *data;
    } Img;


typedef struct{
    Img *rImg;
    Img *gImg;
    Img *bImg;
} rgbImg;

struct ListPoint{
    double **list;
    unsigned char pointSize;
    int size;
};

struct filter{
    double* filter;
    int size;
    int halfSize;
    double divider;
};

#endif //FUNTIMES_HELPERFUN_H

rgbImg* ppmToImg(FILE *img);
void destroyRgbImg(rgbImg *img);
void rgbImgToPpm(FILE *file, rgbImg *img);
rgbImg * copyRgbImg(rgbImg *img);
void ImgToPgm(FILE *file, Img *img1);
void outImg(FILE *file, Img *img);
void destroyImg(Img *img);
void thresholdHelper(Img* img1);
void putVal(Img *img, int x, int y, int val);
int checkMask(Img *img, int x, int y, int *arr, int xsize, int ysize);
unsigned char checkMaskGeneric(Img *img, int x, int y, int size);
unsigned char getVal(Img *img, int x, int y, unsigned char defaultVal);
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

//edgeDetector.c
void edgeDetector(Img* img, int direction, int reversed, int centered, int sobel);
void gaussianGradientEdge(Img *img, double sigma, int reversed, int direction);

//filters.c
void histogramManipulation(Img* img, void* pixelTransformation(Img *img, unsigned char *fun),
                  void* transformation(unsigned char *arr, double a, double b), double a, double b);
void linearTransform(unsigned char *arr, double a, double b);
void powerLawTransform(unsigned char *arr, double c, double velar);
void smoothingFilter(Img *img, int size, int function(Img* img, int i, int j, struct filter fil, int size), void (fil)(struct filter* filter1, int size, double a, double b),
                     double a, double b);
int medianFilter(Img* img, int i, int j, struct filter fil, int size);
void averagingFilter(struct filter* filter1, int size, double a, double b);
void histogramStretching(unsigned char *arr, double a, double b);
int applyLinearFilterToPixel(Img *img, int i, int j, struct filter filter, int a);
void gaussianConvolution(struct filter* fil, int size, double c, double sigma);

void ppmToArray(FILE *file, rgbImg* img);
rgbImg* ppmtoRgbImg(FILE *pFile);

//utility
void addPoints(const double *src, double *dst, int size);
void multiplyPoints(const double src, double *dst, int size);
void dividePointCons(const double src, double *dst, int size);
double distancePoints(double *a, double *b, int size, int withPos);
double drand( double low, double high );
void nullDPoint(double *dst, int size);
void nullList(struct ListPoint *list);
Img* getImg();
rgbImg *getRgbImg();

//kmean
int minimumDistance(double *point, struct ListPoint list, int withPos);
void kmean(struct ListPoint *Imglist, struct ListPoint *cluster, int withPos, int numberGen, double genDelta);
struct ListPoint * genRandClusterImg(Img *img, int k, int size);
void rgbIMGToListPoint(rgbImg *img, struct ListPoint *dst);
void IMGToListPoint(Img *img, struct ListPoint *dst);
void destroyListPoint(struct ListPoint listPoint);
void listPointToRgbImg(struct ListPoint *src, rgbImg *dst, struct ListPoint *cluster, int withPos);
void listPointToImg(struct ListPoint *src, Img *dst, struct ListPoint *cluster, int withPos);

void ListPoint(struct ListPoint *dst, int pointSize, int size);
void Point(double *dst, const double* vals, int size, int x, int y);

//assignments
int assign1();
int assign2();
int assign3();