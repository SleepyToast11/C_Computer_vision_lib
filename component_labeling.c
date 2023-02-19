//
// Created by jerome on 2/11/23.
//
#include <string.h>
#include <stdlib.h>
#include "helperFun.h"

unsigned char findRoot(unsigned char *tree, unsigned char node);

void minComponentSize(Img* img, unsigned char *tree, unsigned char *ptr, int compSize) {

    int arr[256] = {0};
    unsigned char fun[256] = { 0 };

    int size = (img->width) * (img->height);

    for (int i = 0; i < size; i++) {
        arr[ (int) *((img->data) + i)]++;
    }

    for (int i = 0; i < 256; i++) {
        if(arr[i] < compSize)
            tree[i] = 0;
        else
            tree[i] = i;
    }

    *ptr = 1; //since no obj can have 1 as index, we can now use the first index of 1 simplifying future algos.

    for(int i = 2; i < 256; i++) //here we start using the first comp as 1
        if(tree[i] != 0) {
            fun[i] = *ptr;
            tree[*ptr] = *ptr;
            (*ptr)++;
        }
    perPixelTransformation(img, fun);
}

inline unsigned char getVal(Img *img, int x, int y, unsigned char defaultVal){
    if(x < 0 || y < 0 || x >= img -> width || y >= img->height)
        return defaultVal;
    return *(img->data + (y* img->width + x));
}

inline unsigned char findRoot(unsigned char *tree, unsigned char node){
    if(node == tree[node])
        return node;
    tree[node] = findRoot(tree, tree[node]);
    return tree[node];
}

void simplify(Img *img, unsigned char *tree, unsigned char *ptr){
    tree[1] = 1;

    for (int i = 2; i < 255; i++) {
        findRoot(tree, i);
    }

    perPixelTransformation(img, tree);
    *ptr = 2;
    unsigned char newTree[256] = { 0 };

    for(int i = 2; i < 256; i++)
        if(tree[i] == (unsigned char) i) {
            newTree[i] = *ptr;
            tree[*ptr] = *ptr;
            (*ptr)++;
        }

    if(*ptr >= 255)
        exit(-1);

    perPixelTransformation(img, newTree);
}

int label(Img *img){
    unsigned char tree[256] = { 0 };
    unsigned char ptr = 2;
    unsigned char tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;

    for(int i = 0; i < img -> height; i++){
        for(int j = 0; j < img -> width; j++){

            if(getVal(img, j, i, 0) != 0) {

                tmp0 = getVal(img, j - 1, i, 0);
                tmp1 = getVal(img, j, i - 1, 0);
                tmp3 = getVal(img, j - 1, i - 1, 0);
                tmp4 = getVal(img, j + 1, i - 1, 0);
                tmp2 = 0;

                if (tmp0 == 0 && tmp1 == 0 && tmp3 == 0 && tmp4 == 0){
                    tree[ptr] = ptr;
                    putVal(img, j, i, ptr);
                    ptr++;
                    if(ptr == 255)
                        simplify(img, tree, &ptr);
                }
                else{
                if(tmp0 != 0){
                    putVal(img, j, i, findRoot(tree, tmp0));
                    tmp2 = findRoot(tree, tmp0);
                }

                if(tmp1 != 0){
                    if(tmp2 == 0){
                        putVal(img, j, i, findRoot(tree, tmp1));
                        tmp2 = findRoot(tree, tmp1);
                    }
                    else{
                        tree[tmp1] = tmp2;
                    }
                }

                    if(tmp3 != 0){
                        if(tmp2 == 0){
                            putVal(img, j, i, findRoot(tree, tmp3));
                            tmp2 = findRoot(tree, tmp3);
                        }
                        else{
                            tree[tmp3] = tmp2;
                        }
                    }

                if(tmp4 != 0){
                    if(tmp2 == 0) {
                        putVal(img, j, i, findRoot(tree, tmp4));
                        tmp2 = findRoot(tree, tmp4);
                    }
                    else{
                        tree[tmp4] = tmp2;
                    }
                }
                }
            }
        }
        }
    simplify(img, tree, &ptr);
    minComponentSize(img, tree, &ptr, 0);
    img -> numOfComp = ptr;
    return ((int) ptr - 1);
}