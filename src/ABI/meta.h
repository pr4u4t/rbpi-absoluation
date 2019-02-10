#ifndef _META_H_
#define _META_H_

#include <variant.h>

typedef enum _ObjectType ObjectType;
typedef enum _MetaFuncType MetaFuncType;


enum _MetaFuncType {
    FUNC_FIXED      = 0,
    FUNC_VARIADIC   = 1
};

enum _ObjectType {
    OBJECT_INVALID  = 0,
    OBJECT          = (1 << 0),
    OBJECT_FACTORY  = (1 << 1),
    OBJECT_PLUGIN   = (1 << 2), 
    OBJECT_DRIVER   = (1 << 3),
    OBJECT_HARDWARE = (1 << 4),
    OBJECT_ADC 	    = (1 << 5),
    OBJECT_DAC      = (1 << 6),
    OBJECT_TERMINAL = (1 << 7),
    OBJECT_SHELL    = (1 << 8)
};

typedef struct _Meta Meta;
struct _Meta {
    struct _Funcs*          _funcs;
	struct _MetaFuncs*	    _mfuncs;
	struct _MetaProperties* _mprop;
	size_t                  _size;
	size_t                  _fsize;
	size_t                  _msize;
	size_t                  _mpsize;
	const char*             _name;
};

typedef struct _MetaFunc MetaFunc;
struct _MetaFunc {
    const char*         _name;
    const void**        _arguments;
    VariantType         _ret;
    size_t              _offset;
    MetaFuncType        _type;
};

typedef struct _MetaProperty MetaProp;
struct _MetaProperty {
	const char*	_name;
	int 		_type;
	size_t 		_offset;
    size_t      _size;
};


#define META(Name,funcs,mtfuncs,mtprop)					                \
const Meta Name ## Meta  =   {  					                    \
	._funcs		= static_cast(struct _Funcs*,&funcs),		            \
        ._mfuncs	= static_cast(struct _MetaFuncs*,&mtfuncs),	        \
        ._mprop		= static_cast(struct _MetaProperties*,&mtprop),	    \
        ._size		= sizeof(Name),					                    \
        ._fsize		= sizeof(funcs),				                    \
        ._msize		= sizeof(mtfuncs),				                    \
        ._mpsize	= sizeof(mtprop),				                    \
	._name		= stringify(Name)				                        \
};

