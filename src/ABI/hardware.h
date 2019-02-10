#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include <object.h>

OBJECT(Hardware,Object,
    METHODS
    (
        METHOD(void, isr),
        METHOD(int,  chipID)
    ),
    META_PROPERTIES(),
    DATAS()
)

//OBJECT_EXPORT(Hardware)

#endif
