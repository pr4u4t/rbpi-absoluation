#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <abi.h>
#include <object.h>

typedef enum _TerminalStreamMode TerminalStreamMode;
enum _TerminalStreamMode {
	STREAM_BLOCK		= 0,
	STREAM_NON_BLOCK 	= 1
};

OBJECT(Terminal,Object,
    METHODS(
        VMETHOD(bool,                open,           void*,          void*,  TerminalStreamMode  ),
        VMETHOD(int,                 read,           uint8_t*,       size_t                      ),
        VMETHOD(int,                 write,          const uint8_t*, size_t                      ),
        VMETHOD(int,                 readf,          const uint8_t*, uint8_t*                    ),
        VMETHOD(int,                 writef,         const uint8_t*,  ...                      ),  //VARIADIC
        VMETHOD(void,                flush                                                       ),
        VMETHOD(void,                close                                                       ),
        VMETHOD(void,                setStreamMode,  TerminalStreamMode                          ),
        VMETHOD(TerminalStreamMode,  streamMode                                                  ),
        VMETHOD(void,                in                                                          ),
        VMETHOD(void,                out                                                         )
    ),
    PROPERTIES(
    )
)

//OBJECT_EXPORT(Terminal)

#endif
