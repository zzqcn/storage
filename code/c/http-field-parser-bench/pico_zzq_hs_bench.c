#include <hs.h>
#include "common.h"
#include "picohttpparser.h"

hs_database_t *g_anchorDb = NULL;
hs_scratch_t *g_scratch = NULL;

typedef struct MatchContext
{
    HttpInfo_t *httpInfo;
    struct phr_header* header;
} __rte_cache_aligned MatchContext_t;


typedef struct AnchorPattern
{
    const char *regex;
    int anchorId;
} AnchorPattern_t;

const __rte_cache_aligned AnchorPattern_t HTTPANCHOR_PATTERNS[] = {
    {HTTP_USERAGENT,        1},
    {HTTP_CONTENTTYPE,      2},
    {HTTP_HOST,             4},
    {HTTP_REFERER,          7},
    {HTTP_SERVER,           8},
    {HTTP_COOKIE,           9},
    {HTTP_CONTENTLENGTH,    10},
    {HTTP_CONNECTION,       11}
};

void *hsAlloc(size_t size)
{
    int ret;
    void *p = NULL;

    ret = posix_memalign(&p, RTE_CACHE_LINE_SIZE, size);
    if (ret != 0)
        return NULL;

    return p;
}

void hsFree(void *ptr)
{
    free(ptr);
}

static hs_database_t *compileHTTPAnchorDb(void)
{
    hs_database_t *db = NULL;
    uint32_t i, n;
    const char *exps[16];
    uint32_t flags[16];
    uint32_t ids[16];
    int ret;
    hs_compile_error_t *hsErr;

    n = sizeof(HTTPANCHOR_PATTERNS) / sizeof(HTTPANCHOR_PATTERNS[0]);

    for (i = 0; i < n; i++) {
        exps[i] = HTTPANCHOR_PATTERNS[i].regex;
#ifdef DEBUG
        printf("compile anchor: %s\n", exps[i]);
#endif
        flags[i] = HS_FLAG_CASELESS | HS_FLAG_DOTALL;
        ids[i] = i;
    }

    ret = hs_compile_multi(exps, flags, ids, n, HS_MODE_BLOCK, NULL, &db, &hsErr);
    if (ret != HS_SUCCESS) {
        if (hsErr->expression < 0)
            fprintf(stderr, "compile httpAnchorDb failed: %s\n", hsErr->message);
        else
            fprintf(stderr, "compile httpAnchorDb failed: pattern %s with error %s\n",
                   exps[hsErr->expression], hsErr->message);
        hs_free_compile_error(hsErr);
        return NULL;
    }

    return db;
}


int httpAnchorCallback(unsigned int id, unsigned long long from, unsigned long long to,
                       unsigned int flags, void *ctx)
{
    MatchContext_t *p = (MatchContext_t *)ctx;
    const AnchorPattern_t *ptn = &HTTPANCHOR_PATTERNS[id];

#ifdef DEBUG
    printf("anchorCallback: match %s\n", ptn->regex);
#endif

    p->httpInfo->StrLen[ptn->anchorId] = p->header->value_len;
    p->httpInfo->String[ptn->anchorId] = p->header->value;

    return 1;
}

int parseHttp(char *data, uint32_t dataLen, HttpInfo_t *httpInfo)
{
    int ret;
    MatchContext_t ctx __rte_cache_aligned;
    //const char *method;
    //size_t method_len;
    const char *path;
    size_t path_len;
    //int minor_version;
    struct phr_header headers[32] __rte_cache_aligned;
    size_t i, num_headers;

    ctx.httpInfo = httpInfo;
    num_headers = 32;

    //phr_parse_request(data, dataLen, &method, &method_len, &path, &path_len, &minor_version,
    //                  headers, &num_headers, 0);
    phr_parse_request(data, dataLen, &path, &path_len, headers, &num_headers, 0);

    httpInfo->String[0] = path;
    httpInfo->StrLen[0] = path_len;

    for (i = 0; i < num_headers; i++) {
        ctx.header = &headers[i];
        hs_scan(g_anchorDb, headers[i].name, headers[i].name_len, 0, g_scratch, httpAnchorCallback, &ctx);
    }

    return 0;
}

int main(int argc, char **argv)
{
    int ret;
    char *filename;
    FILE *file;
    long file_length;
    char *data __rte_cache_aligned;
    uint64_t start, stop, total = 0;
    uint32_t i, N = 0;
    HttpInfo_t *httpInfo __rte_cache_aligned = NULL;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <file> <loop count>\n", argv[0]);
        return -1;
    }

    filename = argv[1];
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        goto fail;
    }

    N = atoi(argv[2]);
    if (N == 0 || N > INT_MAX) {
        fprintf(stderr, "loop count %u is invalid\n", N);
        goto fail;
    }

    fseek(file, 0, SEEK_END);
    file_length = ftell(file);
    if (file_length == -1) {
        perror("ftell");
        goto fail;
    }
    fseek(file, 0, SEEK_SET);

    data = hsAlloc(file_length);
    if (fread(data, 1, file_length, file) != (size_t)file_length) {
        fprintf(stderr, "couldn't read entire file\n");
        free(data);
        goto fail;
    }

    ret = hs_set_allocator(hsAlloc, hsFree);
    if (ret != HS_SUCCESS) {
        fprintf(stderr, "hs_set_allocator failed\n");
        goto fail;
    }

    g_anchorDb = compileHTTPAnchorDb();
    if (g_anchorDb == NULL)
        goto fail;
    ret = hs_alloc_scratch(g_anchorDb, &g_scratch);
    if (ret != HS_SUCCESS)
        goto fail;

    httpInfo = hsAlloc(sizeof(HttpInfo_t));
    if (httpInfo == NULL) {
        fprintf(stderr, "malloc httpInfo failed\n");
        goto fail;
    }

    start = rte_rdtsc();
    for (i = 0; i < N; i++) {
        ResetHttpInfoAnchors(httpInfo);
        parseHttp(data, file_length, httpInfo);
#ifdef DEBUG
        PrintHttpInfoAnchors(httpInfo);
#endif
    }
    stop = rte_rdtsc();

    printf(FMT64 " cycles\n", stop - start);

    free(data);
    free(httpInfo);

    return 0;

fail:
    if (httpInfo != NULL)
        free(httpInfo);
    fclose(file);
    return EXIT_FAILURE;
}
