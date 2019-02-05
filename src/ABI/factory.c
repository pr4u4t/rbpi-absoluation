#include <factory.h>

static FactoryFuncs Factory_vft;

static FactoryMetaFuncs FactoryMeta_vft = {
    .open         = META_FUNC(Factory,open,VAR_BOOLEAN,FUNC_FIXED,MARG(VAR_STRING)),
    .load         = META_FUNC(Factory,load,VAR_BOOLEAN,FUNC_FIXED,MARG(VAR_STRING)),
    .unload       = META_FUNC(Factory,unload,VAR_BOOLEAN,FUNC_FIXED,MARG(VAR_STRING)),
    .instance     = META_FUNC(Factory,instance,VAR_PTR,FUNC_FIXED,MARG(VAR_STRING)),
    .instance     = META_FUNC(Factory,instanceById,VAR_PTR,FUNC_FIXED,MARG(VAR_INTEGER)),
    .newInstance  = META_FUNC(Factory,newInstance,VAR_PTR,FUNC_FIXED,MARG(VAR_STRING)),
    .close        = META_FUNC(Factory,close,VAR_VOID,FUNC_FIXED),
    .loadByPath   = META_FUNC(Factory,loadByPath,VAR_BOOLEAN,FUNC_FIXED,MARG(VAR_STRING),MARG(VAR_STRING)),
    .hasInstance  = META_FUNC(Factory,hasInstance,VAR_PTR,FUNC_FIXED,MARG(VAR_STRING)),
    .hasLoader    = META_FUNC(Factory,hasLoader,VAR_PTR,FUNC_FIXED,MARG(VAR_STRING)),
    .foreach      = META_FUNC(Factory,foreach,VAR_INTEGER,FUNC_FIXED,MARG(VAR_PTR),MARG(VAR_PTR))
};

OBJECT_REGISTER(Factory,OBJECT_FACTORY,Factory_vft,FactoryMeta_vft,Object)

