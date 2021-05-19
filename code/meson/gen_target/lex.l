%option noyywrap noinput nounput nodefault case-insensitive

%{
#include "cmd.h"
#include "grammar.h"
%}


%%

"foo"       { return t_FOO; }
"bar"       { return t_BAR; }
"quit"      { return t_QUIT; }

"(" |
")"             { return yytext[0]; }

[0-9]+          { yylval.val = atoi(yytext); return t_NUM; }

[~a-zA-Z0-9\\/\.:_-]+    { yylval.str = yytext; return t_STRING; }

\n              { return t_EOL; }
[ \t]           { }
.               { printf("error input %c\n", *yytext); }

%%

