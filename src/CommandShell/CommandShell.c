#include <CommandShell.h>
#include <parser.h>
#include <lexer.h>
#include <ffi.h>
#include <xxhash.h>
#include <factory.h>

#define ERROR_JUMP(errs) { error=errs; goto ERROR; }

typedef struct _CommandShellAction CommandShellAction;

struct _CommandShellAction {
    XXH64_hash_t _hash;
    bool (*callback) (CommandShell* shell, const ShellCommandArgs* args, int argc);
};

static bool CommandShell_open (CommandShell* this, Terminal* terminal);
static void CommandShell_close (CommandShell* this);
static int  CommandShell_exec (CommandShell* this, int argc, ...);
static void CommandShell_setTerminal (CommandShell* this,Terminal *term);
static Terminal* CommandShell_terminal (CommandShell* this);
static bool CommandShell_command (CommandShell* this, const ShellCommand* comm);
static bool CommandShell_ffi_exec (CommandShell* this, const ShellCommand* comm, int args);
static bool CommandShell_basic_exec(CommandShell* this, const ShellCommand* comm, int args);
static Variant CommandShell_ffi_exec_internal(CommandShell* this, const ShellCommand* comm, int args, Object* plugin, ObjectMetaFunc* mfunc);

/* -- command shell actions -- */

static bool CommandShell_action_help (CommandShell* this, const ShellCommandArgs* args, int argc);
static bool CommandShell_action_list (CommandShell* this, const ShellCommandArgs* args, int argc);
static bool CommandShell_action_quit (CommandShell* this, const ShellCommandArgs* args, int argc);
static bool CommandShell_action_stop (CommandShell* this, const ShellCommandArgs* args, int argc);
static bool CommandShell_action_restart (CommandShell* this, const ShellCommandArgs* args, int argc);
static bool CommandShell_action_info (CommandShell* this, const ShellCommandArgs* args, int argc);
static bool CommandShell_action_new (CommandShell* this, const ShellCommandArgs* args, int argc);
static bool CommandShell_action_loaders (CommandShell* this, const ShellCommandArgs* args, int argc);

/* -- helpers -- */

static ffi_type* meta_to_ffi(VariantType type);
static char meta_to_format(VariantType type);

/* -- public driver part -- */

static CommandShellFuncs CommandShell_vft = {
    ._base = {
        .open 	        = (bool (*) (Shell*, Terminal*)) &CommandShell_open,
        .close          = (void (*) (Shell*)) &CommandShell_close,
        .exec           = (int  (*) (Shell*, int, ... )) &CommandShell_exec,
        .terminal       = (Terminal* (*) (Shell*)) &CommandShell_terminal,
        .setTerminal    = (void (*) (Shell*, Terminal*)) &CommandShell_setTerminal
    },
    .command        = &CommandShell_command,
    .ffi_exec       = &CommandShell_ffi_exec,
    .basic_exec     = &CommandShell_basic_exec
};

static CommandShellMetaFuncs CommandShellMeta_vft = {
    .command    = META_FUNC(CommandShell,command,VAR_BOOLEAN,FUNC_FIXED),
    .ffi_exec   = META_FUNC(CommandShell,ffi_exec,VAR_BOOLEAN,FUNC_FIXED),
    .basic_exec = META_FUNC(CommandShell,basic_exec,VAR_BOOLEAN,FUNC_FIXED)
};

static unsigned long long CommandShell_seed;
static CommandShellAction CommandShell_actions[8];

OBJECT_REGISTER(CommandShell,OBJECT_SHELL,CommandShell_vft,CommandShellMeta_vft,Shell,CommandShell_seed = mix(clock(),time(NULL),getpid()),    
            CommandShell_actions[0] = (CommandShellAction) {  XXH64("help",     strlen("help"),     CommandShell_seed), &CommandShell_action_help    },
            CommandShell_actions[1] = (CommandShellAction) {  XXH64("list",     strlen("list"),     CommandShell_seed), &CommandShell_action_list    },
            CommandShell_actions[2] = (CommandShellAction) {  XXH64("quit",     strlen("quit"),     CommandShell_seed), &CommandShell_action_quit    },
            CommandShell_actions[3] = (CommandShellAction) {  XXH64("stop",     strlen("stop"),     CommandShell_seed), &CommandShell_action_stop    },
            CommandShell_actions[4] = (CommandShellAction) {  XXH64("restart",  strlen("restart"),  CommandShell_seed), &CommandShell_action_restart },
            CommandShell_actions[5] = (CommandShellAction) {  XXH64("info",     strlen("info"),     CommandShell_seed), &CommandShell_action_info    },
            CommandShell_actions[6] = (CommandShellAction) {  XXH64("new",      strlen("new"),      CommandShell_seed), &CommandShell_action_new     },
            CommandShell_actions[7] = (CommandShellAction) {  XXH64("loaders",  strlen("loaders"),  CommandShell_seed), &CommandShell_action_loaders }
)

