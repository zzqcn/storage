#ifndef __MY_COMPLICATED_H__
#define __MY_COMPLICATED_H__

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

void complicated_foo(void);
int complicated_bar(int a, int b);

#ifdef ENABLE_XXX
void complicated_xxx(int v);
#endif

#ifdef __cplusplus
}
#endif

#endif