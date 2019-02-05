%{
    #include <CommandShell.h>
    #include <parser.h>
%}

  
%option 8bit
%option bison-bridge
/*%option debug*/
%option pointer
/*%option nodefault*/
%option noinput
%option nounput
%option noyywrap
%option reentrant
%option warn

INTEGER     [0-9]+
FLOAT	    [0-9]+\.[0-9]+
STRING 	    [A-Za-z]+[A-Za-z0-9\.]*
WHITE 	    [ ]
NEWLINE	    (\r\n|\n)
IDENTFIER   #[0-9]+
POINTER     @[0-9]+
HEX         0x[0-9]+

%%

{INTEGER}   { yylval->integerValue = (int) strtoul(yytext, NULL, 10); return T_INTEGER;     }

{FLOAT}     { yylval->floatValue = (float) strtof(yytext,NULL); return T_FLOAT;             }

{STRING}    { yylval->stringValue = strdup(yytext); return T_STRING;                        }

{IDENTFIER} { yylval->stringValue = strdup(yytext); return T_IDENTIFIER;                    }

{POINTER}   { yylval->pointerValue = (void*) strtoul(yytext+2, NULL, 16); return T_POINTER; }

{HEX}       { yylval->integerValue = (int) strtoul(yytext, NULL, 16); return T_INTEGER;     }

{NEWLINE}   { return T_EOL; }

{WHITE}     {  }

%%

void yyerror (void *scanner, void *data, const char *msg) {
    printf("error: %s\r\n",msg);
}
