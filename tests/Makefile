# 
# Makefile
# Created: Mon Jan  8 07:33:59 2001 by tek@wiw.org
# Revised: Sun Feb 25 05:06:43 2001 by tek@wiw.org
# Copyright 2001 Julian E. C. Squires (tek@wiw.org)
# This program comes with ABSOLUTELY NO WARRANTY.
# $Id$
# 
#

VPATH=$(TOPDIR)/tests
CFLAGS=$(DENT_CFLAGS)
LDFLAGS=$(DENT_LDFLAGS)

GENERATEDTESTS=test-set test-tga
TESTS=test-set.sh
PROFILES=

.PHONY: clean realclean test profile default $(TESTS) $(PROFILES)

default: test

test: $(TESTS)
profile: $(PROFILES)

$(TESTS) $(PROFILES): $(TOPDIR)/lib/libdentata.a
	./$@

# dependencies for individual tests
test-set.sh: test-set

$(GENERATEDTESTS): %: %.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -ldentata

clean:
	rm -f *~ *.o

realclean: clean
	rm -f $(GENERATEDTESTS)

# EOF Makefile#