CC=gcc
CFLAGS=
LFLAGS=
LIBS=-L../../build 
#-lbcm2835

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
INCLUDES=-I . 
#-I ../../lib/bcm

RESULT=../../build/libabi.so


all: $(OBJECTS) $(RESULT)

$(OBJECTS): %.o: %.c
	$(CC) -fPIC -c $< -o $@ ${CFLAGS} ${INCLUDES}

$(RESULT): $(OBJECTS)
	$(CC) -fPIC -shared -o $@ $^ $(LFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(RESULT)
