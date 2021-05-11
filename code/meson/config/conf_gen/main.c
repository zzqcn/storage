#include "config.h"
#include <stdio.h>

int main(void)
{
    printf("max_threads: %d\n", MAX_THREADS);
#ifdef WITH_DPDK
    printf("with DPDK\n");
#else
    printf("without DPDK\n");
#endif
}