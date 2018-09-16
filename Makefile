# Lovingly crafted by hand.

# Some defaults.  Feel free to override them.
CFLAGS += -g -O2 -Wall
PREFIX = /usr/local

# ------------ Systems and Platforms ------------

# System-specific CFLAGS
sys_CFLAGS =

# Compiler-specific options
cc_CFLAGS =

# Get the OS and machine type.
OS := $(shell uname -s)
MACHINE := $(shell uname -m)

# Pick a compiler, preferring clang, if the user hasn't expressed a preference.
ifeq ($(CC), cc)
	CLANG_PATH = $(shell which clang)
	GCC_PATH = $(shell which gcc)
	ifdef CLANG_PATH
		CC = clang
	else
		ifdef GCC_PATH
			CC = gcc
		endif
	endif
endif

CC_name := $(shell basename $(CC))

# Linux needs some love.
ifeq ($(OS), Linux)
sys_CFLAGS += -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_ISOC11_SOURCE -D__USE_BSD
LIBS += -lm $(shell pkg-config --libs libbsd-overlay uuid)
endif

# Our GCC options.
ifeq ($(CC_name), gcc)
sys_CFLAGS += -fstack-protector
# Warnings
sys_CFLAGS += -Wall -Wextra -Wno-multichar -Wno-unknown-pragmas
cc_CFLAGS = -include $(PCHFILENAME)
endif

# Our clang options.
ifeq ($(CC_name), clang)
sys_CFLAGS += -fstack-protector-all -fstrict-enums -ftrapv
# Warnings
sys_CFLAGS += -Wno-four-char-constants # -Weverything 
cc_CFLAGS = -include-pch $(PCHFILE)
endif

# General options
ifeq ($(GC), 1)
sys_CFLAGS += -DGC_ENABLED
LIBS += -lgc
endif

ifneq ($(DEBUG), 1)
sys_CFLAGS += -DNDEBUG
endif

# ------------ hfsinspect ------------

PRODUCTNAME = hfsinspect
BUILDDIR = build/$(OS)-$(MACHINE)
BINARYPATH = $(BUILDDIR)/$(PRODUCTNAME)
OBJDIR = $(BUILDDIR)/obj

AUXFILES = CHANGELOG LICENSE Makefile README.md hfsinspect.1

SOURCEDIR = src
VENDORDIR = vendor

SOURCES = $(shell find $(SOURCEDIR) -type f -name *.c)
SOURCES += $(wildcard $(VENDORDIR)/crc32c/*.c)
SOURCES += $(wildcard $(VENDORDIR)/memdmp/*.c)

HEADERS = $(shell find $(SOURCEDIR) -type f -name *.h)
HEADERS += $(wildcard $(VENDORDIR)/crc32c/*.h)
HEADERS += $(wildcard $(VENDORDIR)/memdmp/*.h)

OBJFILES = $(patsubst %.c, $(OBJDIR)/%.o, $(SOURCES))
OBJFILES += $(patsubst %.c, $(OBJDIR)/%.o, $(SOURCES))

DEPFILES = $(patsubst %.c, $(OBJDIR)/%.d, $(SOURCES))
DEPFILES = $(patsubst %.c, $(OBJDIR)/%.d, $(SOURCES))

PCHFILENAME = $(SOURCEDIR)/cdefs.h
PCHFILE  = $(OBJDIR)/$(PCHFILENAME).pch
ALLFILES = $(SOURCES) $(HEADERS) $(AUXFILES)

# Required CFLAGS
bin_CFLAGS = -std=c1x -msse4.2 -I$(SOURCEDIR) -I$(VENDORDIR)

INSTALL = install
RM = rm -f


# ------------ Definitions ------------

# The superset we're going to use.
ALL_CFLAGS = $(CFLAGS) $(bin_CFLAGS) $(sys_CFLAGS)
ALL_LDFLAGS = $(LDFLAGS) 

# ------------ Actions ------------

.PHONY: all everything clean distclean pretty docs install uninstall test clean-hfsinspect clean-test clean-docs

all: $(PRODUCTNAME)

$(PRODUCTNAME): $(PCHFILE) $(BINARYPATH)
#	 @echo "Compiled and linked with: $(CC)/$(ALL_CFLAGS)/$(ALL_LDFLAGS)/$(LIBS)/"

$(BINARYPATH): $(OBJFILES)
	@echo Building hfsinspect
	@mkdir -p `dirname $(BINARYPATH)`
	$(CC) -o $(BINARYPATH) $^ $(ALL_CFLAGS) $(ALL_LDFLAGS) $(LIBS)
	@echo "=> $(BINARYPATH)"

$(OBJDIR)/%.o: %.c $(PCHFILE)
	@echo Compiling $<
	@mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(cc_CFLAGS) $(ALL_CFLAGS)

$(OBJDIR)/%.h.pch: %.h
	@echo Precompiling $<
	@mkdir -p `dirname $@`
	$(CC) -o $@ -x c-header $< $(ALL_CFLAGS)

$(OBJDIR)/%.d: %.c
	@echo Generating dependancies $<
	@mkdir -p `dirname $@`
	$(CC) -M -c $< -MF $@

everything: all docs

clean: clean-hfsinspect

distclean: clean-hfsinspect clean-test clean-docs
	@echo "Removing all build artifacts."
	$(RM) -r build

pretty:
	find $(SOURCEDIR) -iname '*.[hc]' -and \! -path '*Apple*' | uncrustify -c uncrustify.cfg -F- --replace --no-backup --mtime -lC

docs:
	@echo "Building documentation."
	doxygen docs/doxygen.config >/dev/null 2>&1

install: $(BINARYPATH)
	@echo "Installing hfsinspect in $(PREFIX)"
	@mkdir -p $(PREFIX)/bin
	$(INSTALL) $(BINARYPATH) $(PREFIX)/bin
	@echo "Installing manpage in $(PREFIX)"
	@mkdir -p $(PREFIX)/share/man/man1
	$(INSTALL) hfsinspect.1 $(PREFIX)/share/man/man1

uninstall:
	$(RM) $(PREFIX)/bin/$(PRODUCTNAME)
	$(RM) $(PREFIX)/share/man/man1/hfsinspect.1

test: all
	gunzip < images/test.img.gz > images/test.img
	./tools/tests.sh $(BINARYPATH) images/test.img

clean-test:
	@echo "Cleaning test images."
	$(RM) "images/test.img" "images/MBR.dmg"

clean-hfsinspect:
	@echo "Cleaning hfsinspect."
	$(RM) -r $(BUILDDIR)

clean-docs:
	@echo "Cleaning documentation."
	$(RM) -r docs/html docs/doxygen.log

dist.tgz: $(ALLFILES)
	tar -czf dist.tgz $^
