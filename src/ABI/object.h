#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <meta.h>

OBJECT(Object,0,
        METHODS
        (
            METHOD(string      ,name                ),
            METHOD(int         ,type                ),
            METHOD(size_t      ,size                ),
            METHOD(long        ,id                  ),
            METHOD(bool        ,open                ),
            METHOD(void        ,close               ),
            METHOD(bool        ,isOpen              ),
            METHOD(void        ,setFactory, pointer ),
            METHOD(pointer     ,factory             )
        ),  
        META_PROPERTIES
        (   
            META_PROPERTY(long,     id      ),
            META_PROPERTY(bool,     open    ),
            META_PROPERTY(pointer,  factory )
        ),  
        DATAS()   
)

#endif
