#include <hardware.h>

static HardwareFuncs Hardware_vft = {};

static HardwareMetaFuncs HardwareMeta_vft = {
    .isr        = META_FUNC(Hardware,isr,VAR_VOID,FUNC_FIXED),
    .chipID     = META_FUNC(Hardware,chipID,VAR_INTEGER,FUNC_FIXED)
};

OBJECT_REGISTER(Hardware,OBJECT_HARDWARE,Hardware_vft,HardwareMeta_vft,Object)
