#include "../helperFun.h"
#include <stdio.h>
#include <stdlib.h>



void rgbIMGToListPoint(rgbImg *img, struct ListPoint *dst){

    double *val = malloc(3 * sizeof (double));



    for(int i = 0; i < (img -> rImg -> height); ++i){
        for(int j = 0; j < (img -> rImg -> width); ++j){

            val[0] = (double) *(((img ->rImg)->data) + (i*(img -> rImg -> width)+j));
            val[1] = (double) *((img ->gImg)->data + (i*(img -> rImg -> width)+j));
            val[2] = (double) *((img ->bImg)->data + (i*(img -> rImg -> width)+j));

    Point(dst->list[i * (img -> rImg -> width) + j],
                  val,
                  3, j, i);
        }
    }
    free(val);
}

int assign3() {

    char *path = malloc(sizeof (char) * 50);
    int optionInt0, optionInt1, optionInt2;
    double optionDouble0;
    char optionChar0;
    FILE *fp;

    printf("\n\nwhat type of image would you like (ppm: 1, pgm: 0): ");
    scanf("%d", &optionInt0);

    printf("\n\nHow many k cluster: ");
    scanf("%d", &optionInt1);

    printf("\n\nWith (1) or without position (0): ");
    scanf("%d", &optionInt2);

    printf("\n\nWhat delta between generation: ");
    scanf("%lf", &optionDouble0);

    if(!optionInt0){
        Img *data= getImg();

    } else{
        rgbImg *data = getRgbImg();
        struct ListPoint imgList;
        struct ListPoint cluster;
        ListPoint(&imgList, 3, (data->rImg->width*data->rImg->height));
        rgbIMGToListPoint(data, &imgList);
        cluster = *genRandClusterImg(data->bImg, optionInt1, 3);

        rgbImg *out = copyRgbImg(data);

        while(1){
            printf("\nHow many generation to run (0 to quit): ");
            scanf("%d", &optionInt0);
            if(!optionInt0)
                break;

            kmean(&imgList, &cluster, optionInt2, optionInt0, optionDouble0);

            listPointToRgbImg(&imgList, out, &cluster, optionInt2);

            fp = fopen("output/threshold.pgm", "w");

            rgbImgToPpm(fp, out);

            fclose(fp);
        }
        destroyRgbImg(out);
        destroyRgbImg(data);
    }

    return 0;
}
