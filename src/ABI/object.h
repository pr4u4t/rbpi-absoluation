#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <meta.h>

OBJECT(Object,0,
    METHODS(
        METHOD(string      ,name                ),
        METHOD(int         ,type                ),
        METHOD(size_t      ,size                ),
        METHOD(bool        ,open                ),
        METHOD(void        ,close               ),
    ),  
    PROPERTIES(   
        PROPERTY(long,     id      ),
        PROPERTY(bool,     isopen  ),
        PROPERTY(pointer,  factory )
    )  
)

#endif
