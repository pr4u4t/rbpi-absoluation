#include <PythonShell.h>

static bool PythonShell_open (PythonShell* this, Terminal* terminal);
static void PythonShell_close (PythonShell* this);
static int  PythonShell_exec (PythonShell* this, int argc, ...);
static void PythonShell_setTerminal (PythonShell* this,Terminal *term);
static Terminal* PythonShell_terminal (PythonShell* this);
static void PythonShell_setScript (PythonShell* this, const char* script);
static const char* PythonShell_script (PythonShell* this);

static PythonShellFuncs PythonShell_vft = {
    ._base = {
        .open 	        = (bool (*) (Shell*, Terminal*)) &PythonShell_open,
        .close          = (void (*) (Shell*)) &PythonShell_close,
        .exec           = (int  (*) (Shell*, int, ...)) &PythonShell_exec,
        .terminal       = (Terminal* (*) (Shell*)) &PythonShell_terminal,
        .setTerminal    = (void (*) (Shell*, Terminal*)) &PythonShell_setTerminal
    },
    .script             = PythinShell_script,
    .setScript          = PythonShell_setScript
};

static LuaShellMetaFuncs LuaShellMeta_vft = {
    .setScript  = META_FUNC(PythonShell,setScript,VAR_VOID,MARG(VAR_STRING)),
    .script     = META_FUNC(PythonShell,script,VAR_STRING)
};

PLUGIN_LOADER_REGISTER(PythonShell,MODULE_SHELL,LuaShell_vft,LuaShellMeta_vft)
MODULE_INIT(PythonShell,MODULE_SHELL,Shell)

static void python_register(){
    PyMethodDef *def;

    /* create new module and class objects */
    PyObject *module = Py_InitModule("Foo", ModuleMethods);
    PyObject *moduleDict = PyModule_GetDict(module);
    PyObject *classDict = PyDict_New(  );
    PyObject *className = PyString_FromString("Foo");
    PyObject *fooClass = PyClass_New(NULL, classDict, className);
    PyDict_SetItemString(moduleDict, "Foo", fooClass);
    Py_DECREF(classDict);
    Py_DECREF(className);
    Py_DECREF(fooClass);

    /* add methods to class */
    for (def = FooMethods; def->ml_name != NULL; def++) {
        PyObject *func = PyCFunction_New(def, NULL);
        PyObject *method = PyMethod_New(func, NULL, fooClass);
        PyDict_SetItemString(classDict, def->ml_name, method);
        Py_DECREF(func);
        Py_DECREF(method);
    }    
    
}

/* -- LUA EXPOSE -- */
//Shell
int py_Shell_open(lua_State* l){
    printf("Shell\r\n");
    
    return 1;
}
int py_Shell_close(lua_State * l){
    return 1;
}
int py_Shell_exec(lua_State * l){
    return 1;
}
int py_Shell_setTerminal(lua_State * l){
    return 1;
}
int py_Shell_terminal(lua_State * l){
   return 1; 
}

/*int l_Shell_destructor(lua_State * l){
   return 1; 
}*/

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
    Plugin* plug = 0;
    int gain; 
    int drate; 
    int mode;
    
    if(!(plug = LuaShell_CheckUserData(l, 1, MODULE_ADC))) return 0;
    gain = luaL_checknumber(l, 2);
    drate = luaL_checknumber(l, 3);
    mode = luaL_checknumber(l, 4);
    lua_pushnumber(l, call(ADC,plug,open,gain,drate,mode));
    
    return 1;
}
int l_ADC_close(lua_State * l){
    Plugin* plug = 0;
    
    if(!(plug = LuaShell_CheckUserData(l, 1, MODULE_ADC))) return 0;
    call(ADC,plug,close);
    
    return 1;
}
int l_ADC_mode(lua_State * l){
    Plugin* plug = 0;
    
    if(!(plug = LuaShell_CheckUserData(l, 1, MODULE_ADC))) return 0;
    lua_pushnumber(l, call(ADC,plug,mode));
    
    return 1;
}
int l_ADC_setMode(lua_State * l){
    Plugin* plug = 0;
    int mode;
    
    if(!(plug = LuaShell_CheckUserData(l, 1, MODULE_ADC))) return 0;
    mode = luaL_checknumber(l, 2);
    call(ADC,plug,setMode,mode);
    
    return 1;
}
int l_ADC_voltage(lua_State* l){
    Plugin* plug = 0;
    int channel = 0;
    
    if(!(plug = LuaShell_CheckUserData(l, 1, MODULE_ADC))) return 0;
    channel = luaL_checknumber(l, 2);
    lua_pushnumber(l, call(ADC,plug,voltage,channel));
    
    return 1; 
}
int l_ADC_channels(lua_State * l){
    Plugin* plug = 0;
    
    if(!(plug = LuaShell_CheckUserData(l, 1, MODULE_ADC))) return 0;
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
    Plugin* plug = 0;
    const char* meta = 0;
    Plugin** udata = 0;
    
    if(!(name = luaL_checkstring(l, 1))) return 0;
    lua_getglobal(l, "_factory");
    if(!(fact = static_cast(Factory*,lua_touserdata(l, -1)))) return 0;
    if(!(plug = call(Factory,fact,hasInstance,name))) return 0;
    if(!(meta = type_to_string(call(Module,plug,type)))) return 0;
    if(!(udata = static_cast(Plugin**,lua_newuserdata(l, sizeof(Plugin*))))) return 0;
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
