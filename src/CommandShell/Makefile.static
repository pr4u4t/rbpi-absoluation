CC=gcc
CFLAGS=
LFLAGS=
LIBS=-L../../build  -labi -lxxhash `pkg-config --libs libffi`

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
INCLUDES=-I . -I ../ABI -I ../parser -I ../../lib/xxHash `pkg-config --cflags libffi`

RESULT=../../build/plugins/CommandShell.so


all: $(OBJECTS) $(RESULT)

$(OBJECTS): %.o: %.c
	$(CC) -fPIC -c $< -o $@ ${CFLAGS} ${INCLUDES}

$(RESULT): $(OBJECTS)
	$(CC) -fPIC -shared -o $@ $^ ../parser/interpreter.o  $(LFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(RESULT)
