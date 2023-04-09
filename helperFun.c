#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "helperFun.h"

#define BLOCK_SIZE 512

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

    char charBuf[BLOCK_SIZE];

    int remaining = (int) fread(charBuf, 1, BLOCK_SIZE, *img);

    char numBuf[10] = {0};
    int j = 0;
    int i;

    //makes a buffer of 512 bytes to hopefully load a full block or two and reads values of image into it. then those are atoi into the data part.
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
    char write_buf[BLOCK_SIZE];
    int j = 0;
    unsigned char temp;
    for(int i = 0; i < size; i++){
        if(j + 10 > BLOCK_SIZE){
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


/////////////////PPM extraction//////////////////////////


//could be improved visually by separating repeating elements in separate functions, ain't feeling like it
Img* ppmExtractor(FILE **pFile){
    rgbImg *ret;
    ret = malloc(sizeof(rgbImg));

    ret -> bImg -> boundboxPresent = 0;
    ret -> gImg -> boundboxPresent = 0;
    ret -> rImg -> boundboxPresent = 0;

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

        if (i == 0){
            ret -> rImg -> width = atoi(numbuf);
            ret -> gImg -> width = atoi(numbuf);
            ret -> bImg -> width = atoi(numbuf);
        }
        if(i == 1) {
            ret -> rImg -> height = atoi(numbuf);
            ret -> gImg -> height = atoi(numbuf);
            ret -> bImg -> height = atoi(numbuf);
        }
        if(i == 2)
            break;
        //2 is just the max value of a pixel, which is assumed to be 255

        while ((buf[0] == ' ' || buf[0] == '\0' || buf[0] == '\n')) {
            header[headCur] = *buf;
            headCur++;
            if(0 == fread(buf, 1,1, *pFile))
                exit(-1);
        }

        numbufcur = 0;
        memset(numbuf, 0, 10);
    }

    header[headCur] = *buf; // last read broke while loop, but is required as a part of header

    headCur++;
    ret -> rImg -> headerSize = headCur;
    ret -> gImg -> headerSize = headCur;
    ret -> bImg -> headerSize = headCur;


    ret -> rImg -> header = header;

    ret -> gImg -> header = malloc(100 * sizeof (char));
    memcpy(ret -> gImg -> header, header, 100);

    ret -> bImg -> header = malloc(100 * sizeof (char));
    memcpy(ret -> bImg -> header, header, 100);

    ppmToArray(pFile, ret);
    return ret;
}

void rgbImgToPpm(FILE *file, rgbImg *img){
    fwrite(img -> bImg -> header, sizeof(char), (img -> bImg -> headerSize), file);
    int size = (img -> bImg -> height) * (img -> bImg -> width) * 3;

    int partNum = 0; //there is obviously "better" ways, but this makes it easy to understand without logic with little more memory use
    Img *part[] = {img -> rImg, img -> gImg, img -> bImg};
    int curArr[] = {0, 0, 0};

    char write_buf[BLOCK_SIZE];
    int j = 0;
    unsigned char temp;

    for(int i = 0; i < size; i++){
        if(j + 10 > BLOCK_SIZE){
            fwrite(write_buf, sizeof(char), j, file);
            j = 0;
        }
        temp = *((part[partNum % 3] -> data) + curArr[partNum % 3]);
        curArr[partNum % 3]++;
        partNum++;

        j += snprintf(write_buf + j, 6, "%d", temp);
        write_buf[j] = ' ';
        j++;
        if((i + 1) % (img -> bImg -> width * 3) == 0 && i != 0) {
            write_buf[j] = '\n';
            j++;
        }
    }
    fwrite(write_buf, sizeof(char), j, file);
}

rgbImg* ppmToImg(FILE *img){
    if(img == 0)
        exit(-1);
    return ppmExtractor(&img);
}

void destroyRgbImg(rgbImg *img){
    destroyImg(img -> rImg);
    destroyImg(img -> gImg);
    destroyImg(img -> bImg);
    free(img);
}

Img * copyRgbImg(rgbImg *img){
    rgbImg *ret = malloc(sizeof (rgbImg));

    ret -> bImg = copyImg(img -> bImg);
    ret -> gImg = copyImg(img -> gImg);
    ret -> rImg = copyImg(img -> rImg);
}

//takes a ptr to an opened and  pgm file and returns a ptr to an array and
void ppmToArray(FILE **file, rgbImg* img){

    int partNum = 0;
    Img *part[] = {img -> rImg, img -> gImg, img -> bImg};
    int curArr[] = {0, 0, 0};

    for (int i = 0; i < 3; ++i) {
        (part[i])->data = malloc(sizeof (char) * img -> bImg -> height * img -> bImg -> width);
    }

    char charBuf[BLOCK_SIZE];

    int remaining = (int) fread(charBuf, 1, BLOCK_SIZE, *file);

    char numBuf[10] = {0};
    int j = 0;
    int i;

    //makes a buffer of 512 bytes to hopefully load a full block or two and reads values of image into it. then those are atoi into the data part.
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
                part[partNum % 3] -> data[curArr[partNum % 3]] = (unsigned char) atoi(numBuf);
                curArr[partNum % 3]++;
                j = 0;
                memset(numBuf, 0, 10);
                partNum++;
            }
            else{
                if(j >= 10){
                    printf("numbuf overflow :(\n");
                    exit(-1);
                }
                numBuf[j] = charBuf[i];
                j++;
            }
            i++;
        }
        load:
        remaining = (int) fread(charBuf, sizeof (char), 128, *file);
    }
}