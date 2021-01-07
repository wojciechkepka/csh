all: clean release

debug: prepare_dir compile link

release: clean prepare_dir compile_release link_release

NAME := csh
CC := gcc
WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wconversion -Wstrict-prototypes
CFLAGS := -std=gnu11 -g $(WARNINGS)
CFLAGS_RELEASE := -std=gnu11 -fPIC -O3 $(WARNINGS)
PROJDIRS := ../src
SRCFILES := $(shell find $(PROJDIRS) -type f -name "*.c")
HDRFILES := $(shell find $(PROJDIRS) -type f -name "*.h")
GCHFILES := $(shell find $(PROJDIRS) -type f -name "*.gch")
_OBJFILES := find . -type f -name "*.o"
OBJFILES := $(shell $(_OBJFILES))
BINFILE := ../bin/$(NAME)


prepare_dir:
	mkdir -p ../bin

compile:
	$(CC) $(CFLAGS) -c $(SRCFILES)

link:
	$(eval OBJFILES := $(shell $(_OBJFILES)))
	$(CC) $(CFLAGS) -o $(BINFILE)_dbg $(OBJFILES)

compile_release:
	$(CC) $(CFLAGS_RELEASE) -c $(SRCFILES)

link_release:
	$(eval OBJFILES := $(shell $(_OBJFILES)))
	$(CC) $(CFLAGS_RELEASE) -o $(BINFILE) $(OBJFILES)

clean:
	$(RM) $(OBJFILES) $(GCHFILES) ../bin/*
