#include "adc.h"

static ADCFuncs ADC_vft = {};

static ADCMetaFuncs ADCMeta_vft = {
    .open       = META_FUNC(ADC,open,VAR_BOOLEAN,FUNC_FIXED,MARG(VAR_INTEGER),MARG(VAR_INTEGER),MARG(VAR_INTEGER)),
    .close      = META_FUNC(ADC,close,VAR_VOID,FUNC_FIXED),
    .mode       = META_FUNC(ADC,mode,VAR_INTEGER,FUNC_FIXED),
    .setMode    = META_FUNC(ADC,setMode,VAR_VOID,FUNC_FIXED,MARG(VAR_INTEGER)),
    .voltage    = META_FUNC(ADC,voltage,VAR_FLOAT,FUNC_FIXED,MARG(VAR_INTEGER)),
    .channels   = META_FUNC(ADC,channels,VAR_INTEGER,FUNC_FIXED)
};

OBJECT_REGISTER(ADC,OBJECT_ADC | OBJECT_HARDWARE | OBJECT,ADC_vft,ADCMeta_vft,Hardware)
