#include "common.h"
#include "http_parser.h"

HttpInfo_t *httpInfo __rte_cache_aligned = NULL;

int on_headers_complete(http_parser *_)
{
    (void)_;
    return 1;
}

int on_url(http_parser *_, const char *at, size_t length)
{
    (void)_;
#ifdef DEBUG
    printf("Url: %.*s\n", (int)length, at);
#endif
    httpInfo->String[0] = at;
    httpInfo->StrLen[0] = length;
    return 0;
}

int on_header_field(http_parser *parser, const char *at, size_t length)
{
#ifdef DEBUG
    printf("Header field: %.*s\n", (int)length, at);
#endif

    return 0;
}

int on_header_value(http_parser *parser, const char *at, size_t length)
{
#ifdef DEBUG
    printf("Header value: %.*s\n", (int)length, at);
#endif
    if (parser->anchor_id != -1) {
        httpInfo->String[parser->anchor_id] = at;
        httpInfo->StrLen[parser->anchor_id] = length;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int ret;
    http_parser_settings settings __rte_cache_aligned;
    http_parser parser __rte_cache_aligned;
    char *filename;
    FILE *file;
    long file_length;
    char *data __rte_cache_aligned;
    uint64_t start, stop;
    uint32_t i, N = 0;

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

    posix_memalign((void**)&data, RTE_CACHE_LINE_SIZE, file_length);
    if (fread(data, 1, file_length, file) != (size_t)file_length) {
        fprintf(stderr, "couldn't read entire file\n");
        free(data);
        goto fail;
    }

    memset(&settings, 0, sizeof(settings));
    settings.on_url = on_url;
    //settings.on_header_field = on_header_field;
    settings.on_header_value = on_header_value;
    settings.on_headers_complete = on_headers_complete;

    ret = posix_memalign((void**)&httpInfo, RTE_CACHE_LINE_SIZE, sizeof(HttpInfo_t));
    if (ret != 0) {
        fprintf(stderr, "malloc httpInfo failed\n");
        goto fail;
    }

    start = rte_rdtsc();
    for (i = 0; i < N; i++) {
        ResetHttpInfoAnchors(httpInfo);
        http_parser_init(&parser, HTTP_REQUEST);
        http_parser_execute(&parser, &settings, data, file_length);
#ifdef DEBUG
        PrintHttpInfoAnchors(httpInfo);
#endif
    }
    stop = rte_rdtsc();

    printf(FMT64 " cycles\n", stop - start);

    free(data);
    free(httpInfo);

    // if (nparsed != (size_t)file_length) {
    //   fprintf(stderr,
    //           "Error: %s (%s)\n",
    //           http_errno_description(HTTP_PARSER_ERRNO(&parser)),
    //           http_errno_name(HTTP_PARSER_ERRNO(&parser)));
    //   goto fail;
    // }

    return EXIT_SUCCESS;

fail:
    if (httpInfo != NULL)
        free(httpInfo);
    fclose(file);
    return EXIT_FAILURE;
}
