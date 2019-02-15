#ifndef _FILE_TERMINAL_H_
#define _FILE_TERMINAL_H_

#include <terminal.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdarg.h>

/*
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
*/

OBJECT(FileTerminal,Object,
        METHODS
        (
            METHOD(bool,    open,       void*,          void*,  TerminalStreamMode),
            METHOD(int,     read,       uint8_t*,       size_t),
            METHOD(int,     write,      const uint8_t*, size_t),
            METHOD(int,     readf,      const uint8_t*, uint8_t*),
            METHOD(int,     writef,     const uint8_t*, ...),
            METHOD(void,    flush),
            METHOD(void,    close),
            METHOD(void,    setStreamMode,                      TerminalStreamMode),
            METHOD(TerminalStreamMode,   streamMode),
            METHOD(void,    in),
            METHOD(void,    out)
        )
)

OBJECT_EXPORT(FileTerminal)

#endif
