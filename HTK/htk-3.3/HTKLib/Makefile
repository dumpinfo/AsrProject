# HTKLib/Makefile.  Generated from Makefile.in by configure.

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

hlib = .
HLIBS = ${exec_prefix}/lib/HTKLib.a

CC     = gcc
CFLAGS = -Wall -Wno-switch -g -O2 -I. 
RANLIB = ranlib
INSTALL= /usr/bin/install -c
target = HTKLib.a
modules = HShell.o HMath.o HSigP.o HWave.o HAudio.o HParm.o HVQ.o \
          HGraf.o HLabel.o HModel.o HUtil.o HTrain.o HDict.o HLM.o \
          HRec.o HNet.o HAdapt.o HFB.o HMem.o HLat.o HMap.o \
          esignal.o esig_asc.o esig_edr.o \
          esig_nat.o strarr.o

all:    HTKLib.a

$(target): $(modules)
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
