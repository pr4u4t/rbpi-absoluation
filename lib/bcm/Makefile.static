CC=gcc
CFLAGS=-fPIC
LFLAGS=
LIBS=

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
INCLUDES=-I . 

RESULT=../../build/libbcm2835.so


all: $(OBJECTS) $(RESULT)

$(OBJECTS): %.o: %.c
	$(CC) -c $< -o $@ ${CFLAGS} ${INCLUDES}

$(RESULT): $(OBJECTS)
	$(CC) -shared -o $@ $^ $(LFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(RESULT)
