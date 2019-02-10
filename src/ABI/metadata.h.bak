#ifndef _METADATA_H_
#define _METADATA_H_

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <time.h>
#include <bool.h>
#include <utils.h>
#include <inttypes.h>
#include <variant.h>
#include <collections.h>

#define callIterator(type,object,method,...) ((typeof(((CAT(type,Funcs)*)0)->method)) *(**((void****)   &object) + offsetof(CAT(type,Funcs),method) / sizeof(void*)))(static_cast(type*,&object) __VA_OPT__(,) __VA_ARGS__) 
#define callObject(type,object,method,...  ) ((typeof(((CAT(type,Funcs)*)0)->method)) *(***((void*****) &object) + offsetof(CAT(type,Funcs),method) / sizeof(void*)))((static_cast(type*,object)) __VA_OPT__(,) __VA_ARGS__)

#ifndef call
#define call(type,object,method,...)                                    \
    ({ __auto_type _o = object;                                         \
    _Generic((object),                                                  \
        Iterator: callIterator(type,_o,method,__VA_ARGS__),             \
        default:  callObject(type,_o,method,__VA_ARGS__)                \
    ); })                                                                                                                                         
#endif

typedef long int ObjectID;

typedef struct _MetaData MetaData;
typedef struct _ObjectMetaFunc ObjectMetaFunc;
typedef struct _ObjectMetaData ObjectMetaData;
typedef struct _ObjectFuncs ObjectFuncs;
typedef struct _ObjectMetaFuncs ObjectMetaFuncs;
typedef struct _ObjectData ObjectData;
typedef struct _Object Object;

typedef struct _ObjectLoader ObjectLoader;
typedef struct _ObjectLoaderFuncs ObjectLoaderFuncs;
typedef struct _ObjectLoaderData ObjectLoaderData;

typedef struct _Factory Factory;
typedef struct _Terminal Terminal;

typedef enum _ObjectType ObjectType;
typedef enum _MetaFuncType MetaFuncType;

enum _MetaFuncType {
    FUNC_FIXED      = 0,
    FUNC_VARIADIC   = 1
};

enum _ObjectType {
    OBJECT_INVALID  = 0,
	OBJECT         	= (1 << 0),
	OBJECT_FACTORY  = (1 << 1),
    OBJECT_PLUGIN   = (1 << 2), 
    OBJECT_DRIVER   = (1 << 3),
    OBJECT_HARDWARE = (1 << 4),
    OBJECT_ADC 	    = (1 << 5),
    OBJECT_DAC      = (1 << 6),
    OBJECT_TERMINAL = (1 << 7),
    OBJECT_SHELL    = (1 << 8)
};

struct _ObjectMetaFunc {
    const char*         _name;
    const void**        _arguments;
    VariantType         _ret;
    size_t              _offset;
    MetaFuncType        _type;
};

struct _ObjectFuncs {
    const char* (*name)     (Object* this);
    ObjectType  (*type)     (Object* this);
    size_t      (*size)     (Object* this);
    ObjectID    (*id)       (Object* this);
    bool        (*open)     (Object* this);
    void        (*close)    (Object* this);
    bool        (*isOpen)   (Object* this);
    void        (*setFactory) (Object* this, Factory* factory);
    Factory*    (*factory)  (Object* this);
};

struct _ObjectMetaFuncs {
    ObjectMetaFunc name;
    ObjectMetaFunc type;
    ObjectMetaFunc size;
    ObjectMetaFunc id;
    ObjectMetaFunc open;
    ObjectMetaFunc close;
    ObjectMetaFunc isOpen;
    ObjectMetaFunc setFactory;
    ObjectMetaFunc factory;
};

struct _MetaData {
    const void*             _funcs;
    const void*             _prop;
    const ObjectMetaFuncs*  _meta;
    const size_t            _size;
    const size_t            _fsize;
    const size_t            _msize;
    const char*             _name;
    const ObjectType        _type;
    const MetaData*         _base;
};

struct _ObjectLoaderFuncs {
    const char* (*name) (const ObjectLoader *this);
    ObjectType  (*type) (const ObjectLoader* this);
    size_t      (*size) (const ObjectLoader* this);
};

