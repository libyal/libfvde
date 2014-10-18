dnl Functions for libhmac
dnl
dnl Version: 20130714

dnl Function to detect if libhmac is available
dnl ac_libhmac_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBHMAC_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libhmac" != x && test "x$ac_cv_with_libhmac" != xno && test "x$ac_cv_with_libhmac" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libhmac"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libhmac}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libhmac}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libhmac])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libhmac" = xno],
  [ac_cv_libhmac=no],
  [dnl Check for a pkg-config file
  AS_IF(
   [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
   [PKG_CHECK_MODULES(
    [libhmac],
    [libhmac >= 20130714],
    [ac_cv_libhmac=yes],
    [ac_cv_libhmac=no])
   ])

  AS_IF(
   [test "x$ac_cv_libhmac" = xyes],
   [ac_cv_libhmac_CPPFLAGS="$pkg_cv_libhmac_CFLAGS"
   ac_cv_libhmac_LIBADD="$pkg_cv_libhmac_LIBS"],
   [dnl Check for headers
   AC_CHECK_HEADERS([libhmac.h])

   AS_IF(
    [test "x$ac_cv_header_libhmac_h" = xno],
    [ac_cv_libhmac=no],
    [dnl Check for the individual functions
    ac_cv_libhmac=yes

    AC_CHECK_LIB(
     hmac,
     libhmac_get_version,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])

    dnl MD5 functions
    AC_CHECK_LIB(
     hmac,
     libhmac_md5_initialize,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])
    AC_CHECK_LIB(
     hmac,
     libhmac_md5_update,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])
    AC_CHECK_LIB(
     hmac,
     libhmac_md5_finalize,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])
    AC_CHECK_LIB(
     hmac,
     libhmac_md5_free,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])

    dnl SHA1 functions
    AC_CHECK_LIB(
     hmac,
     libhmac_sha1_initialize,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])
    AC_CHECK_LIB(
     hmac,
     libhmac_sha1_update,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])
    AC_CHECK_LIB(
     hmac,
     libhmac_sha1_finalize,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])
    AC_CHECK_LIB(
     hmac,
     libhmac_sha1_free,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])

    dnl SHA256 functions
    AC_CHECK_LIB(
     hmac,
     libhmac_sha256_initialize,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])
    AC_CHECK_LIB(
     hmac,
     libhmac_sha256_update,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])
    AC_CHECK_LIB(
     hmac,
     libhmac_sha256_finalize,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])
    AC_CHECK_LIB(
     hmac,
     libhmac_sha256_free,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])

    dnl SHA512 functions
    AC_CHECK_LIB(
     hmac,
     libhmac_sha512_initialize,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])
    AC_CHECK_LIB(
     hmac,
     libhmac_sha512_update,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])
    AC_CHECK_LIB(
     hmac,
     libhmac_sha512_finalize,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])
    AC_CHECK_LIB(
     hmac,
     libhmac_sha512_free,
     [ac_cv_libhmac_dummy=yes],
     [ac_cv_libhmac=no])

    ac_cv_libhmac_LIBADD="-lhmac"
    ])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libhmac" = xyes],
  [AC_DEFINE(
   [HAVE_LIBHMAC],
   [1],
   [Define to 1 if you have the `hmac' library (-lhmac).])
  ])

 AS_IF(
  [test "x$ac_cv_libhmac" = xyes],
  [ac_cv_libhmac_md5=libhmac
  ac_cv_libhmac_sha1=libhmac
  ac_cv_libhmac_sha256=libhmac
  ac_cv_libhmac_sha512=libhmac],
  [ac_cv_libhmac_md5=no
  ac_cv_libhmac_sha1=no
  ac_cv_libhmac_sha256=no
  ac_cv_libhmac_sha512=no])

 AS_IF(
  [test "x$ac_cv_libhmac" = xyes],
  [AC_SUBST(
   [HAVE_LIBHMAC],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBHMAC],
   [0])
  ])
 ])

dnl Function to detect if libhmac dependencies are available
AC_DEFUN([AX_LIBHMAC_CHECK_LOCAL],
 [ac_cv_libhmac_md5=no
 ac_cv_libhmac_sha1=no
 ac_cv_libhmac_sha256=no
 ac_cv_libhmac_sha512=no

 dnl Check for Windows crypto API support
 AS_IF(
  [test "x$ac_cv_enable_winapi" = xyes],
  [AX_WINCRYPT_CHECK_LIB

  AS_IF(
   [test "x$ac_cv_wincrypt" != xno],
   [AX_WINCRYPT_CHECK_MD5
   AX_WINCRYPT_CHECK_SHA1
   AX_WINCRYPT_CHECK_SHA256
   AX_WINCRYPT_CHECK_SHA512

   ac_cv_libhmac_md5=$ac_cv_wincrypt_md5
   ac_cv_libhmac_sha1=$ac_cv_wincrypt_sha1
   ac_cv_libhmac_sha256=$ac_cv_wincrypt_sha256
   ac_cv_libhmac_sha521=$ac_cv_wincrypt_sha521
  ])
 ])

 dnl Check for libcrypto (openssl) support
 AS_IF(
  [test "x$ac_cv_libhmac_md5" = xno && test "x$ac_cv_libhmac_sha1" = xno && test "x$ac_cv_libhmac_sha256" = xno && test "x$ac_cv_libhmac_sha512" = xno],
  [AX_LIBCRYPTO_CHECK_ENABLE

  AS_IF(
   [test "x$ac_cv_libcrypto" != xno],
   [AX_LIBCRYPTO_CHECK_MD5
   AX_LIBCRYPTO_CHECK_SHA1
   AX_LIBCRYPTO_CHECK_SHA256
   AX_LIBCRYPTO_CHECK_SHA512

   ac_cv_libhmac_md5=$ac_cv_libcrypto_md5
   ac_cv_libhmac_sha1=$ac_cv_libcrypto_sha1
   ac_cv_libhmac_sha256=$ac_cv_libcrypto_sha256
   ac_cv_libhmac_sha512=$ac_cv_libcrypto_sha512
  ])
 ])

 dnl Fallback to local versions if necessary
 ac_cv_libhmac_CPPFLAGS="-I../libhmac";
 ac_cv_libhmac_LIBADD="../libhmac/libhmac.la";

 ac_cv_libhmac=local

 AS_IF(
  [test "x$ac_cv_libhmac_md5" = xno],
  [ac_cv_libhmac_md5=local])

 AS_IF(
  [test "x$ac_cv_libhmac_sha1" = xno],
  [ac_cv_libhmac_sha1=local])

 AS_IF(
  [test "x$ac_cv_libhmac_sha256" = xno],
  [ac_cv_libhmac_sha256=local])

 AS_IF(
  [test "x$ac_cv_libhmac_sha512" = xno],
  [ac_cv_libhmac_sha512=local])
 ])

dnl Function to detect how to enable libhmac
AC_DEFUN([AX_LIBHMAC_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libhmac],
  [libhmac],
  [search for libhmac in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 dnl Check for a shared library version
 AX_LIBHMAC_CHECK_LIB

 dnl Check if the dependencies for the local library version
 AS_IF(
  [test "x$ac_cv_libhmac" != xyes],
  [AX_LIBHMAC_CHECK_LOCAL

  AC_DEFINE(
   [HAVE_LOCAL_LIBHMAC],
   [1],
   [Define to 1 if the local version of libhmac is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBHMAC],
   [1])
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBHMAC],
  [test "x$ac_cv_libhmac" = xlocal])
 AS_IF(
  [test "x$ac_cv_libhmac_CPPFLAGS" != "x"],
  [AC_SUBST(
   [LIBHMAC_CPPFLAGS],
   [$ac_cv_libhmac_CPPFLAGS])
  ])
 AS_IF(
  [test "x$ac_cv_libhmac_LIBADD" != "x"],
  [AC_SUBST(
   [LIBHMAC_LIBADD],
   [$ac_cv_libhmac_LIBADD])
  ])

 AS_IF(
  [test "x$ac_cv_libhmac" = xyes],
  [AC_SUBST(
   [ax_libhmac_pc_libs_private],
   [-lhmac])
  ])

 AS_IF(
  [test "x$ac_cv_libhmac" = xyes],
  [AC_SUBST(
   [ax_libhmac_spec_requires],
   [libhmac])
  AC_SUBST(
   [ax_libhmac_spec_build_requires],
   [libhmac-devel])
  ])
 ])

