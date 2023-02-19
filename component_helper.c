//
// Created by jerome on 2/17/23.
//

#include <bits/types/FILE.h>
#include "helperFun.h"

void compToPGN(FILE *file, Img *comp){
for(int i = 0; i < 8; i += 2){
    putVal(comp, comp -> boundBox[i], comp -> boundBox[i+1], 255);
}
}