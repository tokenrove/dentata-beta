# 
# Makefile
# Created: Mon Jan  8 07:00:27 2001 by tek@wiw.org
# Revised: Fri Jul 13 03:07:38 2001 by tek@wiw.org
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
#  -DSIXTEENBIT   - word size is 16-bits (ARM in Thumb mode)
#  -DTHIRTYTWOBIT - word size is 32-bits
#  -DSIXTYFOURBIT - word size is 64-bits
#
DEFINES=-DTHIRTYTWOBIT -DDEBUG
CPPFLAGS=-I$(CURDIR)/include -L$(CURDIR) $(DEFINES)
CFLAGS=-Wall -pedantic -g -O6 $(CPPFLAGS)
LDFLAGS=$(LIBS)

###
# Base targets
#   dos
#   unix-svgalib
#   unix-x11
#   linux-svgalib
#   linux-x11
#   gba
TARGET=unix-x11

# Some of the things that can be tweaked below are:
###
# TARGETOS -- operating system
#   unix    -- UNIX/POSIX (Linux, OpenBSD, DOS w/DJGPP, et cetera)
#   gba     -- Gameboy Advance
#   null    -- empty stubs
###
# TARGETARCH -- architecture
#   generic -- don't use architecture specific code
#   arm     -- use ARM/Thumb specific code
#   x86     -- use IA32 specific code

ifeq ($(TARGET),dos)
AS=nasm
ASFLAGS=-f coff
TARGETARCH=generic
TARGETOS=unix
endif

ifeq ($(TARGET),unix-svgalib)
AS=nasm
# Change this as appropriate for the OS
ASFLAGS=-f aoutb
# Use x86 if appropriate.
TARGETARCH=generic
TARGETOS=unix
LIBS=-lvga
endif

ifeq ($(TARGET),linux-svgalib)
AS=nasm
ASFLAGS=-f elf
# Use x86 if appropriate
TARGETARCH=generic
TARGETOS=unix
LIBS=-lvga
endif

ifeq ($(TARGET),unix-x11)
AS=nasm
# Change this as appropriate for the OS
ASFLAGS=-f aoutb
# Use x86 if appropriate.
TARGETARCH=generic
TARGETOS=unix
LIBS=-L/usr/X11R6/lib -lXext -lX11
CPPFLAGS:=$(CPPFLAGS) -I/usr/X11R6/include
endif

ifeq ($(TARGET),linux-x11)
AS=nasm
ASFLAGS=-f elf
# Use x86 if appropriate.
TARGETARCH=generic
TARGETOS=unix
LIBS=-L/usr/X11R6/lib -lXext -lX11
CPPFLAGS:=$(CPPFLAGS) -I/usr/X11R6/include
endif

ifeq ($(TARGET),gba)
TARGETARCH=arm
TARGETOS=gba
LIBS=
endif

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
	$(RM) src/*~ docs/*~ tests/*~ tools/*~ include/dentata/*~

distclean: libdistclean toolsdistclean testsdistclean docsdistclean clean

# EOF Makefile
