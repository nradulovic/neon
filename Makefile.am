AUTOMAKE_OPTIONS = foreign

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = eds lib

lib_LTLIBRARIES = libneon.la
libneon_la_LDFLAGS = -version-info 2:0:0
libneon_la_SOURCES = 
libneon_la_LIBADD = \
	eds/libneoneds.la \
	lib/libneonlib.la

pkgconfig_DATA = neon.pc
