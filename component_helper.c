//
// Created by jerome on 2/17/23.
//

#include <bits/types/FILE.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "helperFun.h"

void drawBoundBox(Img *comp){
    for(int i = 0; i < 16; i += 2){
        putVal(comp, comp -> boundBox[i], comp -> boundBox[i+1], 255);
    }
}

void applyComponentToOriginal(Img *original, Img *comp){
    int size = (original->width) * (original->height);
    unsigned char temp;
    for(int i = 0; i < size; i++){
        temp = *(comp -> data + i);
        if(temp != 0 && temp != 1)
            *(original -> data + i) = temp;
    }
}

void printComponentSpec(FILE *file, Img *img){
    char *buffer = malloc(sizeof (char) * 500);
    sprintf(buffer, "BoundBox starting top left going clockwise\n"
           "[%d, %d], [%d, %d], [%d, %d], [%d, %d], [%d, %d], [%d, %d], [%d, %d], [%d, %d]\n",
           img->boundBox[0],
           img->boundBox[1],
           img->boundBox[2],
           img->boundBox[3],
           img->boundBox[4],
           img->boundBox[5],
           img->boundBox[6],
           img->boundBox[7],
           img->boundBox[8],
           img->boundBox[9],
           img->boundBox[10],
           img->boundBox[11],
           img->boundBox[12],
           img->boundBox[13],
           img->boundBox[14],
           img->boundBox[15]); //for debugging purpose
    fwrite(buffer, 1, strlen(buffer), file);
    sprintf(buffer,"centroid row: %.2f\ncentroid col: %.2f\narea: %d\n"
           "second order moment row: %.2f\nsecond order moment col: %.2f\nsecond order moment mix: %.2f"
           "\nperimeter: %.2f\n"
           "C1: %.2f\nC2: %.2f",
           img -> centroidr,
           img -> centroidc,
           img -> area,
           img -> SOMonentr,
           img -> SOMonentc,
           img -> SOMonentm,
           img ->  perimeter,
           img -> C1,
           img -> C2);
    fwrite(buffer, 1, strlen(buffer), file);
    free(buffer);
}


void compsToPGN(Img *img, Img *original){
    int i;
    char *path = malloc(sizeof (char) * 50);
    Img *copy = (img);
    unsigned char transform[256];
    for(i = 0; i < 256; i++)
        transform[i] = i;
    transform[1] = 150;
    FILE *fp;
    for(i = 1; i < img -> numOfComp; i++){
        Img *comp = setAllComponent(img, i);
        drawBoundBox(comp);
        applyComponentToOriginal(original, comp);

        sprintf(path, "output/comp%d/img.pgm", i);
        fp = fopen(path, "w");
        perPixelTransformation(comp, transform); //makes the component more visible
        ImgToPgm(fp, comp);
        fclose(fp);

        sprintf(path, "output/comp%d/spec.txt", i);
        fp = fopen(path, "w");
        printComponentSpec(fp, comp);
        fclose(fp);
        destroyImg(comp);
    }
    char *defaultVal = "Sorry empty :)";
    for(; i < 256; i++){
        sprintf(path, "output/comp%d/img.pgm", i);
        fp = fopen(path, "w");
        fwrite(defaultVal, 1, strlen (defaultVal), fp);
        fclose(fp);

        sprintf(path, "output/comp%d/spec.txt", i);
        fp = fopen(path, "w");
        fwrite(defaultVal, 1, strlen(defaultVal), fp);
        fclose(fp);
    }
    sprintf(path, "output/outputOriginal.pgm");
    fp = fopen(path, "w");
    ImgToPgm(fp, original);
    fclose(fp);

    sprintf(path, "output/outputOriginal.pgm");
    fp = fopen(path, "w");
    ImgToPgm(fp, original);
    fclose(fp);

    sprintf(path, "output/output.txt");
    fp = fopen(path, "w");
    sprintf(path, "number of components: %d", img -> numOfComp);
    fwrite(path, 1, strlen(path), fp);
    fclose(fp);

    free(path);
}