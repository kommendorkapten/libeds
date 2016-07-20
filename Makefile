CC = gcc
CFLAGS = -m64 -O2 -I/usr/local/include
LFLAGS += -L/usr/local/lib -lscut
UNAME = $(shell uname -s)
DEBUG = 1

# Default to c99 on Solaris
ifeq ($(UNAME), SunOS)
CC = c99
CFLAGS += -D_POSIX_C_SOURCE=200112L 
endif

# Configure stuff based on compiler
ifeq ($(CC), gcc)
CFLAGS += -W -Wall -pedantic -std=c99
endif

# Configure based on OS
ifeq ($(UNAME), SunOS)
ifeq ($(CC), c99)
CFLAGS += -v
endif
endif

ifeq ($(DEBUG), 1)
CFLAGS += -g
else
CFLAGS += -DNDEBUG
endif

DIRS = obj bin
RAWOBJS = btree.o llist.o stack.o
OBJS = $(RAWOBJS:%=obj/%)
RAWTESTS = $(RAWOBJS:%.o=%_test)
TESTS = $(RAWTESTS:%=bin/%)

.PHONY: clean

###################################################################

all: $(OBJS)

dir: $(DIRS)

$(DIRS):
	mkdir $(DIRS)

test: $(TESTS)

bin/%: %.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $< $(OBJS) $(LFLAGS)

obj/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj/*.o bin/*

lint:
	lint -Xc99 -m64 -errchk=%all -Ncheck=%all -Nlevel=3 llist.c
