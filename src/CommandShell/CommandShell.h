#ifndef _COMMAND_SHELL_H_
#define _COMMAND_SHELL_H_

#include <variant.h>
#include <terminal.h>
#include <shell.h>
#include <sys/queue.h>

typedef void* yyscan_t;
/*
typedef struct _CommandShell CommandShell;
typedef struct _CommandShellFuncs CommandShellFuncs;
typedef struct _CommandShellMetaFuncs CommandShellMetaFuncs;
typedef struct _CommandShellData CommandShellData;
*/
typedef struct _ShellCommand ShellCommand;
typedef struct _ShellCommandArg ShellCommandArg;
typedef struct _ShellCommandArgs ShellCommandArgs;
typedef struct _CommandShellScript CommandShellScript;

struct _ShellCommandArg {
    Variant _data;
    TAILQ_ENTRY(_ShellCommandArg) _entries;
};

struct _ShellCommand {
    char* _module;
    char* _command;
    TAILQ_HEAD(_ShellCommandArgs,_ShellCommandArg) *_arguments;
    TAILQ_ENTRY(_ShellCommand) _entries;
};

struct _CommandShellScript {
    TAILQ_HEAD(_ShellCommands, _ShellCommand) _commands; 
};

/*
struct _CommandShellData {
    Terminal *terminal;
    yyscan_t scanner;
    char prompt[4096];
    CommandShellScript commands;
};

struct _CommandShellFuncs {
    ShellFuncs  _base;
    bool (*command)     (CommandShell* this, const ShellCommand* comm);
    bool (*ffi_exec)    (CommandShell* this, const ShellCommand* comm, int args);
    bool (*basic_exec)  (CommandShell* this, const ShellCommand* comm, int args);
};

struct _CommandShellMetaFuncs {
    ShellMetaFuncs  _base;
    ObjectMetaFunc  command;
    ObjectMetaFunc  ffi_exec;
    ObjectMetaFunc  basic_exec;
};

struct _CommandShell {
    Shell               _base;
    CommandShellData    _data;
};
*/

OBJECT(CommandShell,Object,
       METHODS
       (
            METHOD(bool,    command,    const ShellCommand*),
            METHOD(bool,    ffi_exec,   const ShellCommand*,    int),
            METHOD(bool,    basic_exec, const ShellCommand*,    int)
        )
)

OBJECT_EXPORT(CommandShell)

#endif
