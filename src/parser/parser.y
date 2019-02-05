%{
    #include <CommandShell.h>
    #include <parser.h>
    #include <lexer.h>
    
    void yyerror(void *scanner, void *data, const char *msg);
%}

%define api.pure full
%define parse.error verbose

%lex-param { void *scanner }
%parse-param { void *scanner }
%parse-param { void *data }

%union {
    char*              identValue;
	char*              stringValue;
    int 	           integerValue;
	float	           floatValue;
	void*              pointerValue;
	ShellCommandArg*   arg;
	ShellCommandArgs*  args;
	ShellCommand*      com;
}

%token			        T_EOL
%token			        T_EOF
%token <stringValue>    T_STRING
%token <integerValue> 	T_INTEGER
%token <floatValue>	    T_FLOAT
%token <identValue>     T_IDENTIFIER
%token <pointerValue>   T_POINTER

%type <arg>             argument 
%type <stringValue>     module command  
%type <args>            arguments
%type <com>             line

%start program

%%

/* http://www.tldp.org/HOWTO/Lex-YACC-HOWTO-6.html 6.2 Recursion: 'right is wrong' */

program:
    program line                          { TAILQ_INSERT_HEAD(&static_cast(CommandShellScript*,data)->_commands,$2,_entries); }
    | line                                { if($1) TAILQ_INSERT_HEAD(&static_cast(CommandShellScript*,data)->_commands,$1,_entries); }
    ;

    
line:
    module command arguments T_EOL        { if(($$ = alloc(ShellCommand))) { $$->_module = $1; $$->_command = $2; $$->_arguments = $3; } else YYERROR; }
    | module command T_EOL                { if(($$ = alloc(ShellCommand))) { $$->_module = $1; $$->_command = $2; $$->_arguments = 0;  } else YYERROR; }
    | command arguments T_EOL             { if(($$ = alloc(ShellCommand))) { $$->_module = 0;  $$->_command = $1; $$->_arguments = $2; } else YYERROR; }
    | command T_EOL                       { if(($$ = alloc(ShellCommand))) { $$->_module = 0;  $$->_command = $1; $$->_arguments = 0; } else YYERROR;}
    | T_EOL                               { $$ = NULL; }
    ;

module:
    T_IDENTIFIER                          { $$ = $1; }
    
command:
    T_STRING                              { $$ = $1; }
    ;
    
arguments:      
    arguments argument                    { TAILQ_INSERT_HEAD($$, $2, _entries); }
    | argument                            { if(($$ = alloc(ShellCommandArgs))) { TAILQ_INIT($$); TAILQ_INSERT_HEAD($$, $1, _entries); } }
    ;
    
argument:
    T_STRING                              { if (($$ = alloc(ShellCommandArg)))  $$->_data = (Variant) { ._type = VAR_STRING,  ._data.s = $1 }; else YYERROR; }
    | T_INTEGER                           { if (($$ = alloc(ShellCommandArg)))  $$->_data = (Variant) { ._type = VAR_INTEGER, ._data.i = $1 }; else YYERROR; }
    | T_FLOAT                             { if (($$ = alloc(ShellCommandArg)))  $$->_data = (Variant) { ._type = VAR_FLOAT,   ._data.f = $1 }; else YYERROR; }
    | T_IDENTIFIER                        { if (($$ = alloc(ShellCommandArg)))  $$->_data = (Variant) { ._type = VAR_STRING,  ._data.s = $1 }; else YYERROR; }
    | T_POINTER                           { if (($$ = alloc(ShellCommandArg)))  $$->_data = (Variant) { ._type = VAR_PTR,     ._data.p = $1 }; else YYERROR;}
%%
