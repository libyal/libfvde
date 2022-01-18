AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@LIBCAES_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@LIBFVDE_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	fvdeinfo \
	fvdemount \
	fvdewipekey

fvdeinfo_SOURCES = \
	byte_size_string.c byte_size_string.h \
	fvdeinfo.c \
	fvdetools_getopt.c fvdetools_getopt.h \
	fvdetools_i18n.h \
	fvdetools_input.c fvdetools_input.h \
	fvdetools_libbfio.h \
	fvdetools_libcdata.h \
	fvdetools_libcerror.h \
	fvdetools_libclocale.h \
	fvdetools_libcnotify.h \
	fvdetools_libcsplit.h \
	fvdetools_libfvde.h \
	fvdetools_libfguid.h \
	fvdetools_libuna.h \
	fvdetools_output.c fvdetools_output.h \
	fvdetools_signal.c fvdetools_signal.h \
	fvdetools_unused.h \
	info_handle.c info_handle.h

fvdeinfo_LDADD = \
	@LIBFGUID_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfvde/libfvde.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

fvdemount_SOURCES = \
	fvdemount.c \
	fvdetools_getopt.c fvdetools_getopt.h \
	fvdetools_i18n.h \
	fvdetools_input.c fvdetools_input.h \
	fvdetools_libbfio.h \
	fvdetools_libcdata.h \
	fvdetools_libcerror.h \
	fvdetools_libclocale.h \
	fvdetools_libcnotify.h \
	fvdetools_libcpath.h \
	fvdetools_libcsplit.h \
	fvdetools_libfvde.h \
	fvdetools_libuna.h \
	fvdetools_output.c fvdetools_output.h \
	fvdetools_signal.c fvdetools_signal.h \
	fvdetools_unused.h \
	mount_dokan.c mount_dokan.h \
	mount_file_entry.c mount_file_entry.h \
	mount_file_system.c mount_file_system.h \
	mount_fuse.c mount_fuse.h \
	mount_handle.c mount_handle.h

fvdemount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfvde/libfvde.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

fvdewipekey_SOURCES = \
	fvdewipekey.c \
	fvdetools_getopt.c fvdetools_getopt.h \
	fvdetools_i18n.h \
	fvdetools_libbfio.h \
	fvdetools_libcerror.h \
	fvdetools_libclocale.h \
	fvdetools_libcnotify.h \
	fvdetools_libuna.h \
	fvdetools_output.c fvdetools_output.h \
	fvdetools_signal.c fvdetools_signal.h \
	fvdetools_unused.h \
	wipekey_handle.c wipekey_handle.h

fvdewipekey_LDADD = \
	@LIBCAES_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfvde/libfvde.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on fvdeinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(fvdeinfo_SOURCES)
	@echo "Running splint on fvdemount ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(fvdemount_SOURCES)
	@echo "Running splint on fvdewipekey ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(fvdewipekey_SOURCES)

