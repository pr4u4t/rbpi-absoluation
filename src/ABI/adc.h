#ifndef _ADC_H_
#define _ADC_H_

#include <hardware.h>
    
OBJECT(ADC,Object,
    METHODS
    (
        METHOD(bool,     open,       int,    int,    int ),
        METHOD(void,     close                           ),
        METHOD(int,      mode                            ),
        METHOD(void,     setMode,    int                 ),
        METHOD(float,    voltage,    int                 ),
        METHOD(uint8_t,  channels                        )
    ),
    PROPERTIES()
)

//OBJECT_EXPORT(ADC)

#endif
