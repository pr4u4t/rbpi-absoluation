#include <LuaShell.h> 

#define MYluaL_newlib(L,T,S) (luaL_checkversion(L), lua_createtable(L, 0, S), luaL_setfuncs(L,T,0))



static bool LuaShell_open (LuaShell* this, Terminal* terminal);
static void LuaShell_close (LuaShell* this);
static int  LuaShell_exec (LuaShell* this, int argc, ...);
static void LuaShell_setTerminal (LuaShell* this,Terminal *term);
static Terminal* LuaShell_terminal (LuaShell* this);
static void LuaShell_setScript (LuaShell* this, const char* script);
static const char* LuaShell_script (LuaShell* this);

static LuaShellFuncs LuaShell_vft = {
    ._base = {
        .open 	        = (bool (*) (Shell*, Terminal*)) &LuaShell_open,
        .close          = (void (*) (Shell*)) &LuaShell_close,
        .exec           = (int  (*) (Shell*, int, ...)) &LuaShell_exec,
        .terminal       = (Terminal* (*) (Shell*)) &LuaShell_terminal,
        .setTerminal    = (void (*) (Shell*, Terminal*)) &LuaShell_setTerminal
    },
    .script             = LuaShell_script,
    .setScript          = LuaShell_setScript
};

static LuaShellMetaFuncs LuaShellMeta_vft = {
    .setScript  = META_FUNC(LuaShell,setScript,VAR_VOID,FUNC_FIXED,MARG(VAR_STRING)),
    .script     = META_FUNC(LuaShell,script,VAR_STRING,FUNC_FIXED)
};

OBJECT_REGISTER(LuaShell,OBJECT_SHELL,LuaShell_vft,LuaShellMeta_vft,Shell)

static void register_lua(lua_State* L, const char* name,luaL_Reg* meth,size_t methsize,luaL_Reg* meta,size_t metasize,bool global){
    int lib_id, meta_id;

    lua_createtable(L, 0, 0);
    lib_id = lua_gettop(L);
    luaL_newmetatable(L, name);
    meta_id = lua_gettop(L);
    luaL_setfuncs(L, meta, 0);      
    MYluaL_newlib(L,meth,methsize-1); 
    lua_setfield(L, meta_id, "__index");    
    MYluaL_newlib(L,meta,metasize-1);
    lua_setfield(L, meta_id, "__metatable");
    lua_setmetatable(L, lib_id);
    if(global) lua_setglobal(L, name);
}

Object* LuaShell_CheckUserData(lua_State* l, int n, const ObjectType type){
    return *static_cast(Object**,luaL_checkudata(l, n, type_to_string(type)));
}

/* -- LUA EXPOSE -- */
//Shell
int l_Shell_open(lua_State* l){
    printf("Shell\r\n");
    
    return 1;
}
int l_Shell_close(lua_State * l){
    return 1;
}
int l_Shell_exec(lua_State * l){
    return 1;
}
int l_Shell_setTerminal(lua_State * l){
    return 1;
}
int l_Shell_terminal(lua_State * l){
   return 1; 
}

int l_Shell_destructor(lua_State * l){
   return 1; 
}

luaL_Reg ShellMethods[] = {
    { "open",           l_Shell_open          },
    { "close",          l_Shell_close         },
    { "exec",           l_Shell_exec          },
    { "setTerminal",    l_Shell_setTerminal   },
    { "terminal",       l_Shell_terminal      },
    { NULL,             NULL                  }
};
    
luaL_Reg ShellMeta[] = {
    { "__gc",           l_Shell_destructor    },
    { NULL,             NULL                  }
};

//Terminal
int l_Terminal_open(lua_State* l){
    printf("Terminal\r\n");
    
    return 1;
}
int l_Terminal_read(lua_State * l){
    return 1;
}
int l_Terminal_write(lua_State * l){
    return 1;
}
int l_Terminal_readf(lua_State * l){
    return 1;
}
int l_Terminal_writef(lua_State * l){
   return 1; 
}
int l_Terminal_flush(lua_State * l){
   return 1; 
}
int l_Terminal_close(lua_State * l){
   return 1; 
}
int l_Terminal_setStreamMode(lua_State * l){
   return 1; 
}
int l_Terminal_streamMode(lua_State * l){
   return 1; 
}
int l_Terminal_in(lua_State * l){
   return 1; 
}
int l_Terminal_out(lua_State * l){
   return 1; 
}
int l_Terminal_destructor(lua_State * l){
   return 1; 
}
luaL_Reg TerminalMethods[] = {
    { "open",           l_Terminal_open             },
    { "read",           l_Terminal_read             },
    { "write",          l_Terminal_write            },
    { "readf",          l_Terminal_readf            },
    { "writef",         l_Terminal_writef           },
    { "flush",          l_Terminal_flush            },
    { "close",          l_Terminal_close            },
    { "setStreamMode",  l_Terminal_setStreamMode    },
    { "streamMode",     l_Terminal_streamMode       },
    { "in",             l_Terminal_in               },
    { "out",            l_Terminal_out              },
    { NULL,             NULL                        }
};
    
