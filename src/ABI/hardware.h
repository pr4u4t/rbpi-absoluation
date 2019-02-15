#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include <object.h>

OBJECT(Hardware,Object,
    METHODS
    (
        VMETHOD(void, isr    ),
        VMETHOD(int,  chipID )
    ),
    PROPERTIES(
    )
)

#endif
