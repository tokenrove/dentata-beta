# 
# Makefile.tst
# Created: Mon Jan  8 07:33:59 2001 by tek@wiw.org
# Revised: Sat Jun 23 04:16:21 2001 by tek@wiw.org
# Copyright 2001 Julian E. C. Squires (tek@wiw.org)
# This program comes with ABSOLUTELY NO WARRANTY.
# $Id$
# 
#

TESTTOOLS=./tests/test-set
GENERATEDTESTS=./tests/test-img
TESTS=./tests/test-set.sh
PROFILES=

check: $(TESTTOOLS) $(TESTS) $(GENERATEDTESTS)
profile: $(PROFILES)

$(TESTS) $(PROFILES): libdentata.a
	./$@

$(GENERATEDTESTS): %: %.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -ldentata
	./$@

$(TESTTOOLS): %: %.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -ldentata

testsclean:
	rm -f ./tests/*~ ./tests/*.o

testsdistclean: testsclean
	rm -f $(GENERATEDTESTS) $(TESTTOOLS)

# EOF Makefile.tst
