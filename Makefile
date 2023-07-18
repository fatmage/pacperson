CC = gcc
CCFLAGS = -Wall -Werror -lm -lncurses -g
OUTPUT = pacperson
SRCDIR = src/

all: drawing pacperson main

main: ${SRCDIR}main.c
	${CC} ${SRCDIR}main.c drawing.o pacperson.o -o ${OUTPUT} ${CCFLAGS}

pacperson: ${SRCDIR}pacperson.c ${SRCDIR}pacperson.h
	${CC} -c ${SRCDIR}pacperson.c ${CCFLAGS}

drawing: ${SRCDIR}drawing.c ${SRCDIR}drawing.h
	${CC} -c ${SRCDIR}drawing.c ${CCFLAGS}


distclean:
	rm *.o

clean:
	rm pacperson *.o
