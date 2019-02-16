#ifndef _SHELL_H_
#define _SHELL_H_

#include <terminal.h>

OBJECT(Shell,Object,
    METHODS
    (
        VMETHOD(void,        setVar,      string,    string  ),
        VMETHOD(string,      var,         string             ),
        VMETHOD(int,         vars                            ),
        VMETHOD(bool,        open,        pointer            ),
        VMETHOD(void,        close                           ),
        VMETHOD(int,         exec,        int,   string      ),
        VMETHOD(void,        setTerminal, pointer            )
    ),
    PROPERTIES(
    )
)

//OBJECT_EXPORT(Shell)

#endif
