#ifndef _VARIANT_H_
#define _VARIANT_H_

#include <stdlib.h>
#include <inttypes.h>
#include <sys/queue.h>

typedef struct _Variant Variant;
typedef union _VariantData VariantData;
typedef enum _VariantType VariantType;

typedef struct _ModuleMetaData ModuleMetaData;

enum _VariantType {
    VAR_VOID    = 0,
    VAR_INTEGER = 1,
    VAR_FLOAT   = 2,
    VAR_CHAR    = 3,
    VAR_DOUBLE  = 4,
    VAR_PTR     = 5,
    VAR_STRING  = 6,
    VAR_BOOLEAN = 7,
    VAR_LIST    = 8,
    VAR_SET     = 9,
    VAR_ARRAY   = 10
};

static const char* VariantTypeName[] = { 
    "void" , 
    "integer", 
    "float", 
    "char", 
    "double", 
    "pointer", 
    "string", 
    "boolean",
    "list",
    "set",
    "array"
};

union _VariantData {
    int       i;
    float     f;
    char      c;
    double    d;
    void*     p;
    char*     s;
    long      l;
};

struct _Variant {
    VariantData _data;
    VariantType _type;
};

#endif
