all: clean bld

bld: prepare_dir compile link

NAME := csh
CC := gcc
WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wconversion -Wstrict-prototypes
CFLAGS := -g $(WARNINGS)
PROJDIRS := ../src
SRCFILES := $(shell find $(PROJDIRS) -type f -name "*.c")
HDRFILES := $(shell find $(PROJDIRS) -type f -name "*.h")
GCHFILES := $(shell find $(PROJDIRS) -type f -name "*.gch")
OBJFILES := $(shell find . -type f -name "*.o")
BINFILE := ../bin/$(NAME)


prepare_dir:
	mkdir -p ../bin

compile:
	$(CC) $(CFLAGS) -c $(SRCFILES)

link:
	$(CC) $(CFLAGS) -o $(BINFILE) $(OBJFILES)

clean:
	$(RM) $(OBJFILES) $(BINFILE) $(GCHFILES)