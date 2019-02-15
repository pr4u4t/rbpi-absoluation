#ifndef _COLLECTIONS_H_
#define _COLLECTIONS_H_

#include <inttypes.h>
#include <sys/queue.h>
#include <stdlib.h>
#include <meta.h>

typedef struct _MetaData MetaData;

typedef struct _Array Array;
typedef struct _List List;
typedef struct _Set Set;

typedef struct _ArrayFuncs ArrayFuncs;
typedef struct _ListFuncs ListFuncs;
typedef struct _SetFuncs SetFuncs;

typedef struct _ArrayData ArrayData;
typedef struct _ListData ListData;
typedef struct _ListItem ListItem;
typedef struct _SetData SetData;

typedef struct _IteratorPriv Iterator; 
typedef struct _IteratorFuncs IteratorFuncs;
typedef struct _IteratorPriv IteratorPriv;
typedef struct _IterableFuncs IterableFuncs;
typedef struct _Iterable Iterable;

/* -- Iterator Interface -- */

struct _IteratorPriv {
    void* _base;
    void* _state;
};

struct _IteratorFuncs {
    void (*next) (Iterator* this);
    void (*prev) (Iterator* this);
    void* (*data) (Iterator* this);
    void* (*key) (Iterator* this);
};

struct _IterableFuncs {
    IteratorFuncs   _base;
    Iterator    (*begin)    (void* this);
    Iterator    (*end)      (void* this);
};

struct _Iterable {
    const Meta* _meta;
};

/* -- ARRAY -- */

struct _ArrayFuncs {
    IterableFuncs    _base;
    void*       (*data)         (Array* this);
    void*       (*at)           (Array* this, size_t idx);
    void        (*set)          (Array* this, size_t idx, void* data);
    size_t      (*size)         (Array* this);
    size_t      (*elemSize)     (Array* this);
    void        (*setElemSize)  (Array* this, size_t size);
//    void        (*setMaxSize)   (Array* this, size_t size);
//    size_t      (*maxSize)      (Array* this);
    
};

struct _ArrayData {
    void*   _data;
    size_t  _maxSize;
    size_t  _elemSize;
    size_t  _size;
};

struct _Array {
    const Meta* _meta;
    ArrayData      _data;
};

/* -- LIST -- */

struct _ListFuncs {
    IterableFuncs _base;
    void (*push)(List* this, void* data);
    void* (*pop) (List* this);
};

struct _ListItem {
    void* _data;
    TAILQ_ENTRY(entry) _entries;
};

struct _ListData {
    TAILQ_HEAD(tailhead, entry) _head;
};

struct _List {
    const MetaData* _meta;
    ListData _data;
};

/* -- SET -- */

struct _SetFuncs {
    IterableFuncs _base;
    void (*set) (Set* this,void* key,void* value);
    void (*data) (Set* this,void* key);
};

struct _SetData {
    

};

struct _Set {
    const MetaData* _meta;
    SetData _data;
};

Array* Array_new(size_t elemSize,size_t size);
void Array_delete(Array* this);

List* List_new();
void List_delete(List* this);

Set* Set_new();
void Set_delete(Set* this);

#endif
