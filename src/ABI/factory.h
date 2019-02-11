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
        METHOD(bool,          open,           string                                   ),
        METHOD(bool,          load,           string                                   ),
        METHOD(bool,          unload,         string                                   ),
        METHOD(pointer,       instance,       string                                   ),
        METHOD(pointer,       instanceById,   int                                      ),
        METHOD(pointer,       newInstance,    string                                   ),
        METHOD(void,          close                                                    ),
        METHOD(bool,          loadByPath,     string,       string                     ),
        METHOD(pointer,       hasInstance,    string                                   ),
        METHOD(pointer,       hasLoader,      string                                   ),
        METHOD(int,           foreach,        funcptr, pointer,    int                 )
    ),
    PROPERTIES(
    )
)

//OBJECT_EXPORT(Factory)

#endif
