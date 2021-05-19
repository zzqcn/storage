#include "cmd.h"

int main(int argc, char** argv)
{
    int ret;
    char *line;
    size_t n;

    line = malloc(64+2);
    n = 64;

    while (1) {
        printf("\n>> ");
        ret = getline(&line, &n, stdin);
        if (ret <= 0 || n > 64)
            exit(-1);
        cmd_execute(line);
    }

    free(line);

    return 0;
}