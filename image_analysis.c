#include <memory.h>
#include <stdio.h>
#include "helperFun.h"

void thresholdHelper(Img* img1){
    int arr[256] = { 0 };
    float arrRel[256];
    int sum = 0;
    memset(arr, 0, 256 * sizeof (int));
    int size = (img1 -> width) * (img1 -> height);
    int j;
    for(int i = 0; i < size; i++){
        j =  *((img1 -> data) + i);
        arr[j]++;
        sum++;
    }

    for(int i = 0; i < 256; i++)
        arrRel[i] = (float) arr[i] / (float)sum;
    int k;
    for(int i = 0; i < 256; i++) {
        k = i;
        j = 8;
        printf("%d", k);
        if(k == 0)
            j--;
        while(k != 0){
            j--;
            k /= 10;
        }
        for(; j > 0; j--){
            printf(" ");
        }
    }
    printf("\n");
    //slow, I know
    for(int i = 0; i < 256; i++) {
        j = 8;
        printf("%d", arr[i]);

        if (arr[i] == 0)
            j--;

        while(arr[i] != 0){
            j--;
            arr[i] /= 10;
        }
        for(; j > 0; j--){
            printf(" ");
        }
    }
    printf("\n");
    for(int i = 0; i < 256; i++) {

        printf("%.4f", arrRel[i]);

        printf("  ");
    }
    printf("\n");
}

void putVal(Img *img, int x, int y, unsigned char val){
    if(x < 0 || y < 0 || x >= img -> width || y >= img->height)
        return;
    *(img->data + (y* img->width + x)) = val;
}

int checkMask(Img *img, int x, int y, int *arr, int xsize, int ysize){
    for(int i = 0; i < xsize; i++){
        for (int j = 0; j < ysize; ++j) {
            if(getVal(img, x + i, y + j, 0) != arr[j * xsize + i])
                return 0;
        }
    }
    return 1;
}

unsigned char checkMaskGeneric(Img *img, int x, int y, int size) {
    size = size / 2;
    for (int i = -1 * size; i <= size; i++)
        for (int j = -1 * size; j <= size; j++)
            if (getVal(img, j + x, i + y, 1) != 1) {
                return 0;
            }
    return 1;
}

void errode(Img *img, int size){

    Img *imgCopy = copyImg(img);

    for(int i = 0; i < img -> height; i++){
        for(int j = 0; j < img -> width; j++){
            putVal(img, j, i , checkMaskGeneric(imgCopy, j, i, size));
            }
        }

    destroyImg(imgCopy);
}


//better to chose odd sizes, middle are nonexistent for even
void dilate(Img *img, int size){
    size = size / 2;
    Img *imgCopy = copyImg(img);

    for(int i = 0; i < img -> height; i++){
        for(int j = 0; j < img -> width; j++){

            if(getVal(imgCopy, j, i, 0) == 1){
                for(int x = -1 * size; x <= size; x++)
                    for(int y = -1 * size; y <= size; y++)
                        putVal(img, x + j, y + i, 1);
            }
        }
    }
    destroyImg(imgCopy);
}

int numberOfObj(Img *img){
    int extCorner = 0;
    int intCorner = 0;
    int intMask[][4] =
            {{1, 0, 0, 0},
             {0, 1, 0, 0},
             {0, 0, 1, 0},
             {0, 0, 0, 1}
            };
    int extMask[][4] = {
            {0, 1, 1, 1},
            {1, 0, 1, 1},
            {1, 1, 0, 1},
            {1, 1, 1, 0}
            };
    for(int i = -1; i <= img->height; i++){     //starts at -1 to account for corners at edge of img. same for end
        for(int j = -1; j <= img -> width; j++){
            for (int k = 0; k < 4; ++k) {

                if(1 == checkMask(img, j, i, intMask[k], 2, 2))
                    intCorner++;

                if(1 == checkMask(img, j, i, extMask[k], 2, 2))
                    extCorner++;
            }
        }
    }
    return (intCorner - extCorner) / 4;
}



void thresholdImage(Img *img1, int limit){
    int size = (img1 -> width) * (img1 -> height);
    unsigned char *arr = (img1 -> data);
    for(int i = 0; i < size; i++){
        if((int)arr[i] < limit)
            arr[i] = 0;
        else
            arr[i] = 1;
    }
}

void perPixelTransformation(Img *img1, unsigned char *fun){
    int size = (img1 -> width) * (img1 -> height);
    for(int i = 0; i < size; i++){
        (img1 -> data)[i] = fun[ (int)(img1 -> data)[i]];
    }
}

void invertImage(Img *img1){
    unsigned char arr[] = { 1, 0 };
    perPixelTransformation(img1, arr);
}

void blowOutColour(Img *img1){
    unsigned char arr[] = {0, 255};
    perPixelTransformation(img1, arr);
}

void toZeroAndOne(Img *img1){
    unsigned char arr[2556] = { 0 };
    arr[255] = 1;
    perPixelTransformation(img1, arr);
}

