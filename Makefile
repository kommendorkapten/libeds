CC = gcc
CFLAGS = -m64 -I/usr/local/include
LFLAGS += 
LSCUT = -L/usr/local/lib -lscut
OS = $(shell uname -s)
ISA = $(shell uname -i)
DEBUG = 1

# Default to c99 on Solaris
ifeq ($(OS), SunOS)
  CC = c99
  CFLAGS += -D_POSIX_C_SOURCE=200112L
endif

# Configure stuff based on compiler
ifeq ($(CC), gcc)
  CFLAGS += -W -Wall -pedantic -std=c99 -O2
endif

# Configure based on OS/Compiler
ifeq ($(OS), SunOS)
  ifeq ($(CC), c99)
    CFLAGS += -v -xO5
    ifeq ($(ISA), i86pc)
      CFLAGS += -xarch=sse4_2
    endif
  endif
endif

ifeq ($(DEBUG), 1)
  CFLAGS += -g
else
  CFLAGS += -DNDEBUG
endif

DIRS = obj bin
SOURCES = btree.c llist.c stack.c hmap.c heap.c
OBJS = $(SOURCES:%.c=obj/%.o)
TEST_OBJS = $(OBJS:%.o=%_test.o)

.PHONY: clean
.PHONY: test
.PHONY: lint

###################################################################

all: $(OBJS)

dir: $(DIRS)

$(DIRS):
	mkdir $(DIRS)

test: bin/test $(TEST_OBJS)
	$<

bin/test: test.c $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $< $(OBJS) $(TEST_OBJS) $(LFLAGS) $(LSCUT)

bin/perf: perf.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $< $(OBJS) $(LFLAGS) -lm

obj/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj/*.o bin/*

lint:
	lint -Xc99 -m64 -errwarn=%all -errchk=%all -Ncheck=%all -Nlevel=1 -u -m -erroff=E_FUNC_RET_ALWAYS_IGNOR,E_SIGN_EXTENSION_PSBL,E_CAST_INT_TO_SMALL_INT $(SOURCES) perf.c
