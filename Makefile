CC=gcc
INCDIRS=-I.
OPT=-O0
CFLAGS=-Wall -Wextra $(INCDIRS) $(OPT)

DEBUG ?=
ifeq ($(DEBUG),1)
    CFLAGS += -g
endif

CFILES=main.c filereader.c encryption.c
OBJECTS=main.o filereader.o encryption.o

BINARY=encrypt

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(BINARY) $(OBJECTS)
