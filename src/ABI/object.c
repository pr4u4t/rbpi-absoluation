#ifndef _OBJECT_C_
#define _OBJECT_C_

#define _Object_source_
#include <object.h>

static string Object_name (Object* this){ return (this && this->_meta) ? this->_meta->_name : NULL; }

static int Object_type (Object* this){ return (this && this->_meta) ? this->_meta->_type : 0; }

static size_t Object_size (Object* this){ return (this && this->_meta) ? this->_meta->_size : 0; }

static bool Object_open (Object* this){ return (this) ? set(Object,this,isopen,true) : false; }

static void Object_close (Object* this){ if(this) set(Object,this,isopen,false); }

#endif
