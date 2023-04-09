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

Img* getImg(){
    printf("enter full name of file: ");
    char arg[20];
    memset(arg, 0, 20);
    if(0 == scanf("%s", arg))
        exit(-1);
    FILE *fp;
    fp = fopen(arg, "r");
    Img *data = pgmToImg(fp);
    fclose(fp);
    return data;
}

int assign1(){
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

    Img *data = getImg();
    Img *original = copyImg(data);

    FILE *fp;

    printf("Threshold helper\npixel val, num of pixels, percentage of pixels\n");
    thresholdHelper(data);

    printf("enter threshold:\n");
    int limit;
    if(scanf("%d", &limit) == 0)
        exit(-1);

    thresholdImage(data, limit);
    fp = fopen("output/assign1/threshold.pgm", "w");
    outImg(fp, data);
    fclose(fp);

    fp = fopen("output/assign1/inverted.pgm", "w");


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

    outImg(fp, data);

    printf("\nnumber of components: %d\n", label(data)); //labels image here, could be a call itself though


    fclose(fp);

    compsToPGN(data, original); //makes all the components separate files
    destroyImg(data);
    destroyImg(original);
    return 0;
}

int assign2(){
    Img *data = getImg();

    char *path = malloc(sizeof (char) * 50);

    FILE *fp;

    char filter = 0;
    double filterDouble1, filterDouble2;
    int filterint1, filterint2, filterint3, filterint4;
    int size;

    while (1) {
        printf("\nDo you want to do preprocessing (p), edge detection(e), threshold (t) or quit (q): ");
        scanf("%1s", &filter);

        if (filter == 'p') {
            printf("\nDo you want to do histogram manipulation (h) or smoothing(s):");
            scanf("%1s", &filter);

            if (filter == 'h') {

                printf("\nlinear (l) or power law (p): ");
                scanf("%1s", &filter);

                if (filter == 'p')
                    printf("\n\nValues example:c sigma:");

                else
                    printf("\n\nValues example:a b:");

                scanf("%lf %lf", &filterDouble1, &filterDouble2);

                if (filter == 'p')
                    histogramManipulation(data, (void *) perPixelTransformation,
                                          (void *) powerLawTransform, filterDouble1, filterDouble2);

                else
                    histogramManipulation(data, (void *) perPixelTransformation,
                                          (void *) linearTransform, filterDouble1, filterDouble2);
            } else if (filter == 's') {
                printf("\nlinear (l) or median (m): ");
                scanf("%1s", &filter);

                printf("\nwhat size: ");
                scanf("%d", &filterint1);

                if (filter == 'l') {
                    printf("\naveraging(a) or gaussian(g): ");
                    scanf("%1s", &filter);

                    if (filter == 'g') {
                        printf("\nwhat sigma value: ");
                        scanf("%lf", &filterDouble2);

                        smoothingFilter(data, (int) filterint1, applyLinearFilterToPixel,
                                        gaussianConvolution, 1, filterDouble2);
                    } else {
                        smoothingFilter(data, (int) filterint1, applyLinearFilterToPixel,
                                        averagingFilter, 0, 0);
                    }
                } else {
                    smoothingFilter(data, (int) filterint1, medianFilter,
                                    0, 0, 0);
                }
            }
            printf("\n\nYou can now go see the image in output/preprocessed.pgm\n\n");
            fp = fopen("output/preprocessed.pgm", "w");

            ImgToPgm(fp, data);

            fclose(fp);

            continue;
        }


        if (filter == 'e') {
            printf("\nwhat type of edge detection gaussian (g) or other (o): ");
            scanf("%1s", &filter);

            if (!(filter == 'g' || filter == 'o'))
                break;

            printf("\nwhat direction(1: horizontal, 2: vertical, 3 both): ");
            scanf("%d", &filterint1);

            printf("\nreversed (0: no, 1: yes): ");
            scanf("%d", &filterint2);

            if (filter == 'g') {
                printf("\nwhat sigma value: ");
                scanf("%lf", &filterDouble1);


                gaussianGradientEdge(data, filterDouble1, filterint2, filterint1);
            } else if (filter == 'o') {
                printf("\nSobel operator (any num > 0) Prewitt (1) or not (0): ");
                scanf("%d", &filterint3);

                printf("\ncentered (yes: 1, no: 0): ");
                scanf("%d", &filterint4);

                edgeDetector(data, filterint1, filterint2, filterint4, filterint3);
            }
            printf("\n\nYou can now go see the image in output/postprocess.pgm\n\n");
            fp = fopen("output/postprocess.pgm", "w");

            ImgToPgm(fp, data);

            fclose(fp);

            continue;
        }

        if (filter == 't') {
            thresholdHelper(data);

            printf("\nWhat value to threshold at: ");
            scanf("%d", &filterint1);
            thresholdImage(data, filterint1);

            fp = fopen("output/threshold.pgm", "w");

            outImg(fp, data);

            fclose(fp);
        }
        else
            break;
    }

    free(path);
    destroyImg(data);
    return 0;
}

int main() {

    printf("Hello, which assignment would you want?");
    int option;
    scanf("%d", &option);



    switch (option) {
        case 1:
            return assign1();

        case 2:
            return assign2();

        default:
            printf("only 1 or 2 is possible for now");
            return -1;
    }
}
