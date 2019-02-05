#include <collections.h> 
#include <metadata.h>

/* -- Array -- */
typedef IteratorPriv ArrayIterator;
static Iterator Array_begin(Array* this);
static Iterator Array_end(Array* this);
static void*   Array_data(Array* this);
static void*   Array_at(Array* this, size_t idx);
static void    Array_set(Array* this, size_t idx, void* data);
static size_t  Array_size(Array* this);
static size_t  Array_maxSize(Array* this);
static void    Array_setMaxSize(Array* this, size_t maxSize);
static size_t  Array_elemSize(Array* this);
static void    Array_setElemSize(Array* this, size_t size);
/* -- Array Iterator -- */
static void Array_iterator_prev(Iterator* this);
static void Array_iterator_next(Iterator* this);
static void* Array_iterator_data(Iterator* this);
static void* Array_iterator_key(Iterator* this);

static const ArrayFuncs Array_vft = {
    ._base = {
        ._base = {
            .next   = Array_iterator_next,
            .prev   = Array_iterator_prev,
            .data   = Array_iterator_data, 
            .key    = Array_iterator_key    
        },
        .begin  = (Iterator (*)(void*)) Array_begin,
        .end    = (Iterator (*)(void*)) Array_end
    },
    .data           = Array_data,
    .at             = Array_at,
    .set            = Array_set,
    .size           = Array_size,
 //   .maxSize        = Array_maxSize,
 //   .setMaxSize     = Array_setMaxSize,
    .elemSize       = Array_elemSize,
    .setElemSize    = Array_setElemSize
};

static const MetaData ArrayMetaData = {
    ._funcs = &Array_vft,
    ._prop  = 0,
    ._meta  = 0,
    ._size  = sizeof(Array),
    ._fsize = sizeof(ArrayFuncs),
    ._msize = 0,
    ._name  = "Array",
    ._type  = 0,
    ._base  = 0
};

static void* Array_data (Array* this){
    return (this) ? this->_data._data : 0;
}

static void* Array_at(Array* this, size_t idx){
    if(!this) return 0;
    void** ptr = this->_data._data+idx*sizeof(void*);
    return (static_cast(size_t,idx) >= 0 && static_cast(int,idx) < this->_data._size) ? *ptr  : 0;
}

static void Array_set(Array* this, size_t idx, void* data){
    if(!this || !this->_data._data || idx >= this->_data._size) return;
    void** ptr = this->_data._data+sizeof(void*)*idx;
    *ptr = data;
}

static size_t Array_size(Array* this){
    return (this) ? this->_data._size : 0;
}

static size_t Array_elemSize(Array* this){
    if(!this || !this->_data._data) return 0;
}

static void Array_setElemSize(Array* this, size_t size){
    if(!this || !this->_data._data) return;
    this->_data._elemSize = size;
}

static Iterator Array_begin(Array* this){
    return (this) ? (Iterator) { ._base = this, ._state = 0} : (Iterator){0,0};
}

static Iterator Array_end(Array* this){
    //call(Array,this,size);
    return (this) ? (Iterator) { ._base = this, ._state = 0 } : (Iterator){ 0,0 };
}

static void Array_iterator_prev(Iterator* this){
    if(this->_state > 0) --this->_state;
}

static void Array_iterator_next(Iterator* this){
    ++this->_state; 
}

static void* Array_iterator_data(Iterator* this){
    return (this->_base) ? NULL : NULL;
}

static void* Array_iterator_key(Iterator* this){
    return (this->_base) ? NULL : NULL; 
}

/*
static size_t  Array_maxSize(Array* this){
}

static void Array_setMaxSize(Array* this, size_t maxSize){
}
*/

Array* Array_new(size_t elemSize,size_t size){
    Array* ret = 0;
    if(!(ret = static_cast(Array*,malloc(sizeof(Array))))) return 0;
    ret->_meta = &ArrayMetaData;
    ret->_data = (ArrayData){ ._size = size, ._elemSize = elemSize };
    if(!(ret->_data._data = malloc(elemSize*size))){
        free(ret);
        return 0;
    }

    return ret;
}
void Array_delete(Array* this){
    if(!this) return;
    if(this->_data._data) free(this->_data._data);
    free(this);
}

/* -- List -- */
typedef IteratorPriv ListIterator;
List* List_new(){
    
}
void List_delete(List* this){
    
}

/* -- Set -- */
typedef IteratorPriv SetIterator;
Set* Set_new(){
    
}
void Set_delete(Set* this){
    
}
