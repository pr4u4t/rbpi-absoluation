#ifndef _ABI_H_
#define _ABI_H_

#include <object.h>
#include <collections.h>

#define callIterator(type,object,method,...) ((typeof(((cat(type,Funcs)*)0)->method)) *(**((void****)   &object) + offsetof(cat(type,Funcs),method) / sizeof(void*)))(static_cast(type*,&object) __VA_OPT__(,) __VA_ARGS__) 
#define callObject(type,object,method,...  ) ((typeof(((cat(type,Funcs)*)0)->method)) *(***((void*****) &object) + offsetof(cat(type,Funcs),method) / sizeof(void*)))(*static_cast(type**,&object) __VA_OPT__(,) __VA_ARGS__)

#ifndef call
#define call(type,object,method,...)                      		    \
({									                                \
    __auto_type _o = object;						                \
    _Generic((object),                                    		    \
        Iterator: callIterator(type,_o,method,__VA_ARGS__),     	\
        default:  callObject(type,_o,method,__VA_ARGS__)        	\
    );                                                              \
}) 
#endif

#endif
