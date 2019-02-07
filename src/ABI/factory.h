#ifndef _FACTORY_H_
#define _FACTORY_H_

#include <metadata.h>

/*typedef enum _FactoryEachMode FactoryEachMode;*/

/*typedef struct _Factory Factory;*/
/*typedef struct _FactoryFuncs FactoryFuncs;*/
/*typedef struct _FactoryMetaFuncs FactoryMetaFuncs;*/

/*typedef void (*Factory_each) (void* plug, void* usdata);*/

enum _FactoryEachMode {
    FMODE_INVALID = 0,
    FMODE_PLUGIN  = 1,
    FMODE_LOADER  = 2
};

/*struct _FactoryFuncs {*/
    /*ObjectFuncs _base;*/
    /*bool (*open) (Factory* this, const char* dir);*/
	/*bool (*load) (Factory* this, const char* name);*/
	/*bool (*unload) (Factory* this, const char* name);*/
	/*Object* (*instance) (Factory* this, const char* name);*/
    /*Object* (*instanceById) (Factory* this, const ObjectID id);*/
    /*(Object* (*newInstance) (Factory* this, const char* name);*/
	/*void (*close) (Factory* this);*/
	/*bool (*loadByPath) (Factory* this, const char* path, const char *name);*/
    /*Object* (*hasInstance) (Factory* this, const char* name);*/
    /*ObjectLoader* (*hasLoader) (Factory* this, const char* name);*/
    /*int (*foreach) (Factory* this, Factory_each callback, void* usdata,const FactoryEachMode mode); */
/*};*/

/*struct _FactoryMetaFuncs {*/
    /*ObjectMetaFuncs _base;*/
    /*ObjectMetaFunc open;*/
    /*ObjectMetaFunc load;*/
    /*ObjectMetaFunc unload;*/
    /*ObjectMetaFunc instance;*/
    /*ObjectMetaFunc instanceById;*/
    /*ObjectMetaFunc newInstance;*/
    /*ObjectMetaFunc close;*/
    /*ObjectMetaFunc loadByPath;*/
    /*ObjectMetaFunc hasInstance;*/
    /*ObjectMetaFunc hasLoader;*/
    /*ObjectMetaFunc foreach;*/
/*};*/

/*struct _Factory {*/
    /*//MetaData* _meta;*/
    /*Object      _base;*/
/*};*/

OBJECT(Factory,Object,
       METHODS
       (
           METHOD(bool,          open,           const char*),
           METHOD(bool,          load,           const char*),
           METHOD(bool,          unload          const char*),
           METHOD(Object*,       instance        const char*),
           METHOD(Object*,       instanceById    const),
           METHOD(Object*,       newInstance     const char*),
           METHOD(void,          close),
           METHOD(bool,          loadByPath,     const char*     const char*),
           METHOD(Object*,       hasInstance     const char*),
           METHOD(ObjectLoader*, hasLoader       const char*),
           METHOD(int            foreach         Factory_each,   void*,         const FactoryEachMode)
        )
)
OBJECT_EXPORT(Factory)

#endif
