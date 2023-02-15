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

    for(int i = 2; i < 256; i++)
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
    newTree[1] = 1;

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
    unsigned char tmp0, tmp1, tmp2;

    for(int i = 0; i < img -> height; i++){
        for(int j = 0; j < img -> width; j++){

            if(getVal(img, j, i, 0) != 0) {

                tmp0 = getVal(img, j - 1, i, 0);
                tmp1 = getVal(img, j, i - 1, 0);

                if (tmp0 == 0 && tmp1 == 0){
                    tree[ptr] = ptr;
                    putVal(img, j, i, ptr);
                    ptr++;
                    if(ptr == 255)
                        simplify(img, tree, &ptr);
                }

                else if(tmp0 != tmp1){
                    if(tmp0 == 0)
                        putVal(img, j, i, findRoot(tree, tmp1));
                    else if(tmp1 == 0)
                        putVal(img, j, i, findRoot(tree, tmp0));
                    else {
                        tmp2 = findRoot(tree, tmp1);
                        tree[tmp0] = tmp2;
                        putVal(img, j, i, tmp2);
                    }
                } else
                    putVal(img, j, i, findRoot(tree, tmp1));
            }
        }
    }
    simplify(img, tree, &ptr);
    minComponentSize(img, tree, &ptr, 20);
    return ((int) ptr - 2);
}