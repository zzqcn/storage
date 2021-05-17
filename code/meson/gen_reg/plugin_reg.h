#ifndef __PLUGIN_REG_H__
#define __PLUGIN_REG_H__

#include <stdio.h>

struct plugin_reg
{
    const char* cb_name;
    int (*cb_func)(void);
};

extern struct plugin_reg g_plugins[];
extern const unsigned g_plugins_count;

#endif