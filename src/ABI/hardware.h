#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include <metadata.h>

/*typedef struct _Hardware Hardware;*/
/*typedef struct _HardwareFuncs HardwareFuncs;*/
/*typedef struct _HardwareMetaFuncs HardwareMetaFuncs;*/

/*struct _HardwareFuncs {*/
    /*ObjectFuncs _base;*/
    /*void        (*isr)              (Hardware* this);*/
    /*int 		(*chipID)           (Hardware* this);*/
/*};*/

/*struct _HardwareMetaFuncs {*/
    /*ObjectMetaFuncs _base;*/
    /*ObjectMetaFunc isr;*/
    /*ObjectMetaFunc chipID;*/
/*};*/

/*struct _Hardware {*/
    /*Object _base;*/
/*};*/

OBJECT(Hardware,Object,
       METHODS
       (
           METHOD(void, isr)
           METHOD(int,  chipID)
        )

OBJECT_EXPORT(Hardware)

#endif