luaL_Reg TerminalMeta[] = {
    { "__gc",           l_Terminal_destructor },
    { NULL,             NULL                  }
};
//Hardware
int l_Hardware_isr(lua_State* l){
    printf("Hardware\r\n");
    
    return 1;
}
int l_Hardware_close(lua_State * l){
    return 1;
}
int l_Hardware_destructor(lua_State * l){
   return 1; 
}

luaL_Reg HardwareMethods[] = {
    { "isr",            l_Hardware_isr      },
    { "close",          l_Hardware_close    },
    { NULL,             NULL                }
};
    
luaL_Reg HardwareMeta[] = {
    { "__gc",           l_Hardware_destructor   },
    { NULL,             NULL                    }
};

//ADC
int l_ADC_open(lua_State* l){
    Object* plug = 0;
    int gain; 
    int drate; 
    int mode;
    
    if(!(plug = LuaShell_CheckUserData(l, 1, OBJECT_ADC))) return 0;
    gain = luaL_checknumber(l, 2);
    drate = luaL_checknumber(l, 3);
    mode = luaL_checknumber(l, 4);
    lua_pushnumber(l, call(ADC,plug,open,gain,drate,mode));
    
    return 1;
}
int l_ADC_close(lua_State * l){
    Object* plug = 0;
    
    if(!(plug = LuaShell_CheckUserData(l, 1, OBJECT_ADC))) return 0;
    call(ADC,plug,close);
    
    return 1;
}
int l_ADC_mode(lua_State * l){
    Object* plug = 0;
    
    if(!(plug = LuaShell_CheckUserData(l, 1, OBJECT_ADC))) return 0;
    lua_pushnumber(l, call(ADC,plug,mode));
    
    return 1;
}
int l_ADC_setMode(lua_State * l){
    Object* plug = 0;
    int mode;
    
    if(!(plug = LuaShell_CheckUserData(l, 1, OBJECT_ADC))) return 0;
    mode = luaL_checknumber(l, 2);
    call(ADC,plug,setMode,mode);
    
    return 1;
}
int l_ADC_voltage(lua_State* l){
    Object* plug = 0;
    int channel = 0;
    
    if(!(plug = LuaShell_CheckUserData(l, 1, OBJECT_ADC))) return 0;
    channel = luaL_checknumber(l, 2);
    lua_pushnumber(l, call(ADC,plug,voltage,channel));
    
    return 1; 
}
int l_ADC_channels(lua_State * l){
    Object* plug = 0;
    
    if(!(plug = LuaShell_CheckUserData(l, 1, OBJECT_ADC))) return 0;
    lua_pushnumber(l, call(ADC,plug,channels));
    
    return 1; 
}
int l_ADC_destructor(lua_State * l){
    printf("ADC destructor\r\n");
    return 1; 
}

luaL_Reg ADCMethods[] = {
    { "open",           l_ADC_open          },
    { "close",          l_ADC_close         },
    { "mode",           l_ADC_mode          },
    { "setMode",        l_ADC_setMode       },
    { "voltage",        l_ADC_voltage       },
    { "channels",       l_ADC_channels      },
    { NULL,             NULL                }
};
    
luaL_Reg ADCMeta[] = {
    { "__gc",           l_ADC_destructor    },
    { NULL,             NULL                }
};

//Factory
/*int l_Factory_get(lua_State* l){
    printf("Factory\r\n");
    
    return 1;
}*/
int l_Factory_instance(lua_State* l){
    const char* name = 0;
    Factory* fact = 0;
    Object* plug = 0;
    const char* meta = 0;
    Object** udata = 0;
    
    if(!(name = luaL_checkstring(l, 1))) return 0;
    lua_getglobal(l, "_factory");
    if(!(fact = static_cast(Factory*,lua_touserdata(l, -1)))) return 0;
    if(!(plug = call(Factory,fact,hasInstance,name))) return 0;
    if(!(meta = type_to_string(call(Object,plug,type)))) return 0;
    if(!(udata = static_cast(Object**,lua_newuserdata(l, sizeof(Object*))))) return 0;
    if(!(*udata = plug)) return 0;
    
    luaL_getmetatable(l, meta);
    lua_setmetatable(l, -2);
    
    return 1;
}
int l_Factory_instanceById(lua_State * l){
    return 1;
}
int l_Factory_newInstance(lua_State * l){
    return 1;
}
int l_Factory_destructor(lua_State * l){
    printf("Factory destructor");
    return 0; 
}

luaL_Reg FactoryMethods[] = {
    //{ "get",            l_Factory_get           },
    { "instance",       l_Factory_instance      },
    { "instanceById",   l_Factory_instanceById  },
    { "newInstance",    l_Factory_newInstance   },
    { NULL,             NULL                    }
};
    
