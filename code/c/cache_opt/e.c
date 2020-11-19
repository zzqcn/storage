#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#define N   10*1024*1024
#define ROW 1024
#define COL 10*1024

int main(void) {
    int i, j;
    int* p = (int*) malloc(sizeof(int) * N);
    uint64_t start, stop;

    start = rte_rdtsc();
    for(i=0; i<COL; i++) {
        for(j=0; j<ROW; j++) {
            p[i + j*COL] ++;
        }
    }
    stop = rte_rdtsc();

    printf(FMT64 "\n", stop - start);

    return 0;
}

