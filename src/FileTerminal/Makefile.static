CC=gcc
CFLAGS=
LFLAGS=
LIBS=-L../../build -labi

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
INCLUDES=-I . -I ../ABI

RESULT=../../build/plugins/FileTerminal.so


all: $(OBJECTS) $(RESULT)

$(OBJECTS): %.o: %.c
	$(CC) -fPIC -c $< -o $@ ${CFLAGS} ${INCLUDES}

$(RESULT): $(OBJECTS)
	$(CC) -fPIC -shared -o $@ $^ $(LFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(RESULT)
