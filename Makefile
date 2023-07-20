CC = gcc
CCFLAGS = -Wall -Werror -g
LDFLAGS = -lm -lncurses
OUTPUT = pacperson
SRCDIR = src/

all: drawing pacperson main

main: ${SRCDIR}main.c
	${CC} ${SRCDIR}main.c drawing.o pacperson.o -o ${OUTPUT} ${CCFLAGS} ${LDFLAGS}

pacperson: ${SRCDIR}pacperson.c ${SRCDIR}pacperson.h
	${CC} -c ${SRCDIR}pacperson.c ${CCFLAGS} ${LDFLAGS}

drawing: ${SRCDIR}drawing.c ${SRCDIR}drawing.h
	${CC} -c ${SRCDIR}drawing.c ${CCFLAGS} ${LDFLAGS}


distclean:
	rm *.o

clean:
	rm pacperson *.o
