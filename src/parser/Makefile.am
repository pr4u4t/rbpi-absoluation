LEXER=flex
YACC=bison
CC=gcc
CFLAGS=
AR=ar
ARFLAGS=rcs

INCLUDE=-I ./  -I ../CommandShell -I ../ABI

LSOURCES=$(wildcard *.y) $(wildcard *.l)
LEXS=$(filter %.l,$(LSOURCES))
YACCS=$(filter %.y,$(LSOURCES))
LEXRES=$(patsubst %.l,%.c,$(LEXS))
YACCRES=$(patsubst %.y,%.c,$(YACCS))
SOURCES=$(LEXRES) $(YACCRES)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
RESULT=interpreter.o

all: $(LEXRES) $(YACCRES) $(OBJECTS) $(RESULT)

$(LEXRES): %.c: %.l
	flex --header-file=lexer.h --outfile=$@ $<

$(YACCRES): %.c: %.y
	bison -v --defines=parser.h --output=$@ $<

$(OBJECTS): %.o: %.c
	$(CC) -fPIC -c $< -o $@ $(CFLAGS) $(INCLUDE)

$(RESULT): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

versions:
	/usr/local/bin/flex --version
	/usr/local/bin/bison --version

clean:
	rm -f $(LEXRES) $(YACCRES) $(OBJECTS) $(RESULT) lexer.h parser.h 
