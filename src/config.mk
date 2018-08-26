# compiler and linker
CC = cc

PREFIX=/usr/local

# flags
CFLAGS = -ggdb3 -Os -Wall -Wno-unused-variable -march=native -fno-stack-protector -D_GNU_SOURCE

DEP_ALSA = `pkg-config --cflags --libs alsa`
DEP_LIBNOTIFY = `pkg-config --cflags --libs libnotify`

LIBS = ${DEP_ALSA} ${DEP_LIBNOTIFY}

