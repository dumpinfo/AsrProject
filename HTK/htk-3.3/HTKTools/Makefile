# HTKTools/Makefile.  Generated from Makefile.in by configure.

############################################################
# Makefile for HTKTools
# 
# N.B. you must make HTKLib.a (in the HTKLib directory first)
#
# "make" or "make all": to build all all binaries
# "make install": install all binaries in ${exec_prefix}/bin.i686
# "make clean": remove object files
# "make cleanup": remove object files and binaries
############################################################

SHELL   = 	/bin/sh
inc     = 	../HTKLib
HTKLIB  = 	$(inc)/HTKLib.a 
srcdir = .
top_srcdir = ..

prefix = /usr/local
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin.i686
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib.i686
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

CC      = 	gcc
CFLAGS  = 	-Wall -Wno-switch -g -O2 -I$(inc)
LDFLAGS = 	 -lm
INSTALL = 	/usr/bin/install -c
PROGS   = 	HBuild \
			HCopy \
			HDMan \
			HERest \
			HHEd \
			HInit \
			HLEd \
			HLRescore \
			HLStats \
			HList  \
			HParse \
			HQuant \
			HRest \
			HResults \
			HSGen \
			HSLab \
			HSmooth \
			HVite \
		  	HCompV  

all: $(PROGS)

# build $(PROGS)
HSLab: HSLab.c $(HTKLIB)
	if [ ! -d $(bindir) ] ; then mkdir $(bindir) ; fi
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS) -lX11
	$(INSTALL) -m 755 $@ $(bindir)

%: %.c $(HTKLIB) 
	if [ ! -d $(bindir) ] ; then mkdir $(bindir) ; fi
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS) 
	$(INSTALL) -m 755 $@ $(bindir)

strip: $(PROGS)
	-strip $(PROGS)

clean:
	-rm -f *.o 

cleanup:
	-rm -f *.o $(PROGS)

distclean:
	-rm -f *.o $(PROGS) Makefile

install: mkinstalldir $(PROGS)
	for program in $(PROGS) ; do $(INSTALL) -m 755 $${program} $(bindir) ; done

mkinstalldir:
	-mkdir -p $(bindir)

.PHONY: all strip clean cleanup distclean install mkinstalldir
