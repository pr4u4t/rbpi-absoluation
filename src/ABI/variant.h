#ifndef _VARIANT_H_
#define _VARIANT_H_

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include <primitives.h>

typedef void* pointer;
typedef char* string;
typedef union _VariantData VariantData;
typedef struct _Variant Variant;


SMART_ENUM(VariantType,
	NUM(void,		0),
	NUM(int,		1),
	NUM(long,		2),
	NUM(char,		3),
	NUM(float,		4),
	NUM(double,		5),
	NUM(int8_t,		6),
	NUM(int16_t,	7),
	NUM(int32_t,	8),
	NUM(int64_t,	9),
	NUM(uint8_t,	10),
	NUM(uint16_t,	11),
	NUM(uint32_t,	12),
	NUM(uint64_t,	13),
    NUM(pointer,    14),
    NUM(string,     15)
)

union _VariantData {
    int8_t    ci;
    int16_t   si;
    int32_t   i;
    int64_t   li;
    int8_t    uci;
    uint16_t  usi;
    uint32_t  ui;
    uint64_t  uli;
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
