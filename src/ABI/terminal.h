#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <metadata.h>

/*typedef struct _Terminal Terminal;*/
/*typedef struct _TerminalFuncs TerminalFuncs;*/
/*typedef struct _TerminalMetaFuncs TerminalMetaFuncs;*/

typedef enum _TerminalStreamMode TerminalStreamMode;

enum _TerminalStreamMode {
	STREAM_BLOCK		= 0,
	STREAM_NON_BLOCK 	= 1
};

/*struct _TerminalFuncs {*/
    /*ObjectFuncs _base;*/
    /*bool (*open) (Terminal* this, void* in, void* out, TerminalStreamMode mode);*/
	/*int (*read) (Terminal* this, uint8_t* buff, size_t size);*/
	/*int (*write) (Terminal* this, const uint8_t* buff, size_t size);*/
	/*int (*readf) (Terminal* this, const uint8_t *format, uint8_t* buff);*/
	/*int (*writef) (Terminal* this, const uint8_t* format, ... );*/
    /*void (*flush) (Terminal* this);*/
	/*void (*close) (Terminal* this);*/
    /*void (*setStreamMode) (Terminal* this, TerminalStreamMode mode);*/
    /*TerminalStreamMode (*streamMode) (Terminal* this);*/
    /*void* (*in) (Terminal *this);*/
    /*void* (*out) (Terminal* this);*/
/*};*/

/*struct _TerminalMetaFuncs {*/
    /*ObjectMetaFuncs _base;*/
    /*ObjectMetaFunc open;*/
    /*ObjectMetaFunc read;*/
    /*ObjectMetaFunc write;*/
    /*ObjectMetaFunc readf;*/
    /*ObjectMetaFunc writef;*/
    /*ObjectMetaFunc flush;*/
    /*ObjectMetaFunc close;*/
    /*ObjectMetaFunc setStreamMode;*/
    /*ObjectMetaFunc streamMode;*/
    /*ObjectMetaFunc in;*/
    /*ObjectMetaFunc out;*/
/*};*/

/*struct _Terminal {*/
	/*Object         _base;*/
/*};*/

OBJECT(Terminal,Object,
        METHODS
        (
            METHOD(bool,    open,       void*,          void*,  TerminalStreamMode)
            METHOD(int,     read,       uint8_t*,       size_t)
            METHOD(int,     write,      const uint8_t*, size_t)
            METHOD(int,     readf,      const uint8_t*, uint8_t*)
            METHOD(int,     writef,     const uint8_t*, ...)
            METHOD(void,    flush)
            METHOD(void,    close)
            METHOD(void,    setStreamMode,                      TerminalStreamMode)
            METHOD(TerminalStreamMode,   streamMode)
            METHOD(void,    in)
            METHOD(void,    out)
        )
OBJECT_EXPORT(Terminal)

#endif
