#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#define N 10*1024*1024

struct DATA {
    int a;
    int b;
};

int main(void) {
    int i;
    struct DATA* p = (struct DATA*) malloc(sizeof(struct DATA) * N);
    uint64_t start, stop;

    start = rte_rdtsc();
    for(i=0; i<N; i++) {
        p[i].a = p[i].b;
    }
    stop = rte_rdtsc();

    printf(FMT64 "\n", stop - start);

    return 0;
}

