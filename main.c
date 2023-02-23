#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helperFun.h"

/* for reference
    struct img{
        char *data;
        int height;
        int width;
        char *header;
    };
 */

int main() {
    printf("Hello! This is a simple program without gui to analyse an pgm image and get\n"
           "various data of all the components. The program works linearly in steps and \n"
           "no arguments are used if passed as to try to keep it simple. Here is in order the steps:\n\n"
           "1. You will be prompted to enter the name of a file (ex: image1.pgm)\n\n"
           "2. A threshold helper will print the number of pixel for each possible value with the percent of the total\n"
           "\talso included is a .txt file called binarytreshold which are the values I found for each images after\n"
           "\tthe numbers\n\n"
           "3. A threshold.pgm image will be created in the output file of the binary image that thresholding got\n\n"
           "4. Object counting algorithm will go and count the number of object based on corners. These are often\n"
           "\timprecise as interior shapes count as negative. The number will be printed on the terminal"
           "5. The image will be available to erode and dilate, then outputted to inverted.pgm\n\n"
           "6. The image will perform a connected components analysis and output on the terminal the number of components\n\n"
           "7. Finally, the original image with the modifications will be outputted in output/original.pgm and\n"
           "\tas well as a txt. Also, for each component present, the program will output, in its own folder in output called \n"
           "\tcomp(NUMBEROFCOMPONENT), a stand alone image of the component in a pgm format with the same pixel "
           "\t labeling scheme as required, with all its data point in a .txt file\n\n\n");
    printf("enter full name of file: ");
    char arg[20];
    memset(arg, 0, 20);
    if(0 == scanf("%s", arg))
        exit(-1);
    FILE *fp, *fp1;
    fp = fopen(arg, "r");
    Img *data = pgmToImg(fp);
    Img *original = copyImg(data);
    fclose(fp);

    printf("Threshold helper\npixel val, num of pixels, percentage of pixels\n");
    thresholdHelper(data);

    printf("enter threshold:\n");
    int limit;
    if(scanf("%d", &limit) == 0)
        exit(-1);

    thresholdImage(data, limit);
    fp1 = fopen("output/threshold.pgm", "w");
    outImg(fp1, data);
    fclose(fp1);

    fp1 = fopen("output/inverted.pgm", "w");


    invertImage(data);



    printf("number of object (corner algorithm): %d\n\n", numberOfObj(data));
while(1){
    char filter;
    int filterInt;
    printf("\nDo you want to dilate(d), erode(e) or do nothing (q):");
    scanf("%c", &filter);
    char temp = filter;
    if(filter == '\n')
        continue;
    if(temp == 'd' || temp == 'e'){
        printf("\nby how much (diameter of filter): \n");
        scanf("%d", &filterInt);
        if(temp == 'd')
            dilate(data, filterInt);
        if(temp == 'e')
            errode(data, filterInt);
    }
    else
        break;
}

    outImg(fp1, data);

    printf("\nnumber of components: %d\n", label(data)); //labels image here, could be a call itself though


    fclose(fp1);

    compsToPGN(data, original); //makes all the components separate files
    destroyImg(data);
    destroyImg(original);
    return 0;
}
