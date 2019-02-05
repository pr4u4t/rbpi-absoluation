#ifndef _MAIN_H_
#define _MAIN_H_

#ifdef _GNU_SOURCE
    #include <stdint.h>
    #include <signal.h>
    #include <ucontext.h>
#endif

#include <stdio.h>
#include <bool.h>
#include <app.h>
#include <getopt.h>
#include <mloader.h>
#include <shell.h>

#define _NAME_		"SensorsAbsoLUAtion"	
#define _VERSION_ 	9999 

#define ERROR_JUMP(code,text) { msg = text; ret = code; goto EXIT; }

#endif
