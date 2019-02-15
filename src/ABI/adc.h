#ifndef _ADC_H_
#define _ADC_H_

#include <hardware.h>
    
OBJECT(ADC,Hardware,
    METHODS
    (
        VMETHOD(bool,     open,       int,    int,    int ),
        VMETHOD(void,     close                           ),
        VMETHOD(int,      mode                            ),
        VMETHOD(void,     setMode,    int                 ),
        VMETHOD(float,    voltage,    int                 ),
        VMETHOD(uint8_t,  channels                        )
    ),
    PROPERTIES()
)

//OBJECT_EXPORT(ADC)

#endif
