#ifndef _SHELL_H_
#define _SHELL_H_

#include <terminal.h>

/*typedef struct _Shell Shell;*/
/*typedef struct _ShellFuncs ShellFuncs;*/
/*typedef struct _ShellMetaFuncs ShellMetaFuncs;*/
/*typedef struct _ShellEnvVar ShellEnvVar;*/
/*typedef struct _ShellData ShellData;*/
/*typedef struct _ShellEnv ShellEnv;*/

/*struct _ShellEnv{*/
    /*void (*set) (const char* name,const char* value);*/
    /*const char* (*get) (const char* name);*/
    /*const char* (*nameAt) (size_t at);*/
    /*size_t (*size) (void);*/
    /*void (*unset) (const char* name);*/
/*};*/

/*struct _ShellFuncs {*/
    /*ObjectFuncs _base;*/
    /*bool (*open)  (Shell* this, Terminal* parent);*/
    /*void (*close) (Shell* this);*/
    /*int  (*exec)  (Shell* this, int argc, ... );*/
    /*void (*setTerminal) (Shell* this, Terminal *term);*/
    /*Terminal* (*terminal) (Shell* this);*/
    /**/
/*};*/

/*struct _ShellMetaFuncs {*/
    /*ObjectMetaFuncs _base;*/
    /*ObjectMetaFunc  open;*/
    /*ObjectMetaFunc  close;*/
    /*ObjectMetaFunc  exec;*/
    /*ObjectMetaFunc  setTerminal;*/
    /*ObjectMetaFunc  terminal;*/
    /*ObjectMetaFunc  setEnv;*/
    /*ObjectMetaFunc  env;*/
/*};*/

/*struct _ShellEnvVar{*/
    /*const char* _name;*/
    /*const char* _value;*/
/*};*/

/*struct _ShellData{*/
/*};*/

/*struct _Shell {*/
    /*Object      _base;*/
    /*ShellData   _data;*/
/*};*/

OBJECT(Shell,Object,
        METHODS
        (
            METHOD(void,        set,         const char*,    const char*),
            METHOD(const char*, get,         const char*),
            METHOD(const char*, nameAt,      size_t),
            METHOD(size_t,      size,        void),
            METHOD(void,        unset,       const char*),
            METHOD(bool,        open,        Terminal*),
            METHOD(void,        close),
            METHOD(int,         exec,        int argc,       ...),
            METHOD(void,        setTerminal, Terminal*)
        )
)
OBJECT_EXPORT(Shell)

#endif
