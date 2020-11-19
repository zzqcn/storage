// data from picohttpparser/bench.c

#include "http_parser.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define REQ                                                                                                                        \
    "GET /wp-content/uploads/2010/03/hello-kitty-darth-vader-pink.jpg HTTP/1.1\r\n"                                                \
    "Host: www.kittyhell.com\r\n"                                                                                                  \
    "User-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.6; ja-JP-mac; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3 "             \
    "Pathtraq/0.9\r\n"                                                                                                             \
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"                                                  \
    "Accept-Language: ja,en-us;q=0.7,en;q=0.3\r\n"                                                                                 \
    "Accept-Encoding: gzip,deflate\r\n"                                                                                            \
    "Accept-Charset: Shift_JIS,utf-8;q=0.7,*;q=0.7\r\n"                                                                            \
    "Keep-Alive: 115\r\n"                                                                                                          \
    "Connection: keep-alive\r\n"                                                                                                   \
    "Cookie: wp_ozh_wsa_visits=2; wp_ozh_wsa_visit_lasttime=xxxxxxxxxx; "                                                          \
    "__utma=xxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.x; "                                                             \
    "__utmz=xxxxxxxxx.xxxxxxxxxx.x.x.utmccn=(referral)|utmcsr=reader.livedoor.com|utmcct=/reader/|utmcmd=referral\r\n"             \
    "\r\n"

#ifdef DEBUG
#define LOOP_COUNT 1
#else
#define LOOP_COUNT  10000000
#endif

#ifdef DEBUG
int on_header_field(http_parser *parser, const char *at, size_t length)
{
    printf("Header field: %.*s\n", (int)length, at);
    return 0;
}

int on_header_value(http_parser *parser, const char *at, size_t length)
{
    printf("Header value: %.*s\n", (int)length, at);
    return 0;
}
#endif

int main(void)
{
    struct http_parser parser;
    http_parser_settings settings;
    int i;
    int err;
    size_t parsed;

    http_parser_settings_init(&settings);

#ifdef DEBUG
    settings.on_header_field = on_header_field;
    settings.on_header_value = on_header_value;
#endif

    for (i = 0; i < LOOP_COUNT; i++) {
        http_parser_init(&parser, HTTP_REQUEST);
        parsed = http_parser_execute(&parser, &settings, REQ, sizeof(REQ)-1);
    }

    return 0;
}