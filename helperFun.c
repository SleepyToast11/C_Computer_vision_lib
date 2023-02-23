//
// Created by jerome on 2/1/23.
//
//

#include <stdlib.h>
#include <bits/types/FILE.h>
#include <stdio.h>
#include <memory.h>
#include "helperFun.h"

void destroyImg(Img *img){
    free(img -> data);
    free(img -> header);
    if(img -> boundboxPresent)
        free(img -> boundBox);
    free(img);
}

Img * copyImg(Img *img){
    Img *ret = malloc(sizeof (Img));
    ret -> boundboxPresent = 0;

    ret -> header = malloc(100 * sizeof (char));

    memcpy(ret -> header, img -> header, 100);

    ret -> width = img -> width;
    ret -> height = img -> height;
    ret -> headerSize = img -> headerSize;

    ret -> data = malloc(sizeof (char) * ret -> height * ret -> width);
    memcpy((ret -> data), (img -> data), sizeof (unsigned char) * ret -> height * ret -> width);

    return ret;
}

//takes a ptr to an opened and  pgm file and returns a ptr to an array and
unsigned char* pgmToArray(FILE **img, int height, int width){
    if(img == NULL)
        return NULL;

    int curArr = 0;
    unsigned char *arr;
    arr = malloc(sizeof (char) * height * width);     //REMEMBER TO FREE THIS YOU DOOFUS :) done -> destroyimg() :)

    char charBuf[1024];

    int remaining = (int) fread(charBuf, 1, 1024, *img);

    char numBuf[10] = {0};
    int j = 0;
    int i;

    //makes a buffer of 1024 bytes to hopefully load a full block or two and reads values of image into it. then those are atoi into the data part.
    //reduces the amounts of reads and improves the amount of memory usage vs reading the whole file at once.
    while(remaining > 0){
        i = 0;
            while(i < remaining) {
                if(j == 0){
                    while ((charBuf[i] == ' ' || charBuf[i] == '\0' || charBuf[i] == '\n') && (i < remaining))
                        i++;
                    if(i >= remaining)
                        goto load;
                }
                if (charBuf[i] == ' ' || charBuf[i] == '\0' || charBuf[i] == '\n') {
                    arr[curArr] = (unsigned char) atoi(numBuf);
                    curArr++;
                    j = 0;
                    memset(numBuf, 0, 10);
                }
                else{
                    if(j >= 10){
                        printf("numbuf overflow :(\n");
                        free(arr);
                        exit(-1);
                    }
                    numBuf[j] = charBuf[i];
                    j++;
                }
                i++;
            }
        load:
        remaining = (int) fread(charBuf, 1, 128, *img);
    }
    return arr;
}


//could be improved visually by separating repeating elements in separate functions, ain't feeling like it
Img* pgmExtractor(FILE **pFile){
     Img *ret;
    ret = malloc(sizeof( Img));
    ret -> boundboxPresent = 0;
    char buf[1];
    char *header; //null terminated header
    header = malloc(100 * sizeof (char));
    int headCur = 0;
    if( 0 == fread(buf, 1,1, *pFile))
        exit(-1);
    while(!(buf[0] == ' ' || buf[0] == '\0' || buf[0] == '\n')){
        header[headCur] = *buf;
        headCur++;
        if( 0 == fread(buf, 1,1, *pFile))
            exit(-1);
    }
    while((buf[0] == ' ' || buf[0] == '\0' || buf[0] == '\n')){
        header[headCur] = *buf;
        headCur++;
        if( 0 == fread(buf, 1,1, *pFile))
            exit(-1);
    }

    for(int i = 0; i<3;i++) {

        char numbuf[10];
        int numbufcur = 0;
        memset(numbuf, 0, 10);

        while (!(buf[0] == ' ' || buf[0] == '\0' || buf[0] == '\n')) {
            header[headCur] = *buf;
            numbuf[numbufcur] = *buf;
            numbufcur++;
            headCur++;
            if( 0 == fread(buf, 1,1, *pFile))
                exit(-1);
        }

        if (i == 0)
            ret->width = atoi(numbuf);
        if(i == 1)
            ret -> height = atoi(numbuf);
        if(i == 2)
            break;
        //2 is just the max value of a pixel, which is assumed to be 255

        while ((buf[0] == ' ' || buf[0] == '\0' || buf[0] == '\n')) {
            header[headCur] = *buf;
            headCur++;
            if( 0 == fread(buf, 1,1, *pFile))
                exit(-1);
        }

        numbufcur = 0;
        memset(numbuf, 0, 10);
    }



    header[headCur] = *buf; // last read broke while loop, but is required as a part of header
    ret -> headerSize = ++headCur;
    ret -> header = header;
    ret -> data = pgmToArray(pFile, ret -> height, ret -> width);
    return ret;
}

void ImgToPgm(FILE *file, Img *img1){
    fwrite(img1 -> header, sizeof(char), (img1 -> headerSize), file);
    int size = (img1 -> height) * (img1 -> width);
    char write_buf[1024];
    int j = 0;
    unsigned char temp;
    for(int i = 0; i < size; i++){
        if(j + 10 > 1024){
            fwrite(write_buf, sizeof(char), j, file);
            j = 0;
        }
            temp = *((img1 -> data) + i);
        j += snprintf(write_buf + j, 6, "%d ", temp);
        write_buf[j] = ' ';
        j++;
        if((i + 1) % img1 -> width == 0 && i != 0)
            write_buf[j] = '\n';
        else
            write_buf[j] = ' ';
        j++;
    }
    fwrite(write_buf, sizeof(char), j, file);
}

void outImg(FILE *file, Img *img){
    blowOutColour(img);
    ImgToPgm(file, img);
    toZeroAndOne(img);
}

Img* pgmToImg(FILE *img){
    if(img == 0)
        exit(-1);
    return pgmExtractor(&img);
}

