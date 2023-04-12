#include "../helperFun.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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