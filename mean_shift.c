#include <math.h>
#include <malloc.h>
#include "helperFun.h"
#include "stdlib.h"

#define ROUND_2_INT(f) ((int)(f >= 0.0 ? (f + 0.5) : (f - 0.5)))

typedef struct {
    double r;
    double g;
    double b;
    double x;
    double y;
} Pixel;

void rgbImgToPixel(rgbImg *src, Pixel *dst, int x, int y){
    dst->r = getVal(src -> rImg, x, y, 0);
    dst->g = getVal(src -> gImg, x, y, 0);
    dst->b = getVal(src -> bImg, x, y, 0);
    dst->x = x;
    dst->y = y;
}

double RgbEuclidean_distance(Pixel a, Pixel c) {
    return sqrt(pow(a.r - c.r, 2) + pow(a.g - c.g, 2) + pow(a.b - c.b, 2));
}

void rgbSegmentation(rgbImg *data, double kernel_bandwidth, int radius, int MAX_ITERATIONS) {

    int size = data->bImg->width * data->bImg->height;

    int iteration = 0;
    char toggle = 1;  //flip from 1 to 0 whe ^ with flip
    char flip = 1;

    Pixel *image[2];  //contains the future and current pixel arrays flipping between each instead of copying
    image[0] = malloc(sizeof (Pixel) * size);
    image[1] = malloc(sizeof (Pixel) * size);


    //copy the image values in pixel form
    for (int i = 0; i < size; i++) {
        rgbImgToPixel(data, &image[0][i], i % data->bImg->width,i / data->bImg->width);
        rgbImgToPixel(data, &image[1][i], i % data->bImg->width,i / data->bImg->width);
    }
    //values to be used later
    int x, y, j;
    //sets to prevent exit on first go
    double max_shift_distance = INFINITY;


    while (!(max_shift_distance <= 0.25 || iteration++ > MAX_ITERATIONS)) {

        //keep on the lookout to see what iteration number, will  appear far on the right than all
        printf("\t%d\n", iteration);

        max_shift_distance = 0;

        for (int i = 0; i < size; i++) {
            //this pixel's new mean with accompanying weight
            Pixel p_new;
            p_new.r = 0;
            p_new.g = 0;
            p_new.b = 0;
            p_new.x = 0;
            p_new.y = 0;
            double total_weight = 0;


            for (int l = -radius; l <= radius; ++l) {
                for (int m = -radius; m <= radius; ++m) {

                    if(l == 0 && m == 0)
                        continue;

                    x = i % data->bImg->width + l;
                    y = i / data->bImg->width + m;

                    j = y * data->bImg->width + x;

                    if (x < 0 || x > data->bImg->width ||
                        y < 0 || y > data->bImg->height || RgbEuclidean_distance(image[toggle][i], image[toggle][j]) > kernel_bandwidth)
                        continue;

                    double distance = RgbEuclidean_distance(image[toggle][i], image[toggle][j]) / kernel_bandwidth;
                    double weight = exp(-pow(distance, 2) / 2) / sqrt(2 * M_PI);

                    p_new.r += image[toggle][j].r * weight;
                    p_new.g += image[toggle][j].g * weight;
                    p_new.b += image[toggle][j].b * weight;
                    p_new.x += image[toggle][j].x * weight;
                    p_new.y += image[toggle][j].y * weight;
                    total_weight += weight;
                }
            }
            if(total_weight != 0) {
                p_new.r /= total_weight;
                p_new.g /= total_weight;
                p_new.b /= total_weight;
                p_new.y /= total_weight;
                p_new.x /= total_weight;
            }

                double shift_distance = RgbEuclidean_distance(p_new, image[toggle][i]);

                if (shift_distance > max_shift_distance) {
                    max_shift_distance = shift_distance;
                    printf("\n%3lf", max_shift_distance);
                    printf("\nnew %3lf, %3lf, %3lf, %3lf, %3lf", p_new.r, p_new.g, p_new.b, p_new.x, p_new.y);
                    printf("\nold %3lf, %3lf, %3lf, %3lf, %3lf", image[toggle][i].r, image[toggle][i].g, image[toggle][i].b, image[toggle][i].x, image[toggle][i].y);
                }

                image[toggle ^ flip][i] = p_new;
        }
        toggle ^= flip;

    }

    for (int i = 0; i < size; i++) {
        putVal(data->rImg, i % data->bImg->width,i / data->bImg->width, image[toggle][i].r);
        putVal(data->gImg, i % data->bImg->width,i / data->bImg->width, image[toggle][i].g);
        putVal(data->bImg, i % data->bImg->width,i / data->bImg->width, image[toggle][i].b);
    }
    free(image[0]);
    free(image[1]);
}

typedef struct {
    double z;
    double x;
    double y;
} grayPixel;

void imgToGrayPixel(Img *src, grayPixel *dst, int x, int y){
    dst->z = getVal(src, x, y, 0);
    dst->x = x;
    dst->y = y;
}

