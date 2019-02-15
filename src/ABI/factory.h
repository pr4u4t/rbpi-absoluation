#ifndef _FACTORY_H_
#define _FACTORY_H_

#include <object.h>

typedef void (*Factory_each) (Object* object, pointer usdata);
typedef enum _FactoryEachMode FactoryEachMode;
enum _FactoryEachMode {
    FMODE_INVALID = 0,
    FMODE_PLUGIN  = 1,
    FMODE_LOADER  = 2
};

OBJECT(Factory,Object,
    METHODS(
        VMETHOD(bool,          open,           string                                   ),
        VMETHOD(bool,          load,           string                                   ),
        VMETHOD(bool,          unload,         string                                   ),
        VMETHOD(pointer,       instance,       string                                   ),
        VMETHOD(pointer,       instanceById,   int                                      ),
        VMETHOD(pointer,       newInstance,    string                                   ),
        VMETHOD(void,          close                                                    ),
        VMETHOD(bool,          loadByPath,     string,       string                     ),
        VMETHOD(pointer,       hasInstance,    string                                   ),
        VMETHOD(pointer,       hasLoader,      string                                   ),
        VMETHOD(int,           foreach,        funcptr, pointer,    int                 )
    ),
    PROPERTIES(
    )
)

//OBJECT_EXPORT(Factory)

#endif
