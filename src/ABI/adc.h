#ifndef _ADC_H_
#define _ADC_H_

#include <hardware.h>

typedef struct _ADC ADC;
typedef struct _ADCFuncs ADCFuncs;
typedef struct _ADCMetaFuncs ADCMetaFuncs;

struct _ADCFuncs {
    HardwareFuncs _base;
    bool        (*open)             (ADC* this, int gain, int drate, int mode);
    void        (*close)            (ADC* this);
    int     	(*mode)             (ADC* this);
    void        (*setMode)          (ADC* this, int mode);
    float       (*voltage)          (ADC* this, int chan);
    uint8_t     (*channels)         (ADC* this);

};

struct _ADCMetaFuncs {
    HardwareMetaFuncs _base;
    ObjectMetaFunc open;
    ObjectMetaFunc close;
    ObjectMetaFunc mode;
    ObjectMetaFunc setMode;
    ObjectMetaFunc voltage;
    ObjectMetaFunc channels;
};

struct _ADC {
    Hardware _base;
};

OBJECT_EXPORT(ADC)

#endif
