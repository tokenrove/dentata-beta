# 
# Makefile.tst
# Created: Mon Jan  8 07:33:59 2001 by tek@wiw.org
# Revised: Tue Jun 26 20:53:10 2001 by tek@wiw.org
# Copyright 2001 Julian E. C. Squires (tek@wiw.org)
# This program comes with ABSOLUTELY NO WARRANTY.
# $Id$
# 
#

TESTTOOLS=./tests/test-set
GENERATEDTESTS=./tests/memcptst ./tests/imagetst ./tests/colortst
TESTS=./tests/test-set.sh
PROFILES=

check: $(TESTTOOLS) $(TESTS) $(GENERATEDTESTS)
profile: $(PROFILES)

$(TESTS) $(PROFILES): libdentata.a
	./$@

$(GENERATEDTESTS): %: %.o libdentata.a
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -ldentata
	./$@

$(TESTTOOLS): %: %.o libdentata.a
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -ldentata

testsclean:
	$(RM) ./tests/*~ ./tests/*.o

testsdistclean: testsclean
	$(RM) $(GENERATEDTESTS) $(TESTTOOLS)

# EOF Makefile.tst
