# 
# Makefile
# Created: Mon Jan  8 07:33:59 2001 by tek@wiw.org
# Revised: Thu Apr 19 05:22:53 2001 by tek@wiw.org
# Copyright 2001 Julian E. C. Squires (tek@wiw.org)
# This program comes with ABSOLUTELY NO WARRANTY.
# $Id$
# 
#

VPATH=$(TOPDIR)/tools
CFLAGS=$(DENT_CFLAGS)
LDFLAGS=$(DENT_LDFLAGS)

TOOLS=listmodes rawfont2v0 viewtga

default: $(TOOLS)

$(TOOLS): $(TOPDIR)/lib/libdentata.a

$(TOOLS): %: %.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -ldentata

clean:
	rm -f *~ *.o

realclean: clean
	rm -f $(TOOLS)

# EOF Makefile
