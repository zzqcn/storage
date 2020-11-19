#include <hs.h>
#include "common.h"

hs_database_t *g_anchorDb = NULL;
hs_scratch_t *g_scratch = NULL;

typedef struct MatchContext
{
    HttpInfo_t *httpInfo;
    char *data;
    uint32_t dataLen;
    int anchorId;
    int isHttp;
    uint16_t splitOffset;
} __rte_cache_aligned MatchContext_t;

enum PatternType
{
    PTN_ANCHOR,
    PTN_NEWLINE,
    PTN_HTTPEND
};

typedef struct AnchorPattern
{
    const char *regex;
    int type;
    int anchorId;
} AnchorPattern_t;

const __rte_cache_aligned AnchorPattern_t HTTPANCHOR_PATTERNS[] = {
    {HTTP_USERAGENT,     PTN_ANCHOR,    1},
    {HTTP_CONTENTTYPE,   PTN_ANCHOR,    2},
    {HTTP_HOST,          PTN_ANCHOR,    4},
    {HTTP_REFERER,       PTN_ANCHOR,    7},
    {HTTP_SERVER,        PTN_ANCHOR,    8},
    {HTTP_COOKIE,        PTN_ANCHOR,    9},
    {HTTP_CONTENTLENGTH, PTN_ANCHOR,    10},
    {HTTP_CONNECTION,    PTN_ANCHOR,    11},
    {HTTP_NEWLINE,       PTN_NEWLINE,   -1},
    {HTTP_HTTPEND,       PTN_HTTPEND,   -1}
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
    hs_compile_error_t *hsErr;
    uint32_t i, n;
    const char *exps[16];
    uint32_t flags[16];
    uint32_t ids[16];
    int ret;

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

    if (likely(ptn->type == PTN_ANCHOR)) {
        while (to < (p->dataLen - 1)) {
            if (*(p->data + to) != 0x20)
                break;
            to++;
        }
        if (to < p->dataLen - 1) {
            p->anchorId = ptn->anchorId;
            p->httpInfo->StrLen[p->anchorId] = 0;
            p->httpInfo->String[p->anchorId] = (char *)p->data + to;
        } else
            return 1;
    } else if (ptn->type == PTN_NEWLINE) {
        if (p->anchorId == 0) {
            p->httpInfo->StrLen[p->anchorId] =
                (to > p->splitOffset + 10) ? (to - p->splitOffset - 10) : 0;
            if ((p->httpInfo->StrLen[p->anchorId] > 0) &&
                (' ' == *(p->httpInfo->String[p->anchorId] + p->httpInfo->StrLen[p->anchorId] - 1)))
                p->httpInfo->StrLen[p->anchorId]--;
        } else if (p->anchorId != -1) {
            p->httpInfo->StrLen[p->anchorId] = p->data + to - 1 - p->httpInfo->String[p->anchorId];
            if ((p->httpInfo->StrLen[p->anchorId] > 0) &&
                ('\r' ==
                 *(p->httpInfo->String[p->anchorId] + p->httpInfo->StrLen[p->anchorId] - 1)))
                p->httpInfo->StrLen[p->anchorId]--;
        }
        p->anchorId = -1;
    } else if (ptn->type == PTN_HTTPEND) {
        p->httpInfo->String[HTTP_ANCHOR_HTTPEND] = p->data + to;
        p->httpInfo->StrLen[HTTP_ANCHOR_HTTPEND] = p->dataLen - to;
        p->anchorId = -1;
        return 1;
    }

    return 0;
}

int parseHttp(char *data, uint32_t dataLen, HttpInfo_t *httpInfo)
{
    int ret;
    MatchContext_t ctx __rte_cache_aligned;

    ctx.data = data;
    ctx.dataLen = dataLen;
    ctx.httpInfo = httpInfo;
    ctx.isHttp = 1;

    ctx.anchorId = 0;
    httpInfo->String[0] = data + 4;

    ret = hs_scan(g_anchorDb, data, dataLen, 0, g_scratch, httpAnchorCallback, &ctx);
    if (ret != HS_SUCCESS && ret != HS_SCAN_TERMINATED)
        return ret;

    return ret;
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