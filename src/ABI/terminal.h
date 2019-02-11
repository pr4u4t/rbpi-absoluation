#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <object.h>

typedef enum _TerminalStreamMode TerminalStreamMode;
enum _TerminalStreamMode {
	STREAM_BLOCK		= 0,
	STREAM_NON_BLOCK 	= 1
};

OBJECT(Terminal,Object,
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

OBJECT_EXPORT(Terminal)

#endif
