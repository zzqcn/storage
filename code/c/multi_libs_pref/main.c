#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "foo.h"

int main(int argc, char **argv)
{
    int ret;
    void *ha = NULL, *hb = NULL;
    struct foo *fa, *fb;
    int val;

    if (argc < 2) {
        printf("usage: %s <libfoo_path> [libfoo2_path]\n", argv[0]);
        return -1;
    }

    ha = dlopen(argv[1], RTLD_NOW);
    if (NULL == ha) {
        printf("dlopen failed\n");
        goto fail;
    }
    fa = dlsym(ha, FOO_REGISTER_NAME);
    if (NULL == fa) {
        printf("dlsym failed\n");
        goto fail;
    }

    if (argc < 3)
        hb = dlopen(argv[1], RTLD_NOW);
    else
        hb = dlopen(argv[2], RTLD_NOW);
    if (NULL == hb) {
        printf("dlopen failed\n");
        return -1;
    }
    fb = dlsym(hb, FOO_REGISTER_NAME);
    if (NULL == fb) {
        printf("dlsym failed\n");
        goto fail;
    }

    printf("ha: %p, fa: %p\n", ha, fa);
    printf("hb: %p, fb: %p\n", hb, fb);

    fa->set("a", 123);
    fb->set("a", 456);
    ret = fa->get("a", &val);
    if (ret != 0) {
        printf("get pref failed\n");
        goto fail;
    }
    printf("fa->a: %d\n", val);
    ret = fb->get("a", &val);
    if (ret != 0) {
        printf("get pref failed\n");
        goto fail;
    }
    printf("fb->a: %d\n", val);

fail:
    if (hb != NULL)
        dlclose(hb);
    if (ha != NULL)
        dlclose(ha);

    return 0;
}