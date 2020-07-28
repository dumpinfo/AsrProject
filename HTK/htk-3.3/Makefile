# HTK Top level makefile
# Makefile.  Generated from Makefile.in by configure.

SHELL = /bin/sh
srcdir = .
top_srcdir = .

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
CPPFLAGS = 
CFLAGS = $(CPPFLAGS) -Wall -Wno-switch -g -O2
LDFLAGS = 
LIBS = -lm 
INSTALL = /usr/bin/install -c

DESTDIR =

pkgdatadir = $(datadir)/@PACKAGE@
pkglibdir = $(libdir)/@PACKAGE@
pkgincludedir = $(includedir)/@PACKAGE@

top_builddir = .

ACLOCAL = @ACLOCAL@
AUTOCONF = @AUTOCONF@
AUTOMAKE = @AUTOMAKE@
AUTOHEADER = @AUTOHEADER@

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
MAKEINFO = @MAKEINFO@
PACKAGE = @PACKAGE@
VERSION = @VERSION@

SUBDIRS = HTKLib HTKTools HLMLib HLMTools 
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_CLEAN_FILES = 
DIST_COMMON =  README ChangeLog Makefile.am \
Makefile.in aclocal.m4 configure configure.ac install-sh missing \
mkinstalldirs


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = gtar
GZIP_ENV = --best

.SUFFIXES:

all: 
	@for dir in $(SUBDIRS); do \
	  (cd $$dir && $(MAKE) all) \
	  || case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"
install:
	@for dir in $(SUBDIRS); do \
	  (cd $$dir && $(MAKE) install) \
	  || case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"
clean:
	/bin/rm -f *~
	@for dir in $(SUBDIRS); do \
	  (cd $$dir && $(MAKE) clean) \
	  || case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"
distclean: clean
	/bin/rm -f Makefile config.h config.status config.cache config.log
	@for dir in $(SUBDIRS); do \
	  (cd $$dir && $(MAKE) distclean) \
	  || case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"

.PHONY: all install clean distclean


