AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBFPLIST_CPPFLAGS@ \
	@ZLIB_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@LIBCAES_CPPFLAGS@ \
	@LIBFVDE_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYFVDE = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_fvdeinfo.sh \
	$(TESTS_PYFVDE)

check_SCRIPTS = \
	pyfvde_test_support.py \
	pyfvde_test_volume.py \
	test_fvdeinfo.sh \
	test_library.sh \
	test_python_module.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	fvde_test_data_area_descriptor \
	fvde_test_encrypted_metadata \
	fvde_test_encryption_context_plist \
	fvde_test_error \
	fvde_test_io_handle \
	fvde_test_keyring \
	fvde_test_metadata \
	fvde_test_metadata_block \
	fvde_test_notify \
	fvde_test_sector_data \
	fvde_test_segment_descriptor \
	fvde_test_support \
	fvde_test_volume \
	fvde_test_volume_group

fvde_test_data_area_descriptor_SOURCES = \
	fvde_test_data_area_descriptor.c \
	fvde_test_libcerror.h \
	fvde_test_libfvde.h \
	fvde_test_macros.h \
	fvde_test_memory.c fvde_test_memory.h \
	fvde_test_unused.h

fvde_test_data_area_descriptor_LDADD = \
	../libfvde/libfvde.la \
	@LIBCERROR_LIBADD@

fvde_test_encrypted_metadata_SOURCES = \
	fvde_test_encrypted_metadata.c \
	fvde_test_libcerror.h \
	fvde_test_libfvde.h \
	fvde_test_macros.h \
	fvde_test_memory.c fvde_test_memory.h \
	fvde_test_unused.h

fvde_test_encrypted_metadata_LDADD = \
	../libfvde/libfvde.la \
	@LIBCERROR_LIBADD@

fvde_test_encryption_context_plist_SOURCES = \
	fvde_test_encryption_context_plist.c \
	fvde_test_libcerror.h \
	fvde_test_libfvde.h \
	fvde_test_macros.h \
	fvde_test_memory.c fvde_test_memory.h \
	fvde_test_unused.h

fvde_test_encryption_context_plist_LDADD = \
	../libfvde/libfvde.la \
	@LIBCERROR_LIBADD@

fvde_test_error_SOURCES = \
	fvde_test_error.c \
	fvde_test_libfvde.h \
	fvde_test_macros.h \
	fvde_test_unused.h

fvde_test_error_LDADD = \
	../libfvde/libfvde.la

fvde_test_io_handle_SOURCES = \
	fvde_test_io_handle.c \
	fvde_test_libcerror.h \
	fvde_test_libfvde.h \
	fvde_test_macros.h \
	fvde_test_memory.c fvde_test_memory.h \
	fvde_test_unused.h

fvde_test_io_handle_LDADD = \
	../libfvde/libfvde.la \
	@LIBCERROR_LIBADD@

fvde_test_keyring_SOURCES = \
	fvde_test_keyring.c \
	fvde_test_libcerror.h \
	fvde_test_libfvde.h \
	fvde_test_macros.h \
	fvde_test_memory.c fvde_test_memory.h \
	fvde_test_unused.h

fvde_test_keyring_LDADD = \
	../libfvde/libfvde.la \
	@LIBCERROR_LIBADD@

fvde_test_metadata_SOURCES = \
	fvde_test_libcerror.h \
	fvde_test_libfvde.h \
	fvde_test_macros.h \
	fvde_test_memory.c fvde_test_memory.h \
	fvde_test_metadata.c \
	fvde_test_unused.h

fvde_test_metadata_LDADD = \
	../libfvde/libfvde.la \
	@LIBCERROR_LIBADD@

fvde_test_metadata_block_SOURCES = \
	fvde_test_libcerror.h \
	fvde_test_libfvde.h \
	fvde_test_macros.h \
	fvde_test_memory.c fvde_test_memory.h \
	fvde_test_metadata_block.c \
	fvde_test_unused.h

fvde_test_metadata_block_LDADD = \
	../libfvde/libfvde.la \
	@LIBCERROR_LIBADD@

fvde_test_notify_SOURCES = \
	fvde_test_libcerror.h \
	fvde_test_libfvde.h \
	fvde_test_macros.h \
	fvde_test_notify.c \
	fvde_test_unused.h

fvde_test_notify_LDADD = \
	../libfvde/libfvde.la \
	@LIBCERROR_LIBADD@

fvde_test_sector_data_SOURCES = \
	fvde_test_libcerror.h \
	fvde_test_libfvde.h \
	fvde_test_macros.h \
	fvde_test_memory.c fvde_test_memory.h \
	fvde_test_sector_data.c \
	fvde_test_unused.h

fvde_test_sector_data_LDADD = \
	../libfvde/libfvde.la \
	@LIBCERROR_LIBADD@

fvde_test_segment_descriptor_SOURCES = \
	fvde_test_libcerror.h \
	fvde_test_libfvde.h \
	fvde_test_macros.h \
	fvde_test_memory.c fvde_test_memory.h \
	fvde_test_segment_descriptor.c \
	fvde_test_unused.h

fvde_test_segment_descriptor_LDADD = \
	../libfvde/libfvde.la \
	@LIBCERROR_LIBADD@

fvde_test_support_SOURCES = \
	fvde_test_functions.c fvde_test_functions.h \
	fvde_test_getopt.c fvde_test_getopt.h \
	fvde_test_libbfio.h \
	fvde_test_libcerror.h \
	fvde_test_libclocale.h \
	fvde_test_libcnotify.h \
	fvde_test_libfvde.h \
	fvde_test_libuna.h \
	fvde_test_macros.h \
	fvde_test_memory.c fvde_test_memory.h \
	fvde_test_support.c \
	fvde_test_unused.h

fvde_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfvde/libfvde.la \
	@LIBCERROR_LIBADD@

fvde_test_volume_SOURCES = \
	fvde_test_functions.c fvde_test_functions.h \
	fvde_test_getopt.c fvde_test_getopt.h \
	fvde_test_libbfio.h \
	fvde_test_libcerror.h \
	fvde_test_libclocale.h \
	fvde_test_libcnotify.h \
	fvde_test_libfvde.h \
	fvde_test_libuna.h \
	fvde_test_macros.h \
	fvde_test_memory.c fvde_test_memory.h \
	fvde_test_unused.h \
	fvde_test_volume.c

fvde_test_volume_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfvde/libfvde.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

fvde_test_volume_group_SOURCES = \
	fvde_test_libcerror.h \
	fvde_test_libfvde.h \
	fvde_test_macros.h \
	fvde_test_memory.c fvde_test_memory.h \
	fvde_test_unused.h \
	fvde_test_volume_group.c

fvde_test_volume_group_LDADD = \
	../libfvde/libfvde.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

