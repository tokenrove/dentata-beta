# 
# Makefile
# Created: Mon Jan  8 07:33:59 2001 by tek@wiw.org
# Revised: Sat Feb 24 19:14:50 2001 by tek@wiw.org
# Copyright 2001 Julian E. C. Squires (tek@wiw.org)
# This program comes with ABSOLUTELY NO WARRANTY.
# $Id$
# 
#

GENERATEDTESTS=
TESTS=$(GENERATEDTESTS)
GENERATEDPROFILES=
PROFILES=$(GENERATEDPROFILES)

.PHONY: clean realclean test profile default

default: test

test: $(TESTS)
profile: $(PROFILES)

$(TESTS) $(PROFILES): $(TOPDIR)/lib/libdentata.a
	./$@

clean:
	rm -f *~ *.o

realclean: clean
	rm -f $(GENERATEDTESTS)

# EOF Makefile#