# 
# Makefile
# Created: Mon Jan  8 07:00:27 2001 by tek@wiw.org
# Revised: Sat Jun 23 02:24:11 2001 by tek@wiw.org
# Copyright 2001 Julian E. C. Squires (tek@wiw.org)
# This program comes with ABSOLUTELY NO WARRANTY.
# $Id$
# 
#


PREFIX=/usr/local
INSTALLLIB=$(PREFIX)/lib
INSTALLHEADERS=$(PREFIX)/include

###
# Defines:
#  -DDEBUG        - see error_debug() messages
#  -DTHIRTYTWOBIT - word size is 32-bits
#  -DSIXTYFOURBIT - word size is 64-bits
#
DEFINES=-DTHIRTYTWOBIT -DDEBUG
CPPFLAGS=-I$(CURDIR)/include -L$(CURDIR) $(DEFINES)
CFLAGS=-Wall -pedantic -g -O6 $(CPPFLAGS)
LDFLAGS=$(LIBS)
# Comment this out if you aren't using the x86 specific code. (see
# TARGETARCH below)
AS=nasm
ASFLAGS=-f elf

###
# TARGETGFX -- graphics system
#   x11     -- X11
#   svgalib -- SVGAlib
#   null    -- empty stubs
TARGETGFX=x11

###
# TARGETOS -- operating system
#   unix    -- UNIX/POSIX (Linux, OpenBSD, DOS w/DJGPP, et cetera)
#   null    -- empty stubs
TARGETOS=unix

###
# TARGETARCH -- architecture
#   generic -- don't use architecture specific code
#   x86     -- use IA32 specific code
TARGETARCH=generic

###
# LIBS
#   X11 should use:
#LIBS=-L/usr/X11R6/lib -lXext -lX11
#   SVGAlib should use:
#LIBS=lvga
LIBS=-L/usr/X11R6/lib -lXext -lX11

###
# CPPFLAGS
#   X11 should use:
#CPPFLAGS:=$(CPPFLAGS) -I/usr/X11R6/include
# SVGAlib and null shouldn't need other flags.
CPPFLAGS:=$(CPPFLAGS) -I/usr/X11R6/include


### End of user configurable section ###

SHELL=/bin/sh
.SUFFIXES=.c .o

VPATH=./src ./include ./tools ./tests
.PHONY: default all install clean distclean check profile tools docs \
        libclean libdistclean toolsclean toolsdistclean testsclean \
        testsdistclean docsclean docsdistclean

include Makefile.lib Makefile.tls Makefile.tst Makefile.doc

default: all

all: libdentata.a tools docs check install

install: libdentata.a installlib installheaders

installlib: libdentata.a
	install -m 644 libdentata.a $(INSTALLLIB)

installheaders: $(INSTALLHEADERS)/dentata
	install -m 644 ./include/dentata/*.h $(INSTALLHEADERS)/dentata

$(INSTALLHEADERS)/dentata:
	mkdir $(INSTALLHEADERS)/dentata

TAGS:
	etags ./src/*.c ./include/dentata/*.h

clean: libclean toolsclean testsclean docsclean
	rm -f src/*~ docs/*~ tests/*~ tools/*~ include/dentata/*~

distclean: libdistclean toolsdistclean testsdistclean docsdistclean clean

# EOF Makefile
