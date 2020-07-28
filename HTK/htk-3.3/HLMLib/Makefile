# HLMLib/Makefile.  Generated from Makefile.in by configure.

SHELL = /bin/sh
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

CC = gcc
CFLAGS = -Wall -Wno-switch -g -O2 -DSANITY -I. -I../HTKLib/
RANLIB = ranlib
INSTALL = /usr/bin/install -c
target  = HLMLib.a
modules = LModel.o LPMerge.o LPCalc.o LUtil.o LWMap.o LCMap.o LGBase.o

all: $(target)

$(target):	$(modules)
	if [ -f $@ ] ; then  /bin/rm $@ ; fi
	ar rv $@ $^
	$(RANLIB) $@

.PHONY: clean cleanup depend mkinstalldir install

clean:
	-rm -f $(modules) $(target)

cleanup:
	-rm -f $(modules)

depend:
	-makedepend -Y *.c

distclean: clean
	-rm -f Makefile

install: $(target) mkinstalldir 
	$(INSTALL) -m 755 $< $(libdir)

mkinstalldir:
	-mkdir -p $(libdir)
