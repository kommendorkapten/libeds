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
SOURCES = btree.c llist.c stack.c hmap.c
RAWOBJS = $(SOURCES:%.c=%.o)
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
	lint -Xc99 -m64 -errwarn=%all -errchk=%all -Ncheck=%all -Nlevel=2 -u -m -erroff=E_FUNC_RET_ALWAYS_IGNOR $(SOURCES)
