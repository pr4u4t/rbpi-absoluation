#include <mloader.h>

static bool MLoader_open (MLoader* this, const char* dir);
static bool MLoader_load (MLoader* this,const char* name);
static bool MLoader_unload (MLoader* this, const char* name);
static Object* MLoader_instance (MLoader* this, const char* name);
static Object* MLoader_instanceById (MLoader* this, const ObjectID id);
static void MLoader_close (MLoader* this);
static bool MLoader_loadByPath (MLoader* this, const char* path, const char* name);
static Object* MLoader_newInstance (MLoader* this, const char* name);
static Object* MLoader_hasInstance (MLoader* this, const char* name); 
static ObjectLoader* MLoader_hasLoader (MLoader* this, const char* name);
static int MLoader_foreach (MLoader* this, Factory_each callback, void* usdata, const FactoryEachMode mode);

MLoader* get_factory(void);

/* -- module loader function table  -- */

static MLoaderFuncs MLoader_vft = {
    ._base = {
        .open 		    = (bool    (*) (Factory*, const char*))                                 &MLoader_open,
        .load 		    = (bool    (*) (Factory*,const char*))                                  &MLoader_load,
        .unload		    = (bool    (*) (Factory*,const char*))                                  &MLoader_unload,
        .instance	    = (Object* (*) (Factory*,const char*))                                  &MLoader_instance,
        .instanceById   = (Object* (*) (Factory*,const ObjectID))                               &MLoader_instanceById,
        .close		    = (void    (*) (Factory*))                                              &MLoader_close,
        .loadByPath	    = (bool    (*) (Factory*, const char*, const char*))                    &MLoader_loadByPath,
        .hasInstance    = (Object* (*) (Factory*, const char*))                                 &MLoader_hasInstance,
        .hasLoader      = (ObjectLoader* (*) (Factory*, const char*))                           &MLoader_hasLoader,
        .newInstance    = (Object*  (*) (Factory*, const char*))                                &MLoader_newInstance,
        .foreach        = (int (*)(Factory* this, Factory_each, void*,const FactoryEachMode))   &MLoader_foreach
    }
};

static MLoaderMetaFuncs MLoaderMeta_vft = {};

OBJECT_REGISTER(MLoader,OBJECT_FACTORY,MLoader_vft,MLoaderMeta_vft,Factory)

static MLoader* _factory = NULL;

MLoader* get_factory(void){
    return (!_factory) ? (_factory = static_cast(MLoader*,MLoaderLD.new(&MLoaderLD))) : _factory;
}

/* -- Module Factory -- */

static bool MLoader_open (MLoader* this, const char* dir) {
	struct stat stinfo;
	DIR* dptr = NULL;
    struct dirent *entry = NULL;
	char path[4096];
    char plugin[4096];
    char* ptr = path;
    
	if(!this) return false;
    
	LIST_INIT(&this->_data._loaders);
	if(stat(dir,&stinfo) != 0) return false; 
	if(!S_ISDIR(stinfo.st_mode)) return false;
	strncpy(this->_data._path,dir,strlen(dir) > 4096 ? 4096 : strlen(dir) );
    
    if(!(dptr = opendir(dir))) return false;

    while(dptr) {
        if(!(entry = readdir(dptr))) goto EXIT;
        if(!strncmp(entry->d_name,".",strlen(".")) || !strncmp(entry->d_name,"..",strlen(".."))) continue;
   		
        strncpy(ptr,this->_data._path,strlen(this->_data._path));
        ptr += strlen(this->_data._path);
        *ptr = '/'; 
        ++ptr;
        strncpy(ptr,entry->d_name,strlen(entry->d_name));
        ptr+=strlen(entry->d_name);
        *ptr = 0;
		strncpy(plugin,entry->d_name,strlen(entry->d_name)-3);
        plugin[strlen(entry->d_name)-3] = 0;
        
        ptr = path;
        
        printf("Load: %s %s\r\n",entry->d_name,(call(Factory,this,loadByPath,path,plugin)) ? "true" : "false");
    }    
    
EXIT:   
    closedir(dptr);
    return true;
}

static void MLoader_close (MLoader* this) {
    //LIST_FOREACH(TYPE *var, LIST_HEAD *head, entries);
}