void segmentation(Img *data, double kernel_bandwidth, int radius, int MAX_ITERATIONS) {

    int size = data->width * data->height;

    int iteration = 0;
    char toggle = 1;
    char flip = 1;

    grayPixel *image[2];
    image[0] = malloc(sizeof (grayPixel) * size);
    image[1] = malloc(sizeof (grayPixel) * size);


    for (int i = 0; i < size; i++) {
        imgToGrayPixel(data, &image[0][i], i % data->width,i / data->width);
        imgToGrayPixel(data, &image[1][i], i % data->width,i / data->width);
    }

    int x, y, j;
    double max_shift_distance = INFINITY;

    while (!(max_shift_distance <= 0.01 || iteration++ > MAX_ITERATIONS)) {
        printf("\t%d\n", iteration);

        max_shift_distance = 0;

        for (int i = 0; i < size; i++) {
            grayPixel p_new;
            p_new.z = 0;
            p_new.x = 0;
            p_new.y = 0;
            double total_weight = 0;

            for (int l = -radius; l <= radius; ++l) {
                for (int m = -radius; m <= radius; ++m) {

                    x = (i % data->width) + l;
                    y = (i / data->width) + m;

                    j = y * data->width + x;

                    if (x < 0 || x > data->width ||
                        y < 0 || y > data->height || abs(image[toggle][i].z - image[toggle][j].z) > kernel_bandwidth)
                        continue;

                    double distance = abs(image[toggle][i].z - image[toggle][j].z) / kernel_bandwidth;
                    double weight = exp(-pow(distance, 2) / 2) / sqrt(2 * M_PI);

                    p_new.z += image[toggle][j].z * weight;
                    p_new.x += image[toggle][j].x * weight;
                    p_new.y += image[toggle][j].y * weight;
                    total_weight += weight;
                }
            }
            if(total_weight != 0) {
                p_new.z /= total_weight;
                p_new.y /= total_weight;
                p_new.x /= total_weight;
            }

            double shift_distance = abs(p_new.z - image[toggle][i].z);

            if (shift_distance > max_shift_distance) {
                max_shift_distance = shift_distance;
                printf("\n%3lf", max_shift_distance);
                printf("\nnew %3lf, %3lf, %3lf", p_new.z, p_new.x, p_new.y);
                printf("\nold %3lf, %3lf, %3lf", image[toggle][i].z, image[toggle][i].x, image[toggle][i].y);
            }
            image[toggle ^ flip][i] = p_new;
        }
        toggle ^= flip;
    }

    for (int i = 0; i < size; i++) {
        putVal(data, i % data->width,i / data->width, image[toggle][i].z);
    }
    free(image[0]);
    free(image[1]);
}

int cmpfunc1 (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

void meanShiftFilter(Img *data, int radius, int delta){
    Img *copy = copyImg(data);

    int temp, mean[4 * radius * radius + radius], counter;

    for (int i = 0; i < data->width; ++i) {
        for (int j = 0; j <  data->height; ++j) {

            counter = 0;

            for (int k = 0; k <= 4 * radius * radius + radius; ++k) {
                mean[k] = 0;
            }

            for (int l = -radius; l <= radius; ++l) {
                for (int m = -radius; m <= radius; ++m) {

                    if(l == 0 && m == 0)
                        continue;

                    temp = getVal(copy, l+i, m+j, -1);

                    mean[(radius) * (l + radius) + m + radius]  = temp;

                    counter++;
                }
            }
            qsort(mean, counter, sizeof(int), cmpfunc1);
            temp = 0;
            for (int k = counter / 2 - delta; k <= counter / 2 + delta; ++k) {
                if(k < 0) {
                    counter += 2;
                    continue;
                }
                temp += mean[k];
            }
            putVal(data, i, j, ROUND_2_INT((double) temp / (double) counter));
        }
    }
    destroyImg(copy);
}


void meanShiftFilterRGB(rgbImg *data, int radius, int delta){
    rgbImg *copy = copyRgbImg(data);

    int temp[3], curr[3], mean[3][4 * radius * radius + radius], counter;

    for (int i = 0; i < data->rImg->width; ++i) {
        for (int j = 0; j <  data->bImg->height; ++j) {

            counter = 0;

            for (int k = 0; k <= 4 * radius * radius; ++k) {
                mean[0][k] = 0;
                mean[1][k] = 0;
                mean[2][k] = 0;
            }

            for (int l = -radius; l <= radius; ++l) {
                for (int m = -radius; m <= radius; ++m) {

                    if(l == 0 && m == 0)
                        continue;

                    temp[0] = getVal(copy->rImg, l+i, m+j, -1);
                    temp[1] = getVal(copy->gImg, l+i, m+j, -1);
                    temp[2] = getVal(copy->bImg, l+i, m+j, -1);

                    if(temp[0] == -1 || temp[1] == -1 || temp[2] == -1)
                        continue;

                    mean[(radius) * (l + radius) + m + radius][0]  = temp[0];
                    mean[(radius) * (l + radius) + m + radius][1]  = temp[1];
                    mean[(radius) * (l + radius) + m + radius][2]  = temp[2];

                    counter++;
                }
            }
            qsort(mean[counter], counter, sizeof(int), cmpfunc1);
            qsort(mean[counter], counter, sizeof(int), cmpfunc1);
            qsort(mean[counter], counter, sizeof(int), cmpfunc1);
            temp[0]=temp[1]=temp[2]=0;
            for (int k = counter / 2 - delta; k <= counter / 2 + delta; ++k) {
                if(k < 0) {
                    counter += 2;
                    continue;
                }
                temp[0]+=mean[k][0];
                temp[1]+=mean[k][1];
                temp[2]+=mean[k][2];
            }
            putVal(data->rImg,i,j,ROUND_2_INT((double)temp[0]/counter));
            putVal(data->gImg,i,j,ROUND_2_INT((double)temp[1]/counter));
            putVal(data->bImg,i,j,ROUND_2_INT((double)temp[2]/counter));
        }
    }
    destroyImg(copy);
}