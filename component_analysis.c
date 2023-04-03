//
// Created by jerome on 2/15/23.
//
#include <stdlib.h>
#include "helperFun.h"
#include <math.h>

int D0[] = {-1,-1};
int D1[] = {0, -1};
int D2[] = {1,  -1};
int D3[] = {1,  0};
int D4[] = {1, 1};
int D5[] = {0, 1};
int D6[] = {-1,1};
int D7[] = {-1, 0};
int *DIRECTION[] = {D0, D1, D2, D3, D4, D5, D6, D7};

//O(n) do not use twice and remember to free
Img * getComp(Img *img, unsigned char compNum){
    unsigned char fun[256] = { 0 };
    fun[compNum] = 1;
    Img *compImg = copyImg(img);
    perPixelTransformation(compImg, fun);
    compImg -> numOfComp = 1;
    return compImg;
}

//O(n) do not use twice
void setArea(Img *img){
    int size = img -> width * img -> height;
    int j = 0;
    for(int i = 0; i < size; i++){
        if(*((img -> data) + i) != 0)
            j++;
    }
    img -> area = j;
}

void setCentroid(Img *img){
    int height = img -> height;
    int width = img -> width;
    int row = 0;
    int col = 0;
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            if (*((img->data) + i * width + j ) != 0){
                row += i;
                col += j;
            }
    (img -> centroidr) = (double)row / img -> area;
    (img -> centroidc) = (double) col / img -> area;
    putVal(img, (int) img -> centroidc, (int) img -> centroidr, 255);

}

void setBoundBox(Img *img) {
    img->boundboxPresent = 1;
    img->boundBox = malloc(16 * sizeof(int));
    int ptr = 0;
    for (int i = 0; i < img->height;) {
        for (int j = 0; j < img->width;) {
            switch (ptr) {
                case 0:
                    if (*(img->data + i * img->width + j) != 0) {
                        img->boundBox[0] = j;
                        img->boundBox[1] = i;
                        putVal(img, j, i, 255);
                        j = img->width - 1;
                        ptr = 1;
                        break;
                    } else
                        j++;
                    break;
                case 1:
                    if (*(img->data + i * img->width + j) != 0) {
                        img->boundBox[2] = j;
                        img->boundBox[3] = i;
                        putVal(img, j, i, 255);
                        ptr = 2;
                        i = img->height - 1;
                    } else
                        j--;
                    break;
                case 2:
                    if (*(img->data + i * img->width + j) != 0) {
                        img->boundBox[10] = j;
                        img->boundBox[11] = i;
                        putVal(img, j, i, 255);
                        j = img->width - 1;
                        ptr = 3;
                    } else
                        j++;
                    break;
                case 3:
                    if (*(img->data + i * img->width + j) != 0) {
                        img->boundBox[8] = j;
                        img->boundBox[9] = i;
                        putVal(img, j, i, 255);
                        ptr = 4;
                        goto vertical;
                    } else
                        j--;
                    break;
                default:
                    exit(2);
            }
        }
        switch (ptr) {
            case 0:
                i++;
                break;
            case 1:
                i++;
                break;
            case 2:
                i--;
                break;
            default:
                exit(2);
        }
    }
    exit(2);
    vertical:
    for (int i = 0; i < img->width;) {
        for (int j = 0; j < img->height;) {
            switch (ptr) {
                case 4:
                    if (*(img->data + (j * img->width + i)) != 0) {
                        img->boundBox[14] = i;
                        img->boundBox[15] = j;
                        putVal(img, i, j, 255);
                        j = img->height - 1;
                        ptr = 5;
                        break;
                    } else
                        j++;
                    break;
                case 5:
                    if (*(img->data + (j * img->width + i)) != 0) {
                        img->boundBox[12] = i;
                        img->boundBox[13] = j;
                        ptr = 6;
                        putVal(img, i, j, 255);
                        i = img->width - 1;
                    } else
                        j--;
                    break;
                case 6:
                    if (*(img->data + (j * img->width + i)) != 0) {
                        img->boundBox[4] = i;
                        img->boundBox[5] = j;
                        putVal(img, i, j, 255);
                        j = img->height - 1;
                        ptr = 7;
                    } else
                        j++;
                    break;
                case 7:
                    if (*(img->data + (j * img->width + i)) != 0) {
                        img->boundBox[6] = i;
                        img->boundBox[7] = j;
                        putVal(img, i, j, 255);
                        goto end;
                    } else
                        j--;
                    break;
                default:
                    exit(2);
            }
        }
        switch (ptr) {
            case 4:
                i++;
                break;
            case 5:
                i++;
                break;
            case 6:
                i--;
                break;
            default:
                exit(2);
        }
    }
    end:
        ;
}

int isPerimeter(Img *img, int i, int j){
    if (i == 0 || j == 0 || i == (img -> height)-1 || j == img ->height -1 ||
        getVal(img, j, i-1, 0) == 0 || getVal(img, j, i+1, 0) == 0 ||
        getVal(img, j-1, i, 0) == 0 || getVal(img, j+1, i, 0) == 0 ||
        getVal(img, j-1, i-1, 0) == 0 || getVal(img, j+1, i-1, 0) == 0 ||
        getVal(img, j-1, i+1, 0) == 0 || getVal(img, j+1, i+1, 0) == 0) {
        return 1;
    }
    return 0;
}

void setPerimeter(Img *img) {

    double perimeter = 0;

    //starting at boundbox [0,1] ensures we are top left and algo will go clockwise
    int currentx = img->boundBox[0];
    int currenty = img->boundBox[1];
    int startx = img->boundBox[0];
    int starty = img->boundBox[1];
    int i = 0;

    for (;; i = (i + 1) % 8) {
        if (getVal(img, currentx + (DIRECTION[i])[0], currenty + (DIRECTION[i])[1], 0)) {
            currentx += (DIRECTION[i])[0];
            currentx += (DIRECTION[i])[1];
            i -= 3;
            putVal(img, currentx, currenty, 200);
            break;
        }
    }

        for (;currentx != startx || currenty != starty; i = (i + 1) % 8) {
            if (getVal(img, currentx + (DIRECTION[i])[0], currenty + (DIRECTION[i])[1], 0) != 0) {
                currentx += (DIRECTION[i])[0];
                currenty += (DIRECTION[i])[1];
                if(i % 2 == 0)
                    perimeter += sqrt(2);
                else
                    perimeter += 1;
                i = (i + 5) % 8;
                putVal(img, currentx, currenty, 200);
        }
    }
    if(i % 2 == 0)
        perimeter += sqrt(2);
    else
        perimeter += 1;

    img->perimeter = perimeter;
}
void setC1(Img *img){
    setPerimeter(img);
    img -> C1 = (double) (img -> perimeter) * (img -> perimeter) / (img -> area);
}

void setC2(Img *img) {
    int n = img->height;
    int m = img->width;

    double meanSum = 0;
    double varSum = 0;

    int k = 0;
    for (int i = *(img -> boundBox + 14); i <= *(img -> boundBox + 4); i++) {
        for (int j = *(img -> boundBox + 1); j <= *(img -> boundBox + 9); j++) {
            if (getVal(img, j, i, 1) != 0) {
                // if the edge is at the border, we assume that it will go outside in a diagonal manner, therefore
                //by default we will add sqrt(2) and not 1
                if(isPerimeter(img, i, j) != 0){
                    k++;
                    meanSum += sqrt(
                            pow(((double) (img->centroidr - n)), 2)
                            + pow((double) (img -> centroidc - m), 2));
                }
            }
        }
    }
    meanSum = meanSum / (double) k;

    for (int i = *(img -> boundBox + 14); i < *(img -> boundBox + 4); i++) {
        for (int j = *(img -> boundBox + 1); j < *(img -> boundBox + 9); j++) {
            if (getVal(img, j, i, 1) != 0) {
                if (isPerimeter(img, i, j) != 0){
                    varSum +=pow( sqrt(
                            pow(((double) (img->centroidr - n)), 2)
                            + pow((double) (img -> centroidc - m), 2)) - meanSum, 2);
                }
            }
        }
    }
    varSum /= (double) k;

    img -> C2 = meanSum / sqrt(varSum);
}

void setSOMoment(Img *img){
    img -> SOMonentc = 0;
    img -> SOMonentm = 0;
    img -> SOMonentr = 0;
    for (int i = *(img -> boundBox + 14); i < *(img -> boundBox + 4); i++) {
        for (int j = *(img -> boundBox + 1); j < *(img -> boundBox + 9); j++){
            if (getVal(img, j, i, 1) != 0){
                img -> SOMonentc += (double) pow(i - img -> centroidc , 2);
                img -> SOMonentm += (double) (j - img -> centroidr) * (i - img -> centroidc);
                img -> SOMonentr += (double) pow(j - img -> centroidr, 2);
            }
        }
    }
    img -> SOMonentc /= img -> area;
    img -> SOMonentm /= img -> area;
    img -> SOMonentr /= img -> area;
}

Img * setAllComponent(Img *img, unsigned char comp){
    Img *component = getComp(img, comp);
    setArea(component);
    setCentroid(component);
    setBoundBox(component);
    setPerimeter(component);
    setC1(component);
    setC2(component);
    setSOMoment(component);
    return component;
}