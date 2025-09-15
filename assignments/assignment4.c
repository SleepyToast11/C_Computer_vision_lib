#include "../helperFun.h"
#include <stdio.h>
#include <stdlib.h>

int assign4(){
    char *path = malloc(sizeof(char) * 50);
    int optionInt0, optionInt1, optionInt2;
    double optionDouble0;
    FILE *fp;

    printf("\n\nwhat type of image would you like (ppm: 1, pgm: 0): ");
    scanf("%d", &optionInt0);

    printf("\n\nfilter(0) or segmentation(1): ");
    scanf("%d", &optionInt1);

    printf("\n\nwhat radius: ");
    scanf("%d", &optionInt2);


    Img *data;
    rgbImg *rgbData;

    if (!optionInt0) {

        data = getImg();

        if(!optionInt1) {
            printf("\nwhat delta: ");
            scanf("%d", &optionInt1);
            meanShiftFilter(data, optionInt2, optionInt1);
        } else {
            segmentation(data, optionDouble0, optionInt2, 500);
        }

        fp = fopen("out.pgm", "w");
        ImgToPgm(fp, data);
        destroyImg(data);
    }
    else {
        rgbData = getRgbImg();

        if(!optionInt1) {
            printf("\nwhat delta: ");
            scanf("%d", &optionInt1);
            meanShiftFilterRGB(rgbData, optionDouble0, optionInt1);
        }
        else
            rgbSegmentation(rgbData, optionDouble0, optionInt2, 500);


        fp = fopen("out.ppm", "w");
        rgbImgToPpm(fp, rgbData);
        destroyRgbImg(rgbData);
    }
    return 0;
}
