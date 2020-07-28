# HLMTools/Makefile.  Generated from Makefile.in by configure.

# UNIX HTK LANGUAGE MODELLING TOOLS MAKEFILE

SHELL   =	/bin/sh
inc     = 	../HTKLib
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
hlib    = 	../HTKLib
llib    = 	../HLMLib
HLIBS   = 	$(hlib)/HTKLib.a $(llib)/HLMLib.a
CC      = 	gcc
CFLAGS  = 	-Wall -Wno-switch -g -O2 -I$(hlib) -I$(llib) 
LDFLAGS = 	 $(HLIBS) -lm
INSTALL = 	/usr/bin/install -c
PROGS   =	Cluster \
			HLMCopy \
			LAdapt \
			LBuild \
			LFoF \
			LGCopy \
			LGList \
			LGPrep \
			LLink \
			LMerge \
			LNewMap \
			LNorm \
			LPlex \
			LSubset 

all:  $(PROGS)

# build $(PROGS)
%: %.c $(HLIBS)
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