static bool MLoader_load (MLoader* this, const char* name) {

	void* handle;
	char path[4096];
	char* ptr = path;

	if(!this) return false;
	if(!strlen(this->_data._path)) return false;

	strncpy(ptr,this->_data._path,strlen(this->_data._path));
	ptr += strlen(this->_data._path);
	
	strncpy(ptr,name,strlen(name));
	ptr += strlen(name);

	strncpy(ptr,".so",strlen(".so"));
	ptr += strlen(".so");

	return call(Factory,this,loadByPath,path,name);
}

static bool MLoader_loadByPath(MLoader* this,const char* path, const char* name) {
    void* handle = 0;
    void* ld = 0;
    struct _ModEntry *elem;
    char buffer[4096];
    
    strncpy(buffer,name,strlen(name));
    strncpy(buffer+strlen(name),"LD",strlen("LD"));
    buffer[strlen(name)+strlen("LD")] = 0;
    
	if(!this) return false;
    if(!(handle = dlopen(path,RTLD_LAZY | RTLD_GLOBAL))) goto FAIL;
    if(!(ld = dlsym(handle, buffer))) goto ERROR;
    if(!(elem = malloc(sizeof(ModEntry)))) goto ERROR;
    
    elem->ld = ld;
    LIST_INSERT_HEAD(&this->_data._loaders, elem, entries);
    
    return true;
    
ERROR:
    dlclose(handle);
FAIL:
    printf("Load: error opening shared library: %s %s\r\n",path,dlerror());
    return false;
}

static bool MLoader_unload (MLoader* this,const char* name) {
	if(!this) return false;
	return false;
}

static Object* MLoader_newInstance (MLoader* this, const char* name) {
    Object* ret = NULL;
    ModEntry* entry = NULL;
    ModInstance* elem = NULL;
	if(!this) return NULL;
    
    LIST_FOREACH(entry, &this->_data._loaders, entries){
        if(!strncmp(entry->ld->_funcs->name(entry->ld),name, strlen(name))) {
            ret = static_cast(Object*,entry->ld->new(entry->ld));
            call(Object,ret,setFactory,static_cast(Factory*,this));
            break;
        }
    }
    
    if(!ret) return NULL;
    if(!(elem = malloc(sizeof(ModInstance)))) return NULL;
    
    elem->mod = ret;
    LIST_INSERT_HEAD(&this->_data._instances, elem, entries);
    return ret;
}

static Object* MLoader_hasInstance (MLoader* this, const char* name) {
    ModInstance* elem = NULL;
    if(!this) return NULL;
    
    LIST_FOREACH(elem, &this->_data._instances, entries)
        if(!strncmp(call(Object,elem->mod,name),name,strlen(name))) return elem->mod;
    
    return NULL;
}

static ObjectLoader* MLoader_hasLoader (MLoader* this, const char* name) {
    ModEntry* entry = NULL;
    if(!this) return NULL;
    
    LIST_FOREACH(entry, &this->_data._loaders, entries)
        if(strncmp(entry->ld->_funcs->name(entry->ld),name,strlen(name))) return entry->ld;
    
    return NULL;
}

static Object* MLoader_instance (MLoader* this, const char* name) {
    Object* ret = NULL;
    
    if(!this) return NULL;
    if(!(ret = call(Factory,this,hasInstance,name))) return ret;
    return call(Factory,this,newInstance,name);
}

static Object* MLoader_instanceById (MLoader* this, const ObjectID id) {
    ModInstance* elem = NULL;
    if(!this) return NULL;
    
    LIST_FOREACH(elem, &this->_data._instances, entries)
        if(call(Object,elem->mod,id) == id) return elem->mod;
    
    return NULL;
}

static int MLoader_foreach (MLoader* this, Factory_each callback, void* usdata, const FactoryEachMode mode){
    ModInstance* elem = NULL;
    ModEntry* entry = NULL;
    int ret = 0;
    
    if(!this || ! callback) return 0;
    
    if(mode == FMODE_PLUGIN)
        LIST_FOREACH(elem, &this->_data._instances, entries) {    
            callback(elem->mod,usdata);
            ++ret;
        }
    
    if(mode == FMODE_LOADER)
        LIST_FOREACH(entry, &this->_data._loaders, entries) {    
            callback(entry->ld,usdata);
            ++ret;
        }
        
    return ret;
}
