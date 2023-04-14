#include "../helperFun.h"
#include <stdio.h>
#include <stdlib.h>

int assign3() {

    char *path = malloc(sizeof(char) * 50);
    int optionInt0, optionInt1, optionInt2, optionInt3, optionInt4;
    double optionDouble0;
    FILE *fp;


    printf("random seed please:): ");
    scanf("%d", &optionInt0);

    srand(optionInt0);

    printf("\n\nwhat type of image would you like (ppm: 1, pgm: 0 or thread 16): ");
    scanf("%d", &optionInt0);

    Img *data;
    rgbImg *rgbData;

    if (!optionInt0)
        data = getImg();

    else
        rgbData = getRgbImg();

    printf("\n\nHow many k cluster: ");
    scanf("%d", &optionInt1);

    printf("\n\nWith (1) or without position (0): ");
    scanf("%d", &optionInt2);

    printf("\n\nWhat delta between generation: ");
    scanf("%lf", &optionDouble0);

    while(1) {
        printf("\nHow many image out: ");
        scanf("%d", &optionInt3);

        printf("\nHow many generation to run (0 to quit): ");
        scanf("%d", &optionInt4);

        if(!optionInt4)
            break;

        for (int i = 0; i < optionInt3; ++i) {

            if (!optionInt0) {
                struct ListPoint imgList;
                struct ListPoint cluster;
                ListPoint(&imgList, 3, (data->width * data->height));
                IMGToListPoint(data, &imgList);
                genRandClusterImg(data, &cluster, optionInt1, 3);

                Img *out = copyImg(data);


                kmean(&imgList, &cluster, optionInt2, optionInt4, optionDouble0);

                listPointToImg(&imgList, out, &cluster, optionInt2);

                sprintf(path, "out%d.pgm", i);

                fp = fopen(path, "w");

                ImgToPgm(fp, out);

                fclose(fp);

                destroyListPoint(imgList);
                destroyListPoint(cluster);
                destroyImg(out);
            } else {

                struct ListPoint imgList;
                struct ListPoint cluster;
                ListPoint(&imgList, 3, (rgbData->rImg->width * rgbData->rImg->height));
                rgbIMGToListPoint(rgbData, &imgList);
                genRandClusterImg(rgbData->bImg, &cluster, optionInt1, 3);

                rgbImg *out = copyRgbImg(rgbData);

                kmean(&imgList, &cluster, optionInt2, optionInt4, optionDouble0);

                listPointToRgbImg(&imgList, out, &cluster, optionInt2);

                sprintf(path, "out%d.ppm", i);

                fp = fopen(path, "w");

                rgbImgToPpm(fp, out);

                fclose(fp);


                destroyListPoint(imgList);
                destroyListPoint(cluster);
                destroyRgbImg(out);
            }
        }
    }
    if (!optionInt0)
        destroyImg(data);

    else
        destroyRgbImg(data);

    free(path);
    return 0;
}
