CC=gcc
HDRS=include
INCDIRS=-I. -I$(HDRS)
OPT=-O0
CFLAGS=-Wall -Wextra $(INCDIRS) $(OPT)

DEBUG ?=
ifeq ($(DEBUG),1)
    CFLAGS += -g
endif

SRC=src
OBJDIR=obj
BINDIR=bin

CFILES := $(SRC)/main.c $(SRC)/filereader.c $(SRC)/encryption.c
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJDIR)/%.o, $(CFILES))

BINARY=encrypt

all: $(BINDIR)/$(BINARY)

$(BINDIR)/$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

$(OBJDIR)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BINDIR)/$(BINARY) $(OBJDIR)/*.o
