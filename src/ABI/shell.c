#ifndef _SHELL_C_
#define _SHELL_C_

#define _Shell_source_
#include <shell.h>


#endif

//static ShellFuncs Shell_vft = {
    //.setEnv = Shell_setEnv,
    //.env    = Shell_env
//};


/*static ShellMetaFuncs ShellMeta_vft = {
    .open           = META_FUNC(Shell,open, VAR_BOOLEAN,FUNC_FIXED,MARG(VAR_PTR)),
    .close          = META_FUNC(Shell,close,VAR_VOID,FUNC_FIXED),
    .exec           = META_FUNC(Shell,exec, VAR_INTEGER,FUNC_VARIADIC,MARG(VAR_INTEGER)),
    .setTerminal    = META_FUNC(Shell,setTerminal,VAR_VOID,FUNC_FIXED,MARG(VAR_PTR)),
    .terminal       = META_FUNC(Shell,terminal,VAR_PTR,FUNC_FIXED),
    //.setEnv         = { ._name = "setEnv",      ._arguments = Shell_setEnv_arguments,      ._ret =  VAR_VOID,    ._offset = offsetof(ShellFuncs,setEnv)                               },            
    //.env            = { ._name = "env",         ._arguments = Shell_env_arguments,         ._ret =  VAR_STRING,  ._offset = offsetof(ShellFuncs,env)                                  }
};*/

//OBJECT_REGISTER(Shell,OBJECT_SHELL,Shell_vft,ShellMeta_vft,Object)

/*
static void Shell_setEnv(Shell* this, const char* name, const char* value){
    if(!this) return;
}

static const char* Shell_env(Shell* this,const char* name){
    if(!this) return NULL;
    
}
*/
