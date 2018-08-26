# Makefile

include config.mk

all: options alsanotify

options:

	@echo "    CC = ${CC}"
	@echo "CFLAGS = ${CFLAGS}"
	@echo "  LIBS = ${LIBS}"

config.h: config.def.h

	@echo "cp config.def.h -> config.h"
	cp config.def.h config.h

alsanotify.o: alsanotify.c
	${CC} ${CFLAGS} -c alsanotify.c ${LIBS}

alsanotify: alsanotify.o
	${CC} ${CFLAGS} $@.o -o $@ ${LIBS}

install:
	cp -f alsanotify ${PREFIX}/bin/

clean:
	rm -f *.o alsanotify
