def: compile link

compile:
	gcc -c main.c
	gcc -c cmd.c cmd.h

link:
	gcc -o csh main.o cmd.o
