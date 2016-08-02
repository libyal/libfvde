dnl Functions for wincrypt
dnl
dnl Version: 20160802

dnl Function to detect whether a certain #define is present in a certain WINAPI header
AC_DEFUN([AX_WINCRYPT_CHECK_WINAPI_DEFINE],
 [AC_CACHE_CHECK(
  [whether $1 defines $2],
  [ac_cv_winapi_define_$2],
  [AC_LANG_PUSH(C)
  AC_COMPILE_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <windows.h>
#include <$1>

#if !defined( $2 )
#error $2 is not defined
#endif]]
    [[]] )],
   [ac_cv_winapi_define_$2=yes],
   [ac_cv_winapi_define_$2=no])
   AC_LANG_POP(C)
  ])
 ])

dnl Function to detect if wincrypt functions are available
AC_DEFUN([AX_WINCRYPT_CHECK_LIB],
 [AS_IF(
  [test "x$ac_cv_enable_winapi" != xyes],
  [ac_cv_wincrypt=no],
  [AC_CHECK_HEADERS(
   [wincrypt.h],
   [],
   [],
   [#include <windows.h>])

  AS_IF(
   [test "x$ac_cv_header_wincrypt" = xno],
   [ac_cv_wincrypt=no],
   [AC_SUBST(
    [LIBCRYPTO_LIBADD],
    [-ladvapi32])

   ac_cv_wincrypt=libadvapi32])
  ])

 AM_CONDITIONAL(
  [HAVE_WINCRYPT],
  [test "x$ac_cv_wincrypt" != xno])
 ])

dnl Function to detect if wincrypt MD5 functions are available
AC_DEFUN([AX_WINCRYPT_CHECK_MD5],
 [AX_WINCRYPT_CHECK_WINAPI_DEFINE(
  [wincrypt.h],
  [CALG_MD5])

 AS_IF(
  [test "x$ac_cv_winapi_define_CALG_MD5" = xyes],
  [ac_cv_wincrypt_md5=libadvapi32],
  [ac_cv_wincrypt_md5=local])
 ])

dnl Function to detect if wincrypt SHA1 functions are available
AC_DEFUN([AX_WINCRYPT_CHECK_SHA1],
 [AX_WINCRYPT_CHECK_WINAPI_DEFINE(
  [wincrypt.h],
  [CALG_SHA1])

 AS_IF(
  [test "x$ac_cv_winapi_define_CALG_SHA1" = xyes],
  [ac_cv_wincrypt_sha1=libadvapi32],
  [ac_cv_wincrypt_sha1=local])
 ])

dnl Function to detect if wincrypt SHA224 functions are available
AC_DEFUN([AX_WINCRYPT_CHECK_SHA224],
 [AX_WINCRYPT_CHECK_WINAPI_DEFINE(
  [wincrypt.h],
  [CALG_SHA_224])

 AS_IF(
  [test "x$ac_cv_winapi_define_CALG_SHA_224" = xyes],
  [ac_cv_wincrypt_sha224=libadvapi32],
  [ac_cv_wincrypt_sha224=local])
 ])

dnl Function to detect if wincrypt SHA256 functions are available
AC_DEFUN([AX_WINCRYPT_CHECK_SHA256],
 [AX_WINCRYPT_CHECK_WINAPI_DEFINE(
  [wincrypt.h],
  [CALG_SHA_256])

 AS_IF(
  [test "x$ac_cv_winapi_define_CALG_SHA_256" = xyes],
  [ac_cv_wincrypt_sha256=libadvapi32],
  [ac_cv_wincrypt_sha256=local])
 ])

dnl Function to detect if wincrypt SHA512 functions are available
AC_DEFUN([AX_WINCRYPT_CHECK_SHA512],
 [AX_WINCRYPT_CHECK_WINAPI_DEFINE(
  [wincrypt.h],
  [CALG_SHA_512])

 AS_IF(
  [test "x$ac_cv_winapi_define_CALG_SHA_512" = xyes],
  [ac_cv_wincrypt_sha512=libadvapi32],
  [ac_cv_wincrypt_sha512=local])
 ])

