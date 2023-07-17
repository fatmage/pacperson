CC = gcc
CCFLAGS = -Wall -Werror -lm -lncurses -g
OUTPUT = pacperson
SRCDIR = src/

all: pacperson main

main: ${SRCDIR}main.c ${SRCDIR}pacperson.c ${SRCDIR}pacperson.h
	${CC} ${SRCDIR}main.c pacperson.o -o ${OUTPUT} ${CCFLAGS}

pacperson: ${SRCDIR}pacperson.c ${SRCDIR}pacperson.h
	${CC} -c ${SRCDIR}pacperson.c ${CCFLAGS}


distclean:
	rm *.o

clean:
	rm pacperson *.o
