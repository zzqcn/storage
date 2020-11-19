//#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef FAST_MEMCMP
 #include <rte_config.h>
 #include "rte_memcmp.h"
 #define MEMCMP      rte_memcmp
#else
 #define MEMCMP      memcmp
#endif

//#define KEYWORD     "connection"
//#define KEYWORD_LEN 10
#define KEYWORD     "This call causes the data contained in the indicated user buffer"
#define KEYWORD_LEN 64

int main(int argc, char** argv) {
    FILE* fp;
    char* buff;
    size_t buff_len, n, loop, i, cnt;
    double start=0.0, stop=0.0, secs;
    struct timeval before, after;

#ifdef RTE_MACHINE_CPUFLAG_AVX2
    printf("AVX2 enabled\n");
#else
    printf("AVX2 disabled\n");
#endif

    if(argc < 2)
        return -1;

    fp = fopen(argv[1], "r");
    if(NULL == fp)
        return -1;
    fseek(fp, 0, SEEK_END);
    buff_len = ftell(fp);
    rewind(fp);
    buff = (char*) malloc(buff_len);
    if(NULL == buff)
        return -1;
    n = fread(buff, 1, buff_len, fp);
    cnt = 0;

    gettimeofday(&before, NULL);

    for(loop=0; loop<10000; loop++) {
        for(i=0; i<n; i++) {
            if(0 == MEMCMP(buff+i, KEYWORD, KEYWORD_LEN))
                cnt ++;
        }
    }

    gettimeofday(&after, NULL);
    secs = (after.tv_sec - before.tv_sec) + (after.tv_usec - before.tv_usec)/1000000.0;
    printf("Execution time = %2.3lf seconds\n",secs);
    fflush(stdout);

    printf("hit count: %u\n", cnt);

    free(buff);
    fclose(fp);

    return 0;
}

