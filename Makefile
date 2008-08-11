PACKAGE=lua-oocairo
VERSION=$(shell head -1 Changes | sed 's/ .*//')
RELEASEDATE=$(shell head -1 Changes | sed 's/.* //')
PREFIX=/usr/local
DISTNAME=$(PACKAGE)-$(VERSION)

# The path to where the module's source files should be installed.
LUA_CPATH:=$(shell pkg-config lua5.1 --define-variable=prefix=$(PREFIX) \
                              --variable=INSTALL_CMOD)

LIBDIR = $(PREFIX)/lib

# Uncomment this to run the regression tests with valgrind.
#VALGRIND = valgrind -q --leak-check=yes --show-reachable=yes --num-callers=10

OBJECTS = oocairo.lo
SOURCES := $(OBJECTS:.lo=.c)

LIBTOOL := libtool --quiet

CFLAGS := -ansi -pedantic -Wall -W -Wshadow -Wpointer-arith \
          -Wcast-align -Wwrite-strings -Wstrict-prototypes \
          -Wmissing-prototypes -Wnested-externs -Wno-long-long \
          $(shell pkg-config --cflags lua5.1 cairo-png) \
          -DVERSION=\"$(VERSION)\"
LDFLAGS := $(shell pkg-config --libs lua5.1 cairo-png)

# Uncomment this line to enable optimization.  Comment it out when running
# the test suite because it makes the assert() errors clearer and avoids
# warnings about ridiculously long string constants with some versions of gcc.
#CFLAGS := $(CFLAGS) -O3 -fomit-frame-pointer

# Uncomment this line to enable debugging.
DEBUG := -g

# Uncomment this line to prevent the module from being unloaded when Lua exits,
# so that Valgrind can still access the debugging symbols.
#DEBUG := $(DEBUG) -DVALGRIND_LUA_MODULE_HACK

# Uncomment one of these lines to enable profiling and/or gcov coverage testing.
#DEBUG := $(DEBUG) -pg
#DEBUG := $(DEBUG) -fprofile-arcs -ftest-coverage

all: liblua-oocairo.la

test: all
	echo 'lunit.main({...})' | $(VALGRIND) lua -llunit - test/*.lua

# Dependencies.
%.d: %.c
	@echo 'DEP>' $@
	@$(CC) -M $(CFLAGS) $< | \
	   sed -e 's,\($*\)\.o[ :]*,\1.lo $@ : ,g' > $@
-include $(SOURCES:.c=.d)

%.lo: %.c
	@echo 'CC>' $@
	@$(LIBTOOL) --mode=compile $(CC) $(CFLAGS) $(DEBUG) -c -o $@ $<
liblua-oocairo.la: oocairo.lo
	@echo 'LD>' $@
	@$(LIBTOOL) --mode=link $(CC) $(LDFLAGS) $(DEBUG) -o $@ $< -rpath $(LIBDIR)

clean:
	rm -f *.o *.lo *.d core
	rm -rf liblua-oocairo.la .libs
	rm -f gmon.out *.bb *.bbg *.da *.gcov

.PHONY: all test clean