static bool CommandShell_open(CommandShell* this, Terminal* parent){
    if(!this || !parent) return false;
    if(!call(Object,this,open)) return false;
    if(yylex_init(&this->_data.scanner)) return false;

    strncpy(this->_data.prompt,"#>",strlen("#>"));
    TAILQ_INIT(&this->_data.commands._commands);
    call(Shell,this,setTerminal,parent);
    call(Terminal,parent,writef,"%s ",this->_data.prompt);
    call(Terminal,parent,flush);
    
    return true;
}

static void CommandShell_close(CommandShell* this){
    if(!this) return;
    if(this->_data.scanner) yylex_destroy(this->_data.scanner);
    if(this->_data.terminal) call(Terminal,this->_data.terminal,close);
    call(Object,this,close);
}

static int CommandShell_exec(CommandShell* this,int argc, ...){
    if(!this) return 0;
        
    char input[4096];
    memset(input,0,4096);
    ShellCommand *comm = NULL;
    ShellCommandArg* arg = NULL;
    
    if(!this || !this->_data.terminal) return 1;
    if(call(Terminal,this->_data.terminal,read,input,4096) <= 0) return 2;
    
    yy_scan_string(input, this->_data.scanner);
    
    yyparse(this->_data.scanner, &this->_data.commands);
    
    //call parsed commands
    TAILQ_FOREACH_REVERSE(comm, &this->_data.commands._commands, _ShellCommands ,_entries)
        if(comm) call(CommandShell,this,command,comm);
    
    while (!TAILQ_EMPTY(&this->_data.commands._commands)) {
            if(!(comm = TAILQ_FIRST(&this->_data.commands._commands)))
                continue;
            
            TAILQ_REMOVE(&this->_data.commands._commands,comm, _entries);
            
            if(!comm->_arguments) continue;
            
            while(!TAILQ_EMPTY(comm->_arguments)) {
                arg = TAILQ_FIRST(comm->_arguments);
                TAILQ_REMOVE(comm->_arguments,arg, _entries);

                if(arg->_data._type == VAR_STRING)
                    free(arg->_data._data.s);
                
                free(arg);
            }

            free(comm);
    }
    
    TAILQ_INIT(&this->_data.commands._commands);
    
    call(Terminal,call(Shell,this,terminal),writef,"%s ",this->_data.prompt);
    call(Terminal,call(Shell,this,terminal),flush);
}

static void CommandShell_setTerminal (CommandShell* this,Terminal *term){
    if(!this) return;
    this->_data.terminal = term;
}

static Terminal* CommandShell_terminal (CommandShell* this){
    return (this) ? this->_data.terminal : NULL;
}

static bool CommandShell_command(CommandShell* this, const ShellCommand* comm){
    int args = 0;
    ShellCommandArg* arg = NULL;
    
    if(!this) return false;
    if(!comm) return false;
    if(comm->_arguments) TAILQ_FOREACH(arg, comm->_arguments, _entries) ++args;
    if(!comm->_module || !strlen(comm->_module)) return CommandShell_basic_exec(this,comm,args);
    
    return CommandShell_ffi_exec(this,comm,++args);
}

