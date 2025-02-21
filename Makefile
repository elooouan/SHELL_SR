.PHONY: all, clean

# Disable implicit rules
.SUFFIXES:

CC=gcc
CFLAGS=-Wall -g
VPATH=src/

# Note: -lnsl does not seem to work on Mac OS but will
# probably be necessary on Solaris for linking network-related functions 
#LIBS += -lsocket -lnsl -lrt
LIBS+=-lpthread

INCLDIR = -Iinclude
INCLUDE = include/readcmd.h include/csapp.h
OBJS = readcmd.o csapp.o execcmd.o handlers.o jobs.o foreground.o

all: shell

docs:
	doxygen Doxyfile

%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) $(INCLDIR) -c -o $@ $<

%: %.o $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)



clean:
	rm -f shell *.o
	rm -rf doc

