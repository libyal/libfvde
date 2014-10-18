dnl Functions for libxml2
dnl
dnl Version: 20121217

dnl Function to detect if libxml2 is available
dnl ac_libxml2_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBXML2_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libxml2" != x && test "x$ac_cv_with_libxml2" != xno && test "x$ac_cv_with_libxml2" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libxml2"],
   [CPPFLAGS="$CPPFLAGS -I${ac_cv_with_libxml2}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libxml2}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libxml2])
   ])
  ])

 dnl Check for headers
 ac_cv_libxml2=no

 dnl Check for libxml2
 AS_IF(
  [test "x$ac_cv_with_libxml2" != xno],
  [dnl Use pkg-config if available
  AS_IF(
   [test "x$PKGCONFIG" != "x"],
   [PKG_CHECK_MODULES(
    [libxml2],
    [libxml-2.0 >= 2.0],
    [ac_cv_libxml2=libxml2])

   ac_cv_libxml2_CPPFLAGS="$pkg_cv_libxml2_CFLAGS"
   ac_cv_libxml2_LIBADD="$pkg_cv_libxml2_LIBS"
  ])
 ])

 AS_IF(
  [test "x$ac_cv_with_libxml2" != xno && test "x$ac_cv_libxml2" = xno],
  [AS_IF(
   [test "x$ac_cv_with_libxml2" = xauto-detect],
   [ac_cv_libxml2_CPPFLAGS="-I/usr/include/libxml2"
    ac_cv_libxml2_LDFLAGS="-lxml2"
   CPPFLAGS="$CPPFLAGS -I/usr/include/libxml2"],
   [AS_IF(
    [test "x$ac_cv_with_libxml2" != x],
    [ac_cv_libxml2_CPPFLAGS="${ac_cv_with_libxml2}/include"
    ac_cv_libxml2_LDFLAGS="-L${ac_cv_with_libxml2}/lib"])
   ])
  ac_cv_libxml2_LIBADD="-lxml2"

  ac_cv_header_libxml2_h=yes

  AC_CHECK_HEADERS(
   [libxml/xmlversion.h],
   [ac_cv_libxml2_dummy=yes],
   [ac_cv_header_libxml2_h=no])
  AC_CHECK_HEADERS(
   [libxml/parser.h],
   [ac_cv_libxml2_dummy=yes],
   [ac_cv_header_libxml2_h=no])
  AC_CHECK_HEADERS(
   [libxml/tree.h],
   [ac_cv_libxml2_dummy=yes],
   [ac_cv_header_libxml2_h=no])

  AS_IF(
   [test "x$ac_cv_header_libxml2_h" = xno],
   [ac_cv_libxml2=no],
   [ac_cv_libxml2=libxml2

   AC_CHECK_LIB(
    xml2,
    xmlCleanupParser,
    [ac_cv_libxml2_dummy=yes],
    [ac_cv_libxml2=no])

   AC_CHECK_LIB(
    xml2,
    xmlFree,
    [ac_cv_libxml2_dummy=yes],
    [ac_cv_libxml2=no])

   AC_CHECK_LIB(
    xml2,
    xmlDocGetRootElement,
    [ac_cv_libxml2_dummy=yes],
    [ac_cv_libxml2=no])

   AC_CHECK_LIB(
    xml2,
    xmlFreeDoc,
    [ac_cv_libxml2_dummy=yes],
    [ac_cv_libxml2=no])

   AC_CHECK_LIB(
    xml2,
    xmlStrcmp,
    [ac_cv_libxml2_dummy=yes],
    [ac_cv_libxml2=no])
   AC_CHECK_LIB(
    xml2,
    xmlStrlen,
    [ac_cv_libxml2_dummy=yes],
    [ac_cv_libxml2=no])

   AC_CHECK_LIB(
    xml2,
    xmlNodeGetContent,
    [ac_cv_libxml2_dummy=yes],
    [ac_cv_libxml2=no])

   AC_CHECK_LIB(
    xml2,
    xmlReadMemory,
    [ac_cv_libxml2_dummy=yes],
    [ac_cv_libxml2=no])
  ])
 ])

 AS_IF(
  [test "x$ac_cv_libxml2" = xlibxml2],
  [AC_SUBST(
   [LIBXML2_CPPFLAGS],
   [$ac_cv_libxml2_CPPFLAGS])
  AC_SUBST(
   [LIBXML2_LDFLAGS],
   [$ac_cv_libxml2_LDFLAGS])
  AC_SUBST(
   [LIBXML2_LIBADD],
   [$ac_cv_libxml2_LIBADD])
  AC_DEFINE(
   [HAVE_LIBXML2],
   [1],
   [Define to 1 if you have the 'xml2' library (-lxml2).])
  ])

 AS_IF(
  [test "x$ac_cv_libxml2" != xno],
  [AC_SUBST(
   [HAVE_LIBXML2],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBXML2],
   [0])
  ])
 ])

dnl Function to detect how to enable libxml2
AC_DEFUN([AX_LIBXML2_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libxml2],
  [libxml2],
  [search for libxml2 in includedir and libdir or in the specified DIR, or no if not to use libxml2],
  [auto-detect],
  [DIR])

 AX_LIBXML2_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libxml2" = xlibxml2],
  [AC_SUBST(
   [ax_libxml2_pc_libs_private],
   [-lxml2])
  ])

 AS_IF(
  [test "x$ac_cv_libxml2" = xlibxml2],
  [AC_SUBST(
   [ax_libxml2_spec_requires],
   [libxml2])
  AC_SUBST(
   [ax_libxml2_spec_build_requires],
   [libxml2-devel])
  ])
 ])

