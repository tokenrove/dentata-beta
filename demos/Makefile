# 
# Makefile
# Created: Mon Jan  8 07:33:59 2001 by tek@wiw.org
# Revised: Tue Apr  3 16:58:13 2001 by tek@wiw.org
# Copyright 2001 Julian E. C. Squires (tek@wiw.org)
# This program comes with ABSOLUTELY NO WARRANTY.
# $Id$
# 
#

VPATH=$(TOPDIR)/demos
CFLAGS=$(DENT_CFLAGS)
LDFLAGS=$(DENT_LDFLAGS)

GENERATEDDEMOS=screenhacks
DEMOS=$(GENERATEDDEMOS)

default: $(DEMOS)

$(DEMOS): $(TOPDIR)/lib/libdentata.a

$(GENERATEDDEMOS): %: %.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -ldentata

clean:
	rm -f *~ *.o

realclean: clean
	rm -f $(GENERATEDDEMOS)

# EOF Makefile
