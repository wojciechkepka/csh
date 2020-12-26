def: compile link

compile:
	gcc -c main.c
	gcc -c cmd.c cmd.h
	gcc -c line.c line.h
	gcc -c term.c term.h
	gcc -c path.c path.h

link:
	gcc -o csh main.o cmd.o line.o term.o path.o
