#include "plugin_reg.h"

int main(int argc, char** argv)
{
    struct plugin_reg* reg;
    unsigned i;

    for(i=0; i<g_plugins_count; i++) {
        reg = &g_plugins[i];
        printf("plugin: %s\n", reg->cb_name);
        reg->cb_func();
    }

    return 0;
}