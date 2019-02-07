#ifndef _PRIMITIVES_H_
#define _PRIMITIVES_H_

#include <pp_nightmare.h>

#define FORMAT_NUM(udata,name,value)	udata ## _ ## name = value,
#define PRINT_NUM(udata,...)		FORMAT_NUM ENCAPSULE( udata, DECAPSULE __VA_ARGS__ )	
#define NUM(name,value) 		(name,value)

#define PRINT_ARGUMENT(udata,arg)	static_cast(const void*,ENUM_VAL(VariantType,arg)),			
#define PRINT_ARGUMENTS(...)		XMAP(PRINT_ARGUMENT,0,__VA_ARGS__)

#define FORMAT_NARRAY(udata,name,value)	{ (const char*) value, stringify(udata ## _ ## name)  },
#define PRINT_NARRAY(udata,...)		FORMAT_NARRAY ENCAPSULE(udata, DECAPSULE __VA_ARGS__ )

#define ENUM_VAL(num,name)		num ## _ ##name 
#define ENUM_STRING(num,value)		
#define ENUM_STRING_VAL(num,str)

#define SMART_ENUM(name,...)                                                                \
typedef enum _ ## name name;                                                                \
enum _ ## name {                                                                            \
	MAP(PRINT_NUM,name,__VA_ARGS__)                                                         \
};                                                                                          \
const char* name ## _ ## strings[][2] = {                  					                \
        MAP(PRINT_NARRAY,name,__VA_ARGS__)             						                \
        0                                               					                \
};												                                            \
const char* name ## _to_string(name value){							                        \
	for(const char** p = static_cast(const char**,name ## _ ## strings); *p != 0; ++p){	    \
		if(*p == static_cast(char*,value)) return *p;					                    \
	}											                                            \
	return 0;										                                        \
}												                                            \
                                                                                            \
name string_to_ ## name(const char* ename){							                        \
	for(const char** p = static_cast(const char**,name ## _ ## strings); *p != 0; ++p){	    \
		if(*p == ename) return static_cast(name,*++p);					                    \
	}											                                            \
	return -1;										                                        \
}

#endif
