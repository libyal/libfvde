AC_PREREQ( 2.59 )

AC_INIT(
 [libfvde],
 [20160729],
 [joachim.metz@gmail.com])

AC_CONFIG_SRCDIR(
 [include/libfvde.h.in])

AM_INIT_AUTOMAKE([gnu 1.6 tar-ustar])

AC_CONFIG_MACRO_DIR([m4])

dnl Check for host type
AC_CANONICAL_HOST

dnl Check for large file support
AC_SYS_LARGEFILE

dnl Check for libtool DLL support
AC_LIBTOOL_WIN32_DLL

dnl Checks for programs
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl Check for pkg-config
AC_PATH_PROG(PKGCONFIG,[pkg-config])

dnl Support of internationalization (i18n)
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.18.1])

dnl Check for compiler language support
AC_C_CONST
AC_C_VOLATILE

dnl Check if WINAPI support should be enabled
AX_COMMON_CHECK_ENABLE_WINAPI

dnl Check if wide character type should be enabled
AX_COMMON_CHECK_ENABLE_WIDE_CHARACTER_TYPE

dnl Check for type definitions
AX_TYPES_CHECK_LOCAL

dnl Checks for libraries and library functions
AX_COMMON_CHECK_LOCAL

dnl Check for enabling libcstring for cross-platform C string functions
AX_LIBCSTRING_CHECK_ENABLE

dnl Check for enabling libcerror for cross-platform C error functions
AX_LIBCERROR_CHECK_ENABLE

dnl Check for enabling libcthreads for cross-platform C threads functions
AX_LIBCTHREADS_CHECK_ENABLE

dnl Check for enabling libcdata for cross-platform C generic data functions
AX_LIBCDATA_CHECK_ENABLE

dnl Check for enabling libclocale for cross-platform C locale functions
AX_LIBCLOCALE_CHECK_ENABLE

dnl Check for enabling libcnotify for cross-platform C notification functions
AX_LIBCNOTIFY_CHECK_ENABLE

dnl Check for enabling libcsplit for cross-platform C split string functions
AX_LIBCSPLIT_CHECK_ENABLE

dnl Check for enabling libuna for Unicode string support
AX_LIBUNA_CHECK_ENABLE

dnl Check for enabling libcfile for cross-platform C file functions
AX_LIBCFILE_CHECK_ENABLE

dnl Check for enabling libcpath for cross-platform C path functions
AX_LIBCPATH_CHECK_ENABLE

dnl Check for enabling libbfio for basic file IO support
AX_LIBBFIO_CHECK_ENABLE

dnl Check for enabling libfcache for file value cache support
AX_LIBFCACHE_CHECK_ENABLE

dnl Check for enabling libfdata for file data types support
AX_LIBFDATA_CHECK_ENABLE

dnl Check for enabling libfguid for file GUID/UUID support
AX_LIBFGUID_CHECK_ENABLE

dnl Check for enabling libfvalue for file value support
AX_LIBFVALUE_CHECK_ENABLE

dnl Check for enabling libhmac for message digest hash function support
AX_LIBHMAC_CHECK_ENABLE

dnl Check for enabling libcaes for AES cipher function support
AX_LIBCAES_CHECK_ENABLE

dnl Check for library function support
AX_LIBFVDE_CHECK_LOCAL

dnl Check for enabling libcsystem for cross-platform C generic system functions
AX_LIBCSYSTEM_CHECK_ENABLE

dnl Check for libfuse Filesystem in Userspace (FUSE) support
AX_LIBFUSE_CHECK_ENABLE

dnl Check for zlib compression support
AX_ZLIB_CHECK_ENABLE
AX_ZLIB_CHECK_UNCOMPRESS

dnl Check for functions
AS_IF(
 [test "x$ac_cv_enable_winapi" = xno],
 [AC_HEADER_TIME
 AC_CHECK_FUNCS([getegid geteuid time])
 ])

dnl Check if fvdetools should be build as static executables
AX_COMMON_CHECK_ENABLE_STATIC_EXECUTABLES

dnl Check if libfvde should be build with verbose output
AX_COMMON_CHECK_ENABLE_VERBOSE_OUTPUT

dnl Check if libfvde should be build with debug output
AX_COMMON_CHECK_ENABLE_DEBUG_OUTPUT

dnl Check if libfvde python bindings (pyfvde) should be build
AX_PYTHON_CHECK_PYPREFIX
AX_PYTHON_CHECK_ENABLE
AX_PYTHON2_CHECK_ENABLE
AX_PYTHON3_CHECK_ENABLE

AM_CONDITIONAL(
 HAVE_PYTHON_TESTS,
 [test "x${ac_cv_enable_python}" != xno || test "x${ac_cv_enable_python2}" != xno || test "x${ac_cv_enable_python3}" != xno])

dnl Headers included in various places
AC_CHECK_HEADERS([stdarg.h varargs.h])

AS_IF(
 [test "x$ac_cv_header_stdarg_h" != xyes && test "x$ac_cv_header_varargs_h" != xyes],
 [AC_MSG_FAILURE(
  [Missing headers: stdarg.h and varargs.h],
  [1])
 ])

dnl Check if DLL support is needed
AS_IF(
 [test "x$enable_shared" = xyes],
 [AS_CASE(
  [$host],
  [*cygwin* | *mingw*],
  [AC_DEFINE(
   [HAVE_DLLMAIN],
   [1],
   [Define to 1 to enable the DllMain function.])
  AC_SUBST(
   [HAVE_DLLMAIN],
   [1])

  AS_IF(
   [test "x$ac_cv_enable_static_executables" = xno],
   [AC_SUBST(
    [LIBFVDE_DLL_IMPORT],
    ["-DLIBFVDE_DLL_IMPORT"])
   ])
  ])
 ])

dnl Set additional compiler flags
CFLAGS="$CFLAGS -Wall";

dnl Check if requires and build requires should be set in spec file
AS_IF(
 [test "x$ac_cv_libcstring" = xyes || test "x$ac_cv_libcerror" = xyes || test "x$ac_cv_libcthreads" = xyes || test "x$ac_cv_libcdata" = xyes || test "x$ac_cv_libclocale" = xyes || test "x$ac_cv_libcnotify" = xyes  || test "x$ac_cv_libcsplit" = xyes || test "x$ac_cv_libuna" = xyes || test "x$ac_cv_libcfile" = xyes || test "x$ac_cv_libcpath" = xyes || test "x$ac_cv_libbfio" = xyes || test "x$ac_cv_libfdata" = xyes || test "x$ac_cv_libfguid" = xyes || test "x$ac_cv_libfvalue" = xyes || test "x$ac_cv_zlib" != xno || test "x$ac_cv_libcaes" = xyes || test "x$ac_cv_libhmac" = xyes || test "x$ac_cv_libcrypto" = xyes],
 [AC_SUBST(
  [libfvde_spec_requires],
  [Requires:])
 AC_SUBST(
  [libfvde_spec_build_requires],
  [BuildRequires:])
 ])

AS_IF(
 [test "x$ac_cv_libcsystem" = xyes || test "x$ac_cv_libfuse" != xno],
 [AC_SUBST(
  [libfvde_spec_tools_build_requires],
  [BuildRequires:])
 ])

dnl Set the date for the dpkg files
AC_SUBST(
 [DPKG_DATE],
 [`date -R 2> /dev/null`])

dnl Set the date for the spec file
AC_SUBST(
 [SPEC_DATE],
 [`date +"%a %b %e %Y" 2> /dev/null`])

dnl Set the current working directory for PackageMaker files
AC_SUBST(
 [PWD],
 [`pwd`])

dnl Have configure make the Makefiles
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([include/Makefile])
AC_CONFIG_FILES([common/Makefile])
AC_CONFIG_FILES([libcstring/Makefile])
AC_CONFIG_FILES([libcerror/Makefile])
AC_CONFIG_FILES([libcthreads/Makefile])
AC_CONFIG_FILES([libcdata/Makefile])
AC_CONFIG_FILES([libclocale/Makefile])
AC_CONFIG_FILES([libcnotify/Makefile])
AC_CONFIG_FILES([libcsplit/Makefile])
AC_CONFIG_FILES([libuna/Makefile])
AC_CONFIG_FILES([libcfile/Makefile])
AC_CONFIG_FILES([libcpath/Makefile])
AC_CONFIG_FILES([libbfio/Makefile])
AC_CONFIG_FILES([libfcache/Makefile])
AC_CONFIG_FILES([libfdata/Makefile])
AC_CONFIG_FILES([libfguid/Makefile])
AC_CONFIG_FILES([libfvalue/Makefile])
AC_CONFIG_FILES([libhmac/Makefile])
AC_CONFIG_FILES([libcaes/Makefile])
AC_CONFIG_FILES([libfvde/Makefile])
AC_CONFIG_FILES([libcsystem/Makefile])
AC_CONFIG_FILES([fvdetools/Makefile])
AC_CONFIG_FILES([pyfvde/Makefile])
AC_CONFIG_FILES([pyfvde-python2/Makefile])
AC_CONFIG_FILES([pyfvde-python3/Makefile])
AC_CONFIG_FILES([po/Makefile.in])
AC_CONFIG_FILES([po/Makevars])
AC_CONFIG_FILES([manuals/Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([msvscpp/Makefile])
dnl Have configure make the include files
AC_CONFIG_FILES([include/libfvde.h])
AC_CONFIG_FILES([include/libfvde/definitions.h])
AC_CONFIG_FILES([include/libfvde/features.h])
AC_CONFIG_FILES([include/libfvde/types.h])
AC_CONFIG_FILES([libfvde/libfvde_definitions.h])
dnl Have configure make distribution specific files
AC_CONFIG_FILES([common/types.h])
AC_CONFIG_FILES([dpkg/changelog])
AC_CONFIG_FILES([dpkg/copyright])
AC_CONFIG_FILES([libfvde/libfvde.rc])
AC_CONFIG_FILES([libfvde.pc])
AC_CONFIG_FILES([libfvde.spec])
dnl Have configure write its configuration
AC_CONFIG_HEADERS([common/config.h])

AC_OUTPUT

dnl Print a summary
AC_MSG_NOTICE([
Building:
   libcstring support:                        $ac_cv_libcstring
   libcerror support:                         $ac_cv_libcerror
   libcthreads support:                       $ac_cv_libcthreads
   libcdata support:                          $ac_cv_libcdata
   libclocale support:                        $ac_cv_libclocale
   libcnotify support:                        $ac_cv_libcnotify
   libcsplit support:                         $ac_cv_libcsplit
   libuna support:                            $ac_cv_libuna
   libcfile support:                          $ac_cv_libcfile
   libcpath support:                          $ac_cv_libcpath
   libbfio support:                           $ac_cv_libbfio
   libfcache support:                         $ac_cv_libfcache
   libfdata support:                          $ac_cv_libfdata
   libfguid support:                          $ac_cv_libfguid
   libfvalue support:                         $ac_cv_libfvalue
   DEFLATE compression support:               $ac_cv_uncompress
   libhmac support:                           $ac_cv_libhmac
   SHA256 support:                            $ac_cv_libhmac_sha256
   libcaes support:                           $ac_cv_libcaes
   AES support:                               $ac_cv_libcaes_aes
   libcsystem support:                        $ac_cv_libcsystem
   FUSE support:                              $ac_cv_libfuse

Features:
   Multi-threading support:                   $ac_cv_libcthreads_multi_threading
   Wide character type support:               $ac_cv_enable_wide_character_type
   fvdetools are build as static executables: $ac_cv_enable_static_executables
   Python (pyfvde) support:                   $ac_cv_enable_python
   Python version 2 (pyfvde) support:         $ac_cv_enable_python2
   Python version 3 (pyfvde) support:         $ac_cv_enable_python3
   Verbose output:                            $ac_cv_enable_verbose_output
   Debug output:                              $ac_cv_enable_debug_output
]);

