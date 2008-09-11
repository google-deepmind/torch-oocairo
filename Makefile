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
MANPAGES = doc/lua-oocairo-context.3 doc/lua-oocairo-fontface.3 doc/lua-oocairo-matrix.3 doc/lua-oocairo-path.3 doc/lua-oocairo-pattern.3 doc/lua-oocairo-surface.3 doc/lua-oocairo.3

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

all: liblua-oocairo.la $(MANPAGES)

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

doc/lua-%.3: doc/lua-%.pod Changes
	sed 's/E<copy>/(c)/g' <$< | sed 's/E<ndash>/-/g' | \
	    pod2man --center="Lua OO Cairo binding" \
	            --name="$(shell echo $< | sed 's/^doc\///' | sed 's/\.pod$$//' | tr a-z A-Z)" --section=3 \
	            --release="$(VERSION)" --date="$(RELEASEDATE)" >$@

install: all
	mkdir -p $(LUA_CPATH)
	install --mode=644 .libs/liblua-oocairo.so.0.0.0 $(LUA_CPATH)/oocairo.so
	mkdir -p $(PREFIX)/share/man/man3
	for manpage in $(MANPAGES); do \
	    gzip -c $$manpage >$(PREFIX)/share/man/man3/$$(echo $$manpage | sed -e 's/^doc\///').gz; \
	done

checktmp:
	@if [ -e tmp ]; then \
	    echo "Can't proceed if file 'tmp' exists"; \
	    false; \
	fi
dist: all checktmp
	mkdir -p tmp/$(DISTNAME)
	tar cf - --files-from MANIFEST | (cd tmp/$(DISTNAME) && tar xf -)
	cd tmp && tar cf - $(DISTNAME) | gzip -9 >../$(DISTNAME).tar.gz
	cd tmp && tar cf - $(DISTNAME) | bzip2 -9 >../$(DISTNAME).tar.bz2
	rm -rf tmp

clean:
	rm -f *.o *.lo *.d core
	rm -rf liblua-oocairo.la .libs
	rm -f gmon.out *.bb *.bbg *.da *.gcov
	rm -f $(MANPAGES)

.PHONY: all checktmp dist install test clean
