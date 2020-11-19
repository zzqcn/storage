#include "int2str.h"

APPFMT_U16MAP_S  g_u16StrMap[APPFMT_U16MAP_MAX];

void InitU16StrMap(void)
{
    uint32_t i = 0;
    char buf[8];

    for (i = 0; i < APPFMT_U16MAP_MAX; i++) {
        snprintf(buf, 8, "%u", i);
        g_u16StrMap[i].Str    = *(uint64_t *)buf;
        g_u16StrMap[i].StrLen = strlen(buf);
    }
}