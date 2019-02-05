#ifndef _FILE_TERMINAL_H_
#define _FILE_TERMINAL_H_

#include <terminal.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdarg.h>

typedef struct _FileTerminal FileTerminal;
typedef struct _FileTerminalFuncs FileTerminalFuncs;
typedef struct _FileTerminalMetaFuncs FileTerminalMetaFuncs;
typedef struct _FileTerminalData FileTerminalData;

struct _FileTerminalData {
	void* _in;
	void* _out;
};

struct _FileTerminalFuncs {
    TerminalFuncs _base;
};

struct _FileTerminalMetaFuncs {
    TerminalMetaFuncs _base;
};

struct _FileTerminal {
	Terminal           _base;
	FileTerminalData   _data;
};

OBJECT_EXPORT(FileTerminal)

#endif