struct _ObjectLoader {
    const MetaData              _meta;
    const ObjectLoaderFuncs*    _funcs;
    Object* (*new) (const ObjectLoader* this);
};

struct _ObjectData {
    ObjectID    _id;
    bool        _open;
    Factory*    _factory;
};

struct _Object {
    const MetaData* _meta;
    ObjectData      _data;
};

/* -- FUNCTIONS -- */

const char* const type_to_string(ObjectType type);

bool metacall(Object* object, const char *method, const Variant* args);

void meta_info(Object* object, Terminal *term);

ObjectMetaFunc* metafunc_info(Object* object,const char* name);

ObjectMetaFunc* metafunc_find(Object* object,const char* name,const void** signature);


#define OBJECT_ORDER(ord) (100+ord)
#define MARG(arg) (void*) arg
#define LOADER(ld) ld ## LD
#define OBJECT_EXPORT(name) extern const ObjectLoader name ## LD;

#define META_FUNC(module,fname,fret,farg,...)                       \
{   ._name      = #fname,                                           \
    ._ret       = fret,                                             \
    ._offset    = offsetof(module ## Funcs ,fname),                 \
    ._arguments = (const void*[]) { __VA_ARGS__ __VA_OPT__(,) 0 },  \
    ._type      = farg                                              \
}

#define OBJECT_LOADER_REGISTER(name,type,funcs,metaFuncs,base,...)                      \
static Object* name ## _new(const ObjectLoader* ld) {                                   \
    Object *ret = NULL;                                                                 \
    if(!(ret = malloc(ld->_meta._size))) return NULL;                                   \
    memset(ret,0,ld->_meta._size);                                                      \
    ret->_meta = &ld->_meta;                                                            \
    ret->_data._id = lrand48();                                                         \
    return ret;                                                                         \
}                                                                                       \
extern const ObjectLoaderFuncs ObjectLoader_vft;                                        \
const ObjectLoader  name ## LD = {                                                      \
    ._meta  = {                                                                         \
        ._name  = #name,                                                                \
        ._size  = sizeof(name) ,                                                        \
        ._funcs = static_cast(ObjectFuncs*,&funcs),                                     \
        ._type  = type,                                                                 \
        ._meta  = static_cast(ObjectMetaFuncs*,&metaFuncs),                             \
        ._fsize = sizeof(funcs),                                                        \
        ._msize = sizeof(metaFuncs),                                                    \
        ._base  = base                                                                  \
    },                                                                                  \
    .new    = &name ## _new,                                                            \
    ._funcs = &ObjectLoader_vft                                                         \
};                                                                                      \
static void name ## _init(void) __attribute__((constructor(OBJECT_ORDER(type))));       \
static void name ## _init(void) {                                                       \
    printf("Initializing: %s\r\n",#name);                                               \
    if(base){                                                                           \
        const MetaData *m = base;                                                       \
         for(void** sfuncs = static_cast(void**,m->_funcs),                             \
            **dfuncs = static_cast(void**,LOADER(name)._meta._funcs);                   \
            sfuncs != m->_funcs+m->_fsize; ++sfuncs,++dfuncs)                           \
            if(*sfuncs != NULL && *dfuncs == NULL) *dfuncs = *sfuncs;                   \
                                                                                        \
         for(void** sfuncs = static_cast(void**,m->_meta),                              \
            **dfuncs = static_cast(void**,LOADER(name)._meta._meta);                    \
            sfuncs != static_cast(void*,m->_meta)+m->_msize; ++sfuncs,++dfuncs)         \
            *dfuncs = *sfuncs;                                                          \
    }                                                                                   \
                                                                                        \
    __VA_ARGS__ __VA_OPT__(;)                                                           \
}

#define OBJECT_REGISTER(name,type,funcs,mfuncs,base,...)                                                                                                        \
IF_ELSE(base)( OBJECT_LOADER_REGISTER(name,type,funcs,mfuncs,&LOADER(base)._meta,__VA_ARGS__))(OBJECT_LOADER_REGISTER(name,type,funcs,mfuncs,0,__VA_ARGS__))    



OBJECT_EXPORT(Object)

#endif
