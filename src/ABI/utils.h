#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifndef static_cast
#define static_cast(type,data) ((type) data)
#endif
    
#ifndef alloc
#define alloc(type) \
    malloc(sizeof(type))
#endif
    
#ifndef FIELD_SIZEOF
#define FIELD_SIZEOF(t, f) (sizeof(((t*)0)->f))
#endif

#define LAMBDA(c_) ({ c_ _;})    
 
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })    

#define MERGE_TABLES(src,dst)\
for(void** sptr = static_cast(void**,src),**dptr = static_cast(void**,dst); sptr != static_cast(void*,src)+sizeof(*dst);++sptr,++dptr){\
    if(*sptr != NULL && *dptr == NULL) *dptr = *sptr;\
}

char* strtoupper(char* buff);    

unsigned long mix(unsigned long a, unsigned long b, unsigned long c);

#endif
