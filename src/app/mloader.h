#ifndef _MLOADER_H_
#define _MLOADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/queue.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <bool.h>
#include <utils.h>
#include <factory.h>

typedef struct _MLoader MLoader;
typedef struct _MLoaderClass MLoaderClass;
typedef struct _ModEntry ModEntry;
typedef struct _MLoaderData MLoaderData;
typedef struct _ModInstance ModInstance;
typedef struct _MLoaderFuncs MLoaderFuncs;
typedef struct _MLoaderMetaFuncs MLoaderMetaFuncs;

struct _ModInstance {
    Object*                     mod; 
    LIST_ENTRY(_ModInstance)    entries;
};

struct _ModEntry {
	ObjectLoader*          ld;
	LIST_ENTRY(_ModEntry)  entries;
};

struct _MLoaderData {
    LIST_HEAD(_pluginLoaders,_ModEntry) _loaders;
    LIST_HEAD(_pluginInstances,_ModInstance) _instances;
	char _path[4096];
};

struct _MLoaderFuncs {
    FactoryFuncs _base;
};

struct _MLoaderMetaFuncs {
    FactoryMetaFuncs _base;
};

struct _MLoader {
    Factory     _base;
	MLoaderData _data;
};

MLoader* get_factory(void);

#endif
