# 
# Makefile
# Created: Mon Jan  8 07:00:27 2001 by tek@wiw.org
# Revised: Sun Feb 25 00:23:21 2001 by tek@wiw.org
# Copyright 2001 Julian E. C. Squires (tek@wiw.org)
# This program comes with ABSOLUTELY NO WARRANTY.
# $Id$
# 
#

.EXPORT_ALL_VARIABLES:

TOPDIR=$(CURDIR)
#
# Defines:
#  -DUSE_X86_ASM  - use x86 optimized assembler versions where possible
#                   (requires nasm)
#  -DUSE_MIPS_ASM - use MIPS optimized assembler versions where possible
#  -DUSE_SH_ASM   - use SuperH asm
#
#  -DDEBUG        - see error_debug() messages
#
DEFINES=-DUSE_X86_ASM -DDEBUG
DENT_CPPFLAGS=-I$(TOPDIR)/include -L$(TOPDIR)/lib $(DEFINES)
DENT_CFLAGS=-Wall -pedantic -g $(DENT_CPPFLAGS)
DENT_LDFLAGS=$(LIBS)
DIRS=lib tests docs demos tools

### Targets
# X11
TARGET=x11
LIBS=-L/usr/X11R6/lib -lXext -lX11
# SVGAlib
#TARGET=svgalib
#LIBS=-lvga
# null
#TARGET=null
#LIBS=

default: all

svgalib:
	$(MAKE) default TARGET=svgalib LIBS='-lvga'

x11:
	$(MAKE) default TARGET=x11 LIBS='-L/usr/X11R6/lib -lXext -lX11'

null:
	$(MAKE) default TARGET=null LIBS=

.PHONY: default svgalib x11 all clean realclean test profile $(DIRS)

all: $(DIRS)

test: tests

profile:
	$(MAKE) -C tests profile

$(DIRS):
	$(MAKE) -C $@

tests: lib
demos: lib

clean:
	for i in $(DIRS); do make -C $$i clean; done
	rm -f *~

realclean:
	for i in $(DIRS); do make -C $$i realclean; done
	rm -f *~

# EOF Makefile