static bool CommandShell_ffi_exec(CommandShell* this, const ShellCommand* comm, int args){
    void* signature[args];
    Factory* factory = 0;
    Object* plugin = 0;
    char* ptr = 0;
    ObjectMetaFunc* mfunc = 0;
    Variant result;
    char format[] = {'%','_','\r','\n'};
    char* error = 0;
    ShellCommandArg* argument;
    
    if(!this || !comm) return false;
    ptr = comm->_module+1;
    if(!(factory = call(Object,this,factory))) ERROR_JUMP("Failed to obtain factory instance\r\n");
    if(!(plugin = call(Factory,factory,instanceById,atol(ptr)))) ERROR_JUMP("Failed to find plugin specified by id\r\n");

    if(comm->_arguments){ 
        int k = 0;
        TAILQ_FOREACH_REVERSE(argument, comm->_arguments, _ShellCommandArgs ,_entries)
            signature[k++] = static_cast(void*,argument->_data._type);
        signature[k] = 0;
    }

    if(!(mfunc = metafunc_find(static_cast(Object*,plugin),comm->_command,args == 1 ? 0 : static_cast(const void**,signature)))) 
        ERROR_JUMP("Failed to find function matching signature\r\n");
    
    result = CommandShell_ffi_exec_internal(this,comm,args,plugin,mfunc);

    if(!(format[1] = meta_to_format(mfunc->_ret))) return false;
    
    if(format[1] == 'f') return call(Terminal,call(Shell,this,terminal),writef,format,result._data.f);
    if(format[1] == 'g') return call(Terminal,call(Shell,this,terminal),writef,format,result._data.d);
        
    return call(Terminal,call(Shell,this,terminal),writef,format,result._data);
    
ERROR:
    call(Terminal,call(Shell,this,terminal),write,error,strlen(error));
    return false;
}

static Variant CommandShell_ffi_exec_internal(CommandShell* this, const ShellCommand* comm, int args, Object* plugin,ObjectMetaFunc* mfunc){
    ffi_cif cif;
    ffi_type* ffiargs[args];
    void* ffivals[args];
    ffi_type *rtype = 0;
    Variant ret = { ._type = VAR_VOID };
    ShellCommandArg* argument;
    int i = 1,j = 0;
    void **func;
    
    if(!(rtype = meta_to_ffi(mfunc->_ret))) return ret;
 
    //this pointer
    ffiargs[j] = &ffi_type_pointer;
    ffivals[j] = &plugin;
    
    if(mfunc->_arguments && *mfunc->_arguments){
        TAILQ_FOREACH_REVERSE(argument, comm->_arguments, _ShellCommandArgs ,_entries){
            if(max(i,j) > args) {
                call(Terminal,call(Shell,this,terminal),writef,"Invalid number of arguments got %d expected %d\r\n",j,args);
                return ret;
            }

            if(!(static_cast(VariantType,mfunc->_arguments[j]) & argument->_data._type)){
                call(Terminal,call(Shell,this,terminal),writef,"Invalid argument type at %d\r\n",j);
                return ret;
            }
            
            
            ffiargs[i] = meta_to_ffi(argument->_data._type);
            ffivals[i] = &argument->_data._data;
            ++i,++j;
        }
    }

    func = static_cast(void**,static_cast(void*,static_cast(Object*,plugin)->_meta->_funcs)+(mfunc->_offset/sizeof(void*)));
    
    switch(mfunc->_type){
        case FUNC_FIXED:
            if(ffi_prep_cif(&cif, FFI_DEFAULT_ABI, args,rtype, ffiargs) != FFI_OK) return ret;
            break;
        case FUNC_VARIADIC:
            if(ffi_prep_cif(&cif, FFI_DEFAULT_ABI, args,rtype, ffiargs) != FFI_OK) return ret;
            break;
    }
    
    ffi_call(&cif,FFI_FN(*func),&ret._data, ffivals);
    
    return ret;
}

static bool CommandShell_basic_exec(CommandShell* this, const ShellCommand* comm, int argc){
    if(!this) return false;
    XXH64_hash_t hash = XXH64(comm->_command,strlen(comm->_command),CommandShell_seed); 
    
    for(CommandShellAction* p =  CommandShell_actions; p <  CommandShell_actions+(sizeof(CommandShell_actions)/sizeof(CommandShellAction)); ++p){    
        if(hash == p->_hash){
            if(p->callback){
                return p->callback(this,comm->_arguments,argc);
            } 
        }
    }
    
    call(Terminal,call(Shell,this,terminal),write,"Command not found\r\n",strlen("Command not found\r\n"));
    return true;
}

/* -- command shell actions -- */

static bool CommandShell_action_help (CommandShell* this, const ShellCommandArgs* args, int argc){
    if(!this) return false;
    
    const char* help =  "list\t\t- print list of plugin instances\r\n"
                        "info ID\t\t- print info about plugin instance, ID is a plugin identifier\r\n"
                        "help\t\t- display this screen\r\n"
                        "quit\t\t- quit from shell terminating conenction\r\n"
                        "stop\t\t- stop entire program\r\n"
                        "restart\t\t- restart entire application\r\n"
                        "new NAME\t- create new plugin NAME instance\r\n"
                        "loaders\t\t- display available plugin loaders\r\n";
    
    call(Terminal,call(Shell,this,terminal),write,help,strlen(help));
    return true;
}

