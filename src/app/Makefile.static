CC=gcc
CFLAGS=-D_GNU_SOURCE
LFLAGS=-L../../build
LIBS=-lbcm2835 -ldl -labi

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
INCLUDE=-I . -I ../lib/bcm -I ../ABI

RESULT=../../build/adc


all: $(OBJECTS) $(RESULT)

$(OBJECTS): %.o: %.c
	$(CC) -c $< -o $@ ${CFLAGS} ${INCLUDE}

$(RESULT): $(OBJECTS)
	$(CC) $(LFLAGS) -o $@ $^ $(LIBS) 

clean:
	rm -f $(OBJECTS) $(RESULT)
