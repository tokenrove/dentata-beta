# 
# Makefile
# Created: Mon Jan  8 07:18:08 2001 by tek@wiw.org
# Revised: Sat Feb 24 19:59:02 2001 by tek@wiw.org
# Copyright 2001 Julian E. C. Squires (tek@wiw.org)
# This program comes with ABSOLUTELY NO WARRANTY.
# $Id$
# 
#

### raster
ifeq ($(TARGET),x11)
	RASTERMODS=raster_x11.o
endif
ifeq ($(TARGET),svgalib)
	RASTERMODS=raster_svgalib.o
endif

# default
ifndef RASTERMODS
	RASTERMODS=raster_null.o
endif

### memory
MEMORYMODS=memory_stdlib.o

### error
ERRORMODS=error_stack.o error_stdlib.o

### color
COLORMODS=color_rgb.o

### sprite
SPRITEMODS=sprite.o

### libdentata

VPATH=$(TOPDIR)/src
CFLAGS=$(DENT_CFLAGS)
MODULES=$(RASTERMODS) $(MEMORYMODS) $(ERRORMODS) $(COLORMODS) $(SPRITEMODS)

libdentata.a: $(MODULES)
	ar rcs libdentata.a $(MODULES)

clean:
	rm -f *.o
	rm -f *~ $(TOPDIR)/src/*~ $(TOPDIR)/include/dentata/*~

realclean: clean
	rm -f libdentata.a

# EOF Makefile#