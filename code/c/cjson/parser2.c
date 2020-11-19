#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "cjson2/cJSON.h"

void print_object(const cJSON* obj)
{
    if (cJSON_IsInvalid(obj)) {
    } else if (cJSON_IsObject(obj) || cJSON_IsArray(obj) || cJSON_IsRaw(obj)) {
    } else {
        printf("\"%s\": ", obj->string);
        if (cJSON_IsNull(obj)) {
            printf("null");
        } else if (cJSON_IsString(obj)) {
            printf("\"%s\"", obj->valuestring);
        } else if (cJSON_IsNumber(obj)) {
            printf("%d", obj->valueint);
        } else if (cJSON_IsBool(obj)) {
            printf("%s", cJSON_IsTrue(obj) ? "true" : "false");
        }
        printf("\n");
    }
}

static char g_errbuf[CJSON_ERRBUF_SIZE];
const char* parse_get_error(const cJSON* obj)
{
    if (cJSON_IsObject(obj) && obj->prev == NULL && obj->next == NULL) {
        snprintf(g_errbuf, CJSON_ERRBUF_SIZE, "object: <root>, line: %d",
                 obj->lineno);
    } else {
        snprintf(g_errbuf, CJSON_ERRBUF_SIZE, "object: %s, line: %d",
                 obj->string, obj->lineno);
    }
    return g_errbuf;
}

int parse_resolution(cJSON* resolution)
{
    cJSON* node = NULL;

    node = cJSON_GetObjectItem(resolution, "width");
    if (NULL == node)
        return -1;
    if (!cJSON_IsNumber(node)) {
        fprintf(stderr, "invalid \"width\" value: %s\n", parse_get_error(node));
        return -1;
    }
    print_object(node);

    node = cJSON_GetObjectItem(resolution, "height");
    if (NULL == node)
        return -1;
    print_object(node);

    return 0;
}

int parse_resolutions(cJSON* resolutions)
{
    int ret;
    cJSON* node = NULL;

    printf("resolutions:\n");
    cJSON_ArrayForEach(node, resolutions)
    {
        ret = parse_resolution(node);
        if (ret < 0)
            return ret;
        printf("------\n");
    }

    return 0;
}

int parse(const char* const s)
{
    int ret            = 0;
    cJSON* root        = NULL;
    cJSON* name        = NULL;
    cJSON* resolutions = NULL;
    char err_buf[CJSON_ERRBUF_SIZE];

    root = cJSON_Parse(s);
    if (NULL == root) {
        fprintf(stderr, "parse error: %s\n", cJSON_GetErrorMsg(err_buf));
        ret = -1;
        goto end;
    }

    name = cJSON_GetObjectItem(root, "name");
    if (NULL == name) {
        fprintf(stderr, "can't find \"name\": %s\n", parse_get_error(root));
        ret = -1;
        goto end;
    }
    print_object(name);

    resolutions = cJSON_GetObjectItem(root, "resolutions");
    if (NULL == resolutions || !cJSON_IsArray(resolutions)) {
        ret = -1;
        goto end;
    }
    ret = parse_resolutions(resolutions);

end:
    cJSON_Delete(root);
    return ret;
}

char* read_file(const char* path)
{
    int ret;
    int fd;
    struct stat st;
    char* buff;
    off_t buff_len;

    fd = open(path, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "open file %s failed\n", path);
        return NULL;
    }

    if ((fstat(fd, &st) != 0) || (!S_ISREG(st.st_mode))) {
        close(fd);
        fprintf(stderr, "get file info failed");
        return NULL;
    }

    buff_len = st.st_size;
    if (buff_len < 1) {
        close(fd);
        fprintf(stderr, "file length is invalid\n");
        return NULL;
    }

    buff = (char*)malloc(buff_len);
    ret  = read(fd, buff, buff_len);
    if (ret < 0) {
        close(fd);
        fprintf(stderr, "read file failed\n");
        return NULL;
    }
    close(fd);
#if DEBUG
    printf("buff: \n%s\n\n", buff);
#endif

    return buff;
}

int main(int argc, char** argv)
{
    int ret    = 0;
    char* buff = NULL;

    if (argc < 2) {
        printf("usage: %s <JSON_file_path>\n", argv[0]);
        return 0;
    }

    buff = read_file(argv[1]);
    if (buff == NULL)
        return -1;

    ret = parse(buff);
    if (ret != 0)
        return ret;

    free(buff);
    return 0;
}