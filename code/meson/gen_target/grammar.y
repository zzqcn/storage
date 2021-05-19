%{
#include <stdio.h>
#include <string.h>
#include "cmd.h"
%}
 
%union
{
    unsigned int val;
    char* str;
}

%token <val> t_NUM
%token <str> t_STRING
%token t_FOO t_BAR t_QUIT t_EOL

%start cmdline

%%
cmdline:  
    | cmd t_EOL
    ;
    
cmd:  t_FOO     { cmd_foo(); }
    | t_BAR     { cmd_bar(); }
    | t_QUIT    { cmd_quit(); }
    ;

%%