luaL_Reg FactoryMeta[] = {
    { "__gc",           l_Factory_destructor    },
    { NULL,             NULL                    }
};

//Plugin
int l_Plugin_open(lua_State* l){
    printf("Plugin\r\n");
    return 1;
}
int l_Plugin_close(lua_State * l){
    return 1;
}
int l_Plugin_isOpen(lua_State * l){
    return 1;
}
int l_Plugin_id(lua_State * l){
    return 1;
}
int l_Plugin_setFactory(lua_State * l){
    return 1;
}
int l_Plugin_factory(lua_State * l){
    return 1;
}
int l_Plugin_destructor(lua_State * l){
   return 0; 
}

luaL_Reg PluginMethods[] = {
    { "open",       l_Plugin_open           },
    { "close",      l_Plugin_close          },
    { "isOpen",     l_Plugin_isOpen         },
    { "id",         l_Plugin_id             },
    { "setFactory", l_Plugin_setFactory     },
    { "factory",    l_Plugin_factory        },
    { NULL,         NULL                    }
};
    
luaL_Reg PluginMeta[] = {
    { "__gc",           l_Plugin_destructor    },
    { NULL,             NULL                    }
};

/* -- END -- */

static bool LuaShell_open(LuaShell* this, Terminal* parent){
    if(!this || call(Object,this,isOpen)) return false;
    if(!call(Object,this,open)) return false;
    if(!(this->_data._L = luaL_newstate())) return false;
    
    luaL_openlibs(this->_data._L);
    
    register_lua(this->_data._L,"Factory",FactoryMethods,sizeof(FactoryMethods)/sizeof(luaL_Reg),FactoryMeta,sizeof(FactoryMeta)/sizeof(luaL_Reg),true);
    register_lua(this->_data._L,"Plugin",PluginMethods,sizeof(PluginMethods)/sizeof(luaL_Reg),PluginMeta,sizeof(PluginMeta)/sizeof(luaL_Reg),false);
    register_lua(this->_data._L,"Terminal",TerminalMethods,sizeof(TerminalMethods)/sizeof(luaL_Reg),TerminalMeta,sizeof(TerminalMeta)/sizeof(luaL_Reg),false);
    register_lua(this->_data._L,"Shell",ShellMethods,sizeof(ShellMethods)/sizeof(luaL_Reg),ShellMeta,sizeof(ShellMeta)/sizeof(luaL_Reg),false);
    register_lua(this->_data._L,"Hardware",HardwareMethods,sizeof(HardwareMethods)/sizeof(luaL_Reg),HardwareMeta,sizeof(HardwareMeta)/sizeof(luaL_Reg),false);
    register_lua(this->_data._L,"ADC",ADCMethods,sizeof(ADCMethods)/sizeof(luaL_Reg),ADCMeta,sizeof(ADCMeta)/sizeof(luaL_Reg),false);
    
    lua_pushlightuserdata(this->_data._L, call(Object,this,factory));
    lua_setglobal(this->_data._L, "_factory");
    
    return true;
}

static void LuaShell_close(LuaShell* this){
    if(!this) return;
    if(this->_data._terminal) call(Terminal,this->_data._terminal,close);
    if(this->_data._L) lua_close(this->_data._L);
    call(Object,this,close);
}

static int LuaShell_exec(LuaShell* this, int argc, ...){
    long long int var;
    
    if(!this) return 0;
    if(!this->_data._script) return 0;
    if(!this->_data._L) return 0;
    if(!call(Object,this,isOpen)) return 0;

    if(luaL_loadfile(this->_data._L, this->_data._script))
    { 
        printf("lua error %s\r\n",lua_tostring(this->_data._L, -1)); 
        return 0;
    }

    /* Ask Lua to run our little script */
    if(lua_pcall(this->_data._L, 0, LUA_MULTRET, 0) != 0){ 
        printf("lua error %s\r\n",lua_tostring(this->_data._L, -1)); 
    }

    if (!lua_isnumber(this->_data._L, -1)){
        printf("error lua script must return a number\r\n");
    }
     
    var = lua_tointeger(this->_data._L,-1);
    printf("Script return: %lld\r\n",var);

    lua_pop(this->_data._L, 1);  /* Take the returned value out of the stack */
    return var;
}

static void LuaShell_setTerminal(LuaShell* this,Terminal *term){
    if(this) this->_data._terminal = term;
}

static Terminal* LuaShell_terminal(LuaShell* this){
    return (this) ? this->_data._terminal : NULL;
}

static void LuaShell_setScript (LuaShell* this, const char* script){
    if(this) this->_data._script = strdup(script);
}

static const char* LuaShell_script (LuaShell* this){
    return (this) ? this->_data._script : 0;
}
