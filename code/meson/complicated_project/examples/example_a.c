#include "complicated.h"

int main(int argc, char** argv)
{
    complicated_foo();
    (void)complicated_bar(3, 5);

#ifdef ENABLE_XXX
    complicated_xxx(123);
#endif

    return 0;
}