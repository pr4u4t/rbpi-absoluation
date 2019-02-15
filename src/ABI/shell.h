#ifndef _SHELL_H_
#define _SHELL_H_

#include <terminal.h>

OBJECT(Shell,Object,
    METHODS
    (
        METHOD(void,        setVar,      string,    string  ),
        METHOD(string,      var,         string             ),
        METHOD(int,         vars                            ),
        METHOD(bool,        open,        pointer            ),
        METHOD(void,        close                           ),
        METHOD(int,         exec,        int,   string      ),
        METHOD(void,        setTerminal, pointer            )
    ),
    PROPERTIES(
        
    )
)

//OBJECT_EXPORT(Shell)

#endif
