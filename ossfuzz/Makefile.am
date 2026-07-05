if HAVE_LIB_FUZZING_ENGINE
AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@

bin_PROGRAMS = \
	logical_volume_fuzzer \
	volume_fuzzer \
	volume_group_fuzzer

logical_volume_fuzzer_SOURCES = \
	logical_volume_fuzzer.cc \
	ossfuzz_libbfio.h \
	ossfuzz_libfvde.h

logical_volume_fuzzer_LDADD = \
	@LIB_FUZZING_ENGINE@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfvde/libfvde.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

volume_fuzzer_SOURCES = \
	ossfuzz_libbfio.h \
	ossfuzz_libfvde.h \
	volume_fuzzer.cc

volume_fuzzer_LDADD = \
	@LIB_FUZZING_ENGINE@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfvde/libfvde.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

volume_group_fuzzer_SOURCES = \
	ossfuzz_libbfio.h \
	ossfuzz_libfvde.h \
	volume_group_fuzzer.cc

volume_group_fuzzer_LDADD = \
	@LIB_FUZZING_ENGINE@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfvde/libfvde.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@
endif

DISTCLEANFILES = \
	Makefile \
	Makefile.in

splint-local:
	@echo "Running splint on logical_volume_fuzzer ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(logical_volume_fuzzer_SOURCES)
	@echo "Running splint on volume_fuzzer ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(volume_fuzzer_SOURCES)
	@echo "Running splint on volume_group_fuzzer ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(volume_group_fuzzer_SOURCES)

