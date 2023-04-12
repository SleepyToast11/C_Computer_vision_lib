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

    printf("Hello, which assignment would you want?");
    int option;
    scanf("%d", &option);



    switch (option) {
        case 1:
            return assign1();

        case 2:
            return assign2();
        case 3:
            return assign3();

        default:
            printf("only 1 or 2 is possible for now");
            return -1;
    }
}