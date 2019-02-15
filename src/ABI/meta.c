#include <meta.h>
#include <object.h>
#include <terminal.h>
#include <stdio.h>

void meta_info(Object* object,Terminal *term){
    if(!object) return;
    
    (term) ? call(Terminal,term,writef,"Module:    %#x\r\n", object)                : printf("Module:    %#x\r\n", object);
    (term) ? call(Terminal,term,writef,"\t name:   %s\r\n",  object->_meta->_name)  : printf("\t name:   %s\r\n",  object->_meta->_name);
    (term) ? call(Terminal,term,writef,"\t type:   %d\r\n",  object->_meta->_type)  : printf("\t type:   %d\r\n",  object->_meta->_type);
    (term) ? call(Terminal,term,writef,"\t funcs:  %#x\r\n", object->_meta->_funcs) : printf("\t funcs:  %#x\r\n", object->_meta->_funcs);
    (term) ? call(Terminal,term,writef,"\t mfuncs: %#x\r\n", object->_meta->_mfuncs): printf("\t mfuncs: %#x\r\n", object->_meta->_mfuncs);
    (term) ? call(Terminal,term,writef,"\t size:   %dB\r\n", object->_meta->_size)  : printf("\t size:   %dB\r\n", object->_meta->_size);
    (term) ? call(Terminal,term,writef,"\t fsize:  %dB\r\n", object->_meta->_fsize) : printf("\t fsize:  %dB\r\n", object->_meta->_fsize);
    (term) ? call(Terminal,term,writef,"\t mfsize: %dB\r\n", object->_meta->_msize) : printf("\t mfsize: %dB\r\n", object->_meta->_msize);
    
    (term) ? call(Terminal,term,writef,"Meta Functions: %d\r\n", object->_meta->_msize / sizeof(MetaFunc)) 
        : printf("Meta Functions: %d\r\n", object->_meta->_msize / sizeof(MetaFunc));
    
    for(const MetaFunc *i = static_cast(MetaFunc*,object->_meta->_mfuncs);
        i < static_cast(MetaFunc*,object->_meta->_mfuncs)+(object->_meta->_msize / sizeof(MetaFunc)); ++i){
        (term) ? call(Terminal,term,writef,"\tfunction: %s %s ",VariantTypeName[i->_ret],i->_name) : printf("\tfunction: %s %s ",VariantTypeName[i->_ret],i->_name);
        if(i->_arguments && *i->_arguments){
            char *sptr = 0;
            (term) ? call(Terminal,term,write,"(",strlen("(")) : printf("(");
            for(void** args = (void**)i->_arguments; *args != 0; ++args){
                (term) ? call(Terminal,term,writef,"%s",VariantTypeName[static_cast(long int,*args)]) : printf("%s",VariantTypeName[static_cast(long int,*args)]);
                if(*(args+1) != 0) (term) ? call(Terminal,term,write,",",strlen(",")) : printf(",");
            }   
            if(i->_type == FUNC_VARIADIC)
                (term) ? call(Terminal,term,write,",...",strlen(",...")) : printf(",...");
    
            (term) ? call(Terminal,term,write,")\r\n",strlen(")\r\n")) : printf(")\r\n");
        } else (term) ? call(Terminal,term,writef,"(void)\r\n") : printf("(void)\r\n");
    }   
}

MetaFunc* metafunc_info(Object* object,const char* name){
    if(!object || !name) return NULL;
    for(ObjectMetaFunc *i = static_cast(ObjectMetaFunc*,object->_meta->_meta);
        i < static_cast(MetaFunc*,object->_meta->_meta)+(object->_meta->_msize / sizeof(MetaFunc)); ++i){
        if(strcmp(i->_name,name) == 0) return i;
    }   
    
    return NULL;
}

MetaFunc* metafunc_find(Object* object,const char* name,const void** signature){
    if(!object || !name) return NULL;
    for(ObjectMetaFunc *i = static_cast(MetaFunc*,object->_meta->_meta);
        i < static_cast(MetaFunc*,object->_meta->_mfuncs)+(object->_meta->_msize / sizeof(MetaFunc)); ++i){
        if(!i->_name) continue;
        if(strcmp(i->_name,name) == 0){ 
            if((!i->_arguments && !signature) || (!*i->_arguments && !signature)) return i;
            if(i->_arguments && !signature) continue;
            const void** arg = i->_arguments,**sig = signature;
            for(arg = i->_arguments,sig = signature; *arg == *sig && *arg && *sig;++sig,++arg);
            if(!*arg && !*sig) return i;
        }   
    }   
    
    return NULL;
}