#define META_FUNC(module,fname,fret,farg,...)                       			                             \
{   ._name      = #fname,                                           			                             \
    ._ret       = ENUM_VAL(VariantType,fret),                           			                         \
    ._offset    = offsetof(module ## Funcs ,fname),                 			                             \
    ._arguments = (const void*[]) { IFF(PP_NARG(__VA_ARGS__))(PRINT_ARGUMENTS(__VA_ARGS__)) 0 },  			 \
    ._type      = farg                                              			                             \
}

#define META_PROP(Name,ptype,pname)				             \
{								                             \
	._name	 = #pname,					                     \
 	._type	 = ENUM_VAL(VariantType,ptype),			         \
	._offset = offsetof(Name ## Properties,_ ## pname),	     \
	._size   = sizeof(ptype)                                 \
}


#define METHOD(ret,name,...)            	    (ret,name,__VA_ARGS__)
#define METHODS(...) 				            (__VA_ARGS__)

#define DATA(ret,name,...)                      (ret,name,__VA_ARGS__)
#define DATAS(...)                              (__VA_ARGS__)

#define META_PROPERTY(type,name)		        (type,name)
#define META_PROPERTIES(...)			        (__VA_ARGS__)

#define METHOD_FORMAT(Name,ret,name,...)	    ret (*name)(Name* __VA_OPT__(,) __VA_ARGS__);
#define PRINT_METHOD(Name,...)			        IF_ELSE(PP_NARG(__VA_ARGS__))(METHOD_FORMAT ENCAPSULE(Name,DECAPSULE __VA_ARGS__))()
#define PRINT_METHODS(Name,...)			        MAP(PRINT_METHOD,Name,__VA_ARGS__)

#define METAMETHOD_FORMAT(Name,ret,name,...) 	MetaFunc name;
#define PRINT_METAMETHOD(Name,...)           	IF_ELSE(PP_NARG(__VA_ARGS__))(METAMETHOD_FORMAT ENCAPSULE(Name,DECAPSULE __VA_ARGS__))()
#define PRINT_METAMETHODS(Name,...)		        MAP(PRINT_METAMETHOD,Name,__VA_ARGS__)

#define METAPROPERTY_FORMAT(Name,type,name,...) MetaProp name;
#define PRINT_METAPROPERTY(Name,...)       	    IF_ELSE(PP_NARG(__VA_ARGS__))(METAPROPERTY_FORMAT ENCAPSULE(Name,DECAPSULE __VA_ARGS__))()
#define PRINT_METAPROPERTIES(Name,...)       	MAP(PRINT_METAPROPERTY,Name,__VA_ARGS__)

#define PROPERTY_FORMAT(Name,type,name,...)	    type _ ## name;
#define PRINT_PROPERTY(Name,...)		        IF_ELSE(PP_NARG(__VA_ARGS__))(PROPERTY_FORMAT ENCAPSULE(Name,DECAPSULE __VA_ARGS__))()
#define PRINT_PROPERTIES(Name,...)		        MAP(PRINT_PROPERTY,Name,__VA_ARGS__)

#define FORMAT_VFT(Name,ret,name,...)		    .name = &  Name ## _ ## name,
#define PRINT_VFT(Name,...)			            IF_ELSE(PP_NARG(__VA_ARGS__))(FORMAT_VFT ENCAPSULE(Name,DECAPSULE __VA_ARGS__))()
#define PRINT_VFTS(Name,...)			        MAP(PRINT_VFT,Name,__VA_ARGS__)

#define FORMAT_MVFT(Name,ret,name,...)          .name = META_FUNC(Name,name,ret,0,__VA_ARGS__),
#define PRINT_MVFT(Name,...)                    IF_ELSE(PP_NARG(__VA_ARGS__))(FORMAT_MVFT ENCAPSULE(Name,DECAPSULE __VA_ARGS__))()
#define PRINT_MVFTS(Name,...)                   MAP(PRINT_MVFT,Name,__VA_ARGS__)

#define FORMAT_MPT(Name,type,name,...)          .name = META_PROP(Name,type,name),
#define PRINT_MPT(Name,...)                   	IF_ELSE(PP_NARG(__VA_ARGS__))(FORMAT_MPT ENCAPSULE(Name,DECAPSULE __VA_ARGS__))()
#define PRINT_MPTS(Name,...)                    MAP(PRINT_MPT,Name,__VA_ARGS__)

#define FORMAT_DATA(Name,type,name,...)         type name;
#define PRINT_DATA(Name,...)                   	IF_ELSE(PP_NARG(__VA_ARGS__))(FORMAT_DATA ENCAPSULE(Name,DECAPSULE __VA_ARGS__))()
#define PRINT_DATAS(Name,...)                   MAP(PRINT_DATA,Name,__VA_ARGS__)

#define FORMAT_PROTO(Name,ret,name,...)		    static ret Name ## _ ## name (Name* __VA_OPT__(,) __VA_ARGS__);
#define PRINT_PROTO(Name,...)			        IF_ELSE(PP_NARG(__VA_ARGS__))(FORMAT_PROTO ENCAPSULE(Name,DECAPSULE __VA_ARGS__))()
#define PRINT_PROTOS(Name,...)			        MAP(PRINT_PROTO,Name,__VA_ARGS__)


#ifndef OBJECT
#define OBJECT(Name,base,methods,properts,datas)							\
	/* -- header section -- */									            \
	typedef struct _ ## Name ## Data Name ## Data;							\
	struct _ ## Name ## Data {  									        \
		/*IF_ELSE(base)(CAT(base,Data))()*/							        \
		EXPAND(PRINT_DATAS ENCAPSULE ( Name, DECAPSULE datas ))			    \
	};												                        \
	typedef struct _ ## Name ## Properties Name ## Properties;				\
	struct _ ## Name ## Properties {								        \
		/*IF_ELSE(base)(CAT(base,Properties))()*/						    \
		EXPAND(PRINT_PROPERTIES ENCAPSULE ( Name, DECAPSULE properts ))		\
	};												                        \
	typedef struct _ ## Name Name;									        \
	struct _ ## Name {										                \
		IF_ELSE(base)(base _base)(const Meta* _meta);						\
		Name ## Properties	_properties;							        \
		Name ## Data		_data;								            \
	};												                        \
	typedef struct _ ## Name ## Funcs 		Name ## Funcs;					\
	struct _ ## Name ## Funcs { 									        \
		IF_ELSE(base)(CAT(base,Funcs) _base;)()						        \
		EXPAND(PRINT_METHODS ENCAPSULE ( Name, DECAPSULE methods ))			\
	};												                        \
	typedef struct _ ## Name ## MetaFuncs 		Name ## MetaFuncs;			\
	struct _ ## Name ## MetaFuncs { 								        \
		IF_ELSE(base)(CAT(base,MetaFuncs) _base;)()						    \
		EXPAND(PRINT_METAMETHODS ENCAPSULE ( Name, DECAPSULE methods ))     \
	};												                        \
	typedef struct _ ## Name ## MetaProperties 	Name ## MetaProperties;		\
	struct _ ## Name ## MetaProperties { 								    \
		IF_ELSE(base)(CAT(base,Properties) _base;)()					    \
		EXPAND(PRINT_METAPROPERTIES ENCAPSULE ( Name, DECAPSULE properts ))	\
	};												                        \
                                                                            \
	/* -- code section --*/										            \
                                                                            \
	EXPAND(PRINT_PROTOS ENCAPSULE ( Name, DECAPSULE methods ))				\
                                                                            \
	Name ## Funcs 		Name ## _vft 		= { 						    \
		EXPAND(PRINT_VFTS ENCAPSULE ( Name, DECAPSULE methods )) 			\
	};												                        \
	Name ## MetaFuncs 	Name ## Meta_vft	= { 						    \
		EXPAND(PRINT_MVFTS ENCAPSULE ( Name, DECAPSULE methods )) 			\
	};												                        \
	Name ## MetaProperties	Name ## Meta_pt		= {						    \
		EXPAND(PRINT_MPTS ENCAPSULE ( Name, DECAPSULE properts ))			\
	};												                        \
                                                                            \
	META(Name,CAT(Name,_vft),CAT(Name,Meta_vft),CAT(Name,Meta_pt))
