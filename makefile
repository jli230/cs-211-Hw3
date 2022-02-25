

# modify this makefile as necessary.
# for instance, you might want to use the stack module/ADT (in their
#   own files) to implement some of the features of the BPGame ADT.
#

all: bpop

simpleio.o: simpleio.c simpleio.h
	gcc -c simpleio.c

bboard.o:  bboard.c bboard.h
	gcc -c bboard.c

bpgame.o:  bpgame.c bpgame.h
	gcc -c bpgame.c

bpop: bpop.c bpgame.o simpleio.o 
	gcc bpop.c -o bpop bpgame.o simpleio.o

