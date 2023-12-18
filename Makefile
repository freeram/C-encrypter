CC=gcc
HDRS=include
INCDIRS=-I$(HDRS)
OPT=-O0
CFLAGS=-Wall -Wextra $(INCDIRS) $(OPT)

DEBUG ?=
ifeq ($(DEBUG),1)
    CFLAGS += -g -DDEBUG
endif

SRCDIR=src
OBJDIR=obj
BINDIR=bin

OBJECTS := $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(wildcard $(SRCDIR)/*.c)))

BINARY=encrypt

all: $(BINDIR)/$(BINARY)

$(BINDIR)/$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BINDIR)/$(BINARY) $(OBJDIR)/*.o