#endif
    
#define callIterator(type,object,method,...) ((typeof(((cat(type,Funcs)*)0)->method)) *(**((void****)   &object) + offsetof(cat(type,Funcs),method) / sizeof(void*)))(static_cast(type*,&object) __VA_OPT__(,) __VA_ARGS__) 
#define callObject(type,object,method,...  ) ((typeof(((cat(type,Funcs)*)0)->method)) *(***((void*****) &object) + offsetof(cat(type,Funcs),method) / sizeof(void*)))(*static_cast(type**,&object) __VA_OPT__(,) __VA_ARGS__)

/** -- ITER -- **/
typedef struct _Iterator Iterator;
struct _Iterator {

};    
    
#ifndef call
#define call(type,object,method,...)                      		    \
({									                                \
    __auto_type _o = object;						                \
    _Generic((object),                                    		    \
        Iterator: callIterator(type,_o,method,__VA_ARGS__),     	\
        default:  callObject(type,_o,method,__VA_ARGS__)        	\
    );                                                              \
}) 
#endif  

#ifndef set
#define set(type,object,prop,value) 	static_cast(type ## Properties*,&static_cast(type*,object)->_properties)->_ ## prop = value
#endif

#ifndef get
#define get(type,object,prop) 		static_cast(type ## Properties*,&static_cast(type*,object)->_properties)->_ ## prop  
#endif

#define new(O,...) ({ __auto_type _o = static_cast(O*,malloc(sizeof(O))); memset(_o,0,sizeof(O)); *static_cast(const void**,_o) = & O ## Meta; call(O,_o,construct,__VA_ARGS__); _o; })
#define delete(o) static_cast(void (*)(typeof(o)),***static_cast(void****,o))(o); free(o); 


#define meta_getp(object,prop)                                                                                                              \
    ({                                                                                                                                      \
        MetaProp* f = 0;                                                                                                                    \
        for(MetaProp *ptr = static_cast(MetaProp*,p->_meta->_mprop); ptr != static_cast(pointer,p->_meta->_mprop)+p->_meta->_mpsize; ++ptr) \
            if(!strcmp(ptr->_name,prop)) { f = ptr; }                                                                                       \
                                                                                                                                            \
        (f) ? (&object->_properties)+f->_offset : 0;                                                                                        \
    })
    
#define meta_set(object,prop,value)                                                                                                         \
     ({                                                                                                                                     \
                                                                                                                                            \
        for(MetaProp *ptr = static_cast(MetaProp*,p->_meta->_mprop); ptr != static_cast(pointer,p->_meta->_mprop)+p->_meta->_mpsize; ++ptr) \
            if(!strcmp(ptr->_name,prop)) { memcpy((&object->_properties)+ptr->_offset,value,ptr->_size); }                                  \
    })
     
#define meta_call(object,func,...)


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

#endif
