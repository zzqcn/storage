#ifndef __MY_CMD_H__
#define __MY_CMD_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef whitespace
#define whitespace(c) (((c) == ' ') || ((c) == '\t'))
#endif

typedef struct yy_buffer_state* YY_BUFFER_STATE;
void yyerror(char* s);
extern YY_BUFFER_STATE yy_scan_buffer(char*, size_t);
extern YY_BUFFER_STATE yy_scan_string(const char*);
extern int yylex();
extern int yyparse();
extern void yy_delete_buffer(YY_BUFFER_STATE);

void cmd_foo(void);
void cmd_bar(void);
void cmd_quit(void);
int cmd_execute(char* line);

#endif
