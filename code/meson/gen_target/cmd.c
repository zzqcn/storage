#include "cmd.h"

void yyerror(char* s)
{
    fprintf(stderr, "error: %s\n", s);
}

void cmd_foo(void)
{
    printf("cmd foo callback");
}

void cmd_bar(void)
{
    printf("cmd bar callback");
}

void cmd_quit(void)
{
    exit(0);
}

int cmd_execute(char* line)
{
    int ret;
    YY_BUFFER_STATE bp;
    size_t len = strlen(line);

    line[len] = line[len+1] = 0;
    bp = yy_scan_buffer(line, len + 2);
    ret = yyparse();
    yy_delete_buffer(bp);

    return ret;
}