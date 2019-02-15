#ifndef _LUA_SHELL_H_
#define _LUA_SHELL_H_

#include <factory.h>
#include <shell.h>
#include <adc.h>
#include <python.h>

/*
typedef struct _PythonShellFuncs PythonShellFuncs;
typedef struct _PythonShellMetaFuncs PythonShellMetaFuncs;
typedef struct _PythonShellData PythonShellData;
typedef struct _PythonShell PythonShell;



struct _PythonShellFuncs {
    ShellFuncs  _base;
    const char* (*script)   (PythonShell* shell);
    void  (*setScript)      (PythonShell* shell, const char* script);
};

struct _PythonShellMetaFuncs {
    ShellMetaFuncs  _base;
    ModuleMetaFunc  setScript;
    ModuleMetaFunc  script;
};

struct _PythonShellData{
    Terminal*   _terminal;
    const char* _script;
    //lua_State*  _L;
};

struct _PythonShell {
    Shell           _base;
    LuaShellData    _data;
};
*/

OBJECT(PythonShell,Object,
       METHODS
       (
            METHOD(const char,  script),
            METHOD(void,        setScript,  const char)
        )
)

MODULE_LOADER(PythonShell) 
