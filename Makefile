bld: compile link
dbg: compile_dbg link_dbg

NAME = csh
FILES = main.c cmd.c cmd.h line.c line.h term.c term.h path.c path.h prompt.c prompt.h
OFILES = main.o cmd.o line.o term.o path.o prompt.o

compile:
	gcc -Wpedantic -c $(FILES)

compile_dbg:
	gcc -Wpedantic -g -c $(FILES)

link:
	gcc -o $(NAME) $(OFILES)

link_dbg:
	gcc -g -o $(NAME) $(OFILES)