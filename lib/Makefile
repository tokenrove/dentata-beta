# 
# Makefile
# Created: Mon Jan  8 07:18:08 2001 by tek@wiw.org
# Revised: Sat May 19 12:37:30 2001 by tek@wiw.org
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

### system dependant stuff
SYSTEMMODS=memory_stdlib.o error_stdlib.o file_stdlib.o time_unix.o \
           audio_oss.o random_stdlib.o

### generic stuff
GENERICMODS=error_stack.o set.o util.o font.o sample.o wav.o s3m.o

### image related modules
IMAGEMODS=color_rgb.o image.o sprite.o tga.o pcx.o blit_common.o \
          collide_common.o collide_generic.o alphacomp_generic.o

#BLITMODS=blit_x86.o
BLITMODS=blit_generic.o

### event
ifeq ($(TARGET),svgalib)
	EVENTMODS=event_svgalib.o
endif
ifeq ($(TARGET),x11)
	EVENTMODS=event_x11.o
endif
#default
ifndef EVENTMODS
	EVENTMODS=event_null.o
endif

### libdentata

VPATH=$(TOPDIR)/src
CFLAGS=$(DENT_CFLAGS)
MODULES=$(RASTERMODS) $(EVENTMODS) $(SYSTEMMODS) $(IMAGEMODS) $(GENERICMODS) \
	$(BLITMODS)

libdentata.a: $(MODULES)
	ar rcs libdentata.a $(MODULES)

clean:
	rm -f *.o
	rm -f *~ $(TOPDIR)/src/*~ $(TOPDIR)/include/dentata/*~

realclean: clean
	rm -f libdentata.a

# EOF Makefile#
