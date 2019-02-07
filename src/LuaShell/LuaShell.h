#ifndef _LUA_SHELL_H_
#define _LUA_SHELL_H_

#include <factory.h>
#include <shell.h>
#include <adc.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

/*typedef struct _LuaShellFuncs LuaShellFuncs;*/
/*typedef struct _LuaShellMetaFuncs LuaShellMetaFuncs;*/
/*typedef struct _LuaShellData LuaShellData;*/
/*typedef struct _LuaShell LuaShell;*/

//typedef struct _LuaContext LuaContext; 
//struct _LuaContext{
//    Factory* _factory;
//    
//};

/*struct _LuaShellFuncs {*/
    /*ShellFuncs  _base;*/
    /*const char* (*script)     (LuaShell* shell);*/
    /*void  (*setScript)        (LuaShell* shell, const char* script);*/
};

struct _LuaShellMetaFuncs {
    ShellMetaFuncs  _base;
    ObjectMetaFunc  setScript;
    ObjectMetaFunc  script;
};

struct _LuaShellData{
    Terminal*   _terminal;
    const char* _script;
    lua_State*  _L;
};

struct _LuaShell {
    Shell           _base;
    LuaShellData    _data;
};

OBJECT(LuaShell,Object
        METHODS
        (
            METHOD(const char*, script),
            METHOD(void,        setScript,      const char*)
        )
)
OBJECT_EXPORT(LuaShell)

#endif
