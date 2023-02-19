#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helperFun.h"
#include <math.h>

/* for reference
    struct img{
        char *data;
        int height;
        int width;
        char *header;
    };
 */

int main() {
    printf("enter full name of file: \n");
    char arg[20];
    memset(arg, 0, 20);
    if(0 == scanf("%s", arg))
        exit(-1);
    FILE *fp, *fp1;
    fp = fopen(arg, "r");
    Img *data = pgmToImg(fp);
    fclose(fp);

    thresholdHelper(data);

    printf("enter threshold:\n");
    int limit;
    if(scanf("%d", &limit) == 0)
        exit(-1);

    thresholdImage(data, limit);
    fp1 = fopen("output.pgm", "w");
    outImg(fp1, data);
    fclose(fp1);

    printf("%d\n", numberOfObj(data));
    fp1 = fopen("output1.pgm", "w");


    invertImage(data);

    //dilate(data, 2);
    //errode(data, 2);

    printf("%d\n", label(data));

    thresholdHelper(data);


    ImgToPgm(fp1, data);
    fclose(fp1);

    Img *comp = setAllComponent(data, 1);
    destroyImg(comp);

    destroyImg(data);
    return 0;
}