static bool CommandShell_action_list (CommandShell* this, const ShellCommandArgs* args, int argc){
    if(!this) return false;
    const char *ptr = "Module list:\r\n";
    call(Terminal,call(Shell,this,terminal),write,ptr,strlen(ptr));
    call(Factory,call(Object,this,factory),foreach,LAMBDA(void _(void* plug, void* usdata){
        call(Terminal,usdata,writef,"%s: #%d @%x\r\n",call(Object,plug,name),call(Object,plug,id),plug);
    }),call(Shell,this,terminal),FMODE_PLUGIN);
    return true;
}

static bool CommandShell_action_quit (CommandShell* this, const ShellCommandArgs* args, int argc){
    if(!this) return false;
    const char *ptr = "Not implemented\r\n";
    call(Terminal,call(Shell,this,terminal),write,ptr,strlen(ptr));
    return true;
}

static bool CommandShell_action_stop (CommandShell* this, const ShellCommandArgs* args, int argc){
    if(!this) return false;
    const char *ptr = "Not implemented\r\n";
    call(Terminal,call(Shell,this,terminal),write,ptr,strlen(ptr));
    return true;
}

static bool CommandShell_action_restart (CommandShell* this, const ShellCommandArgs* args, int argc){
    if(!this) return false;
    const char *ptr = "Not implemented\r\n";
    call(Terminal,call(Shell,this,terminal),write,ptr,strlen(ptr));
    return true;
}

static bool CommandShell_action_info (CommandShell* this, const ShellCommandArgs* args, int argc){
    if(!this) return false;
    if(argc != 1) return false;
    
    char *ptr = TAILQ_FIRST(args)->_data._data.s;
    ++ptr;
    
    meta_info(static_cast(Object*,call(Factory,call(Object,this,factory),instanceById,atol(ptr))),call(Shell,this,terminal));
    
    return true;
}

static bool CommandShell_action_new (CommandShell* this, const ShellCommandArgs* args, int argc){
    if(!this) return false;
    if(argc != 1) return false;
    
    Object *mod;
    char *ptr;
    
    if(!(ptr = TAILQ_FIRST(args)->_data._data.s)) return false;
    if(!(mod = call(Factory,call(Object,this,factory),newInstance,ptr))) return false;
    
    call(Terminal,call(Shell,this,terminal),writef,"#%d@%x\r\n",call(Object,mod,id),mod);
    
    return true;
}

static bool CommandShell_action_loaders (CommandShell* this, const ShellCommandArgs* args, int argc){
    if(!this) return false;
    const char *ptr = "Object loaders list:\r\n";
    call(Terminal,call(Shell,this,terminal),write,ptr,strlen(ptr));
    call(Factory,call(Object,this,factory),foreach,LAMBDA(void _(void* plug, void* usdata){
        call(Terminal,usdata,writef,"%s\r\n",static_cast(ObjectLoader*,plug)->_funcs->name(plug));
    }),call(Shell,this,terminal),FMODE_LOADER);
    return true;
}


/* -- helpers -- */
static ffi_type* meta_to_ffi(VariantType type){
    switch(type){
        case VAR_VOID:      return &ffi_type_void;
        case VAR_INTEGER:   return &ffi_type_sint;
        case VAR_FLOAT:     return &ffi_type_float;
        case VAR_CHAR:      return &ffi_type_schar;
        case VAR_DOUBLE:    return &ffi_type_double;
        case VAR_PTR:       return &ffi_type_pointer;
        case VAR_STRING:    return &ffi_type_pointer;
        case VAR_BOOLEAN:   return &ffi_type_sint;
    }
    return NULL;
}

static char meta_to_format(VariantType type){
    switch(type){
        case VAR_VOID:      return 0;
        case VAR_INTEGER:   return 'd';
        case VAR_FLOAT:     return 'f';
        case VAR_CHAR:      return 'c';
        case VAR_DOUBLE:    return 'g';
        case VAR_PTR:       return 'x';
        case VAR_STRING:    return 's';
        case VAR_BOOLEAN:   return 'd';
    }
    return 0;
}
