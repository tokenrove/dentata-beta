#!/bin/sh
# 
# test-set.sh
# Created: Sat Feb 24 23:41:49 2001 by tek@wiw.org
# Revised: Sun Feb 25 00:10:29 2001 by tek@wiw.org
# Copyright 2001 Julian E. C. Squires (tek@wiw.org)
# This program comes with ABSOLUTELY NO WARRANTY.
# $Id$
# 
# Perform a series of tests on dentata's set module using the
# test-set program.
#
# FIXME: We currently don't test the effects of working with multiple
# sets in parallel, as test-set does not support it.
#

# Test 1
#
# Create a set, add 100 elements, check set integrity,
# remove 100 elements, delete the set.
./test-set -q -c n a100 c r100 d
if [ $? != 0 ]; then
    echo "test-set.sh: Test 1 failed!"
    exit 1
fi

# EOF test-set.sh
