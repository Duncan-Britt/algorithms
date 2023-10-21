#include <stdio.h>
#include <stdlib.h>
#include "sort.h"

void array_println(const int xs[], int size);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Expected usage: ./app 7 2 9 ... n\n");
        exit(-1);
    }
    
    int nums[argc-1];
    
    int i;
    for (i = 1; i < argc; ++i) {
        nums[i-1] = atoi(argv[i]);
    }

    printf(" Input: ");
    array_println(nums, argc-1);

    merge_sort(nums, argc-1);    
    
    printf("Sorted: ");
    array_println(nums, argc-1);
    
    exit(0);
}

void array_println(const int xs[], int size) {
    printf("{");
    int i;
    for (i = 0; i < size-1; ++i) {
        printf("%d,", xs[i]);
    }
    printf("%d}\n", xs[i]);
}
