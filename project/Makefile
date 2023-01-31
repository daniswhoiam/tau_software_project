CC=gcc
CFLAGS=-ansi -Wall -Wextra -Werror -pedantic-errors
LDFLAGS=-lm
SOURCES=spkmeans.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=spkmeans

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) -o $@

clean:
	rm -rf *.o $(EXECUTABLE)