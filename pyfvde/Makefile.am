if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFVDE_DLL_IMPORT@

pyexec_LTLIBRARIES = pyfvde.la

pyfvde_la_SOURCES = \
	pyfvde.c pyfvde.h \
	pyfvde_datetime.c pyfvde_datetime.h \
	pyfvde_error.c pyfvde_error.h \
	pyfvde_file_object_io_handle.c pyfvde_file_object_io_handle.h \
	pyfvde_file_objects_io_pool.c pyfvde_file_objects_io_pool.h \
	pyfvde_guid.c pyfvde_guid.h \
	pyfvde_integer.c pyfvde_integer.h \
	pyfvde_libbfio.h \
	pyfvde_libcerror.h \
	pyfvde_libclocale.h \
	pyfvde_libfguid.h \
	pyfvde_libfvde.h \
	pyfvde_logical_volume.c pyfvde_logical_volume.h \
	pyfvde_logical_volumes.c pyfvde_logical_volumes.h \
	pyfvde_physical_volume.c pyfvde_physical_volume.h \
	pyfvde_physical_volumes.c pyfvde_physical_volumes.h \
	pyfvde_python.h \
	pyfvde_unused.h \
	pyfvde_volume.c pyfvde_volume.h \
	pyfvde_volume_group.c pyfvde_volume_group.h

pyfvde_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libfvde/libfvde.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFGUID_LIBADD@

pyfvde_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pyfvde_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

