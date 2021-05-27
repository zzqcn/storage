#include <string.h>
#include "foo.h"

static int pref_a = 111;
static int pref_b = 222;

int foo_pref_set(const char* pref, int val)
{
    if (!strcmp(pref, "a")) {
        pref_a = val;
    } else if (!strcmp(pref, "b")) {
        pref_b = val;
    } else {
        return -1;
    }

    return 0;
}

int foo_pref_get(const char* pref, int* val)
{
    if (NULL == val)
        return -1;

    if (!strcmp(pref, "a")) {
        *val = pref_a;
    } else if (!strcmp(pref, "b")) {
        *val = pref_b;
    } else {
        return -1;
    }

    return 0;
}

struct foo foo_register = {
    .set = foo_pref_set,
    .get = foo_pref_get,
};
