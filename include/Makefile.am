include_HEADERS = \
	libfvde.h

pkginclude_HEADERS = \
	libfvde/codepage.h \
	libfvde/definitions.h \
	libfvde/error.h \
	libfvde/extern.h \
	libfvde/features.h \
	libfvde/types.h

EXTRA_DIST = \
	libfvde.h.in \
	libfvde/definitions.h.in \
	libfvde/features.h.in \
	libfvde/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libfvde.h
	/bin/rm -f libfvde/definitions.h
	/bin/rm -f libfvde/features.h
	/bin/rm -f libfvde/types.h
	/bin/rm -f Makefile

