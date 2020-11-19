#include <stdlib.h>
#include <time.h>
#include <x86intrin.h>

#define N           0xfffffff
#define SEED        0x1234

//#define NORMAL
//#define USE_SSE
#define USE_AVX

float *a, *b, *c;


#if defined(NORMAL)
void gen_data(void) {
    unsigned i;
    a = (float*) malloc(N*sizeof(float));
    b = (float*) malloc(N*sizeof(float));
    c = (float*) malloc(N*sizeof(float));
    
    srand(SEED);
    for(i=0; i<N; i++) {
        a[i] = b[i] = (float)(rand() % N);
    }
}

void free_data(void) {
    free(a);
    free(b);
    free(c);
}

#if 1
void multiply(void) {
    unsigned i;
    for(i=0; i<N; i++) {
        c[i] = a[i] * b[i];
    }
}
#else
void multiply(void) {
    unsigned i;
    for(i=0; i<(N & ((~(unsigned)0x3))); i+=4) {
        c[i]   = a[i]   * b[i];
        c[i+1] = a[i+1] * b[i+1];
        c[i+2] = a[i+2] * b[i+2];
        c[i+3] = a[i+3] * b[i+3];
    }
    for(; i<N; i++) {
        c[i] = a[i] * b[i];
    }
}
#endif

#elif defined(USE_SSE)

void gen_data(void) {
    unsigned i;
    a = (float*) _mm_malloc(N*sizeof(float), 16);
    b = (float*) _mm_malloc(N*sizeof(float), 16);
    c = (float*) _mm_malloc(N*sizeof(float), 16);
    
    srand(SEED);
    for(i=0; i<N; i++) {
        a[i] = b[i] = (float)(rand() % N);
    }
}

void free_data(void) {
    _mm_free(a);
    _mm_free(b);
    _mm_free(c);
}

void multiply(void) {
    unsigned i;
    __m128 A, B, C;

    for(i=0; i<(N & ((~(unsigned)0x3))); i+=4) {
        A = _mm_load_ps(&a[i]);
        B = _mm_load_ps(&b[i]);
        C = _mm_mul_ps(A, B);
        _mm_store_ps(&c[i], C);
    }
    for(; i<N; i++) {
        c[i] = a[i] * b[i];
    }
}

#elif defined(USE_AVX)
void gen_data(void) {
    unsigned i;
    a = (float*) _mm_malloc(N*sizeof(float), 32);
    b = (float*) _mm_malloc(N*sizeof(float), 32);
    c = (float*) _mm_malloc(N*sizeof(float), 32);
    
    srand(SEED);
    for(i=0; i<N; i++) {
        a[i] = b[i] = (float)(rand() % N);
    }
}

void free_data(void) {
    _mm_free(a);
    _mm_free(b);
    _mm_free(c);
}

void multiply(void) {
    unsigned i;
    __m256 A, B, C;

    for(i=0; i<(N & ((~(unsigned)0x7))); i+=8) {
        A = _mm256_load_ps(&a[i]);
        B = _mm256_load_ps(&b[i]);
        C = _mm256_mul_ps(A, B);
        _mm256_store_ps(&c[i], C);
    }
    for(; i<N; i++) {
        c[i] = a[i] * b[i];
    }
}

#else
#error compile failed
#endif


void print_data(void) {
    printf("%f, %f, %f, %f\n", c[0], c[1], c[N-2], c[N-1]);
}


int main(void) {
    double start=0.0, stop=0.0, msecs;
    struct timeval before, after;
    gen_data(); 

    gettimeofday(&before, NULL);
    multiply();
    gettimeofday(&after, NULL);

    msecs = (after.tv_sec - before.tv_sec)*1000.0 + (after.tv_usec - before.tv_usec)/1000.0;
    print_data();
    printf("Execution time = %2.3lf ms\n", msecs);

    free_data();
    return 0;
}

