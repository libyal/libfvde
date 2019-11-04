dnl Functions for libfplist
dnl
dnl Version: 20190308

dnl Function to detect if libfplist is available
dnl ac_libfplist_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFPLIST_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libfplist" = xno],
    [ac_cv_libfplist=no],
    [ac_cv_libfplist=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libfplist" != x && test "x$ac_cv_with_libfplist" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libfplist"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libfplist}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libfplist}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libfplist],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libfplist],
          [libfplist >= 20170919],
          [ac_cv_libfplist=yes],
          [ac_cv_libfplist=check])
        ])
      AS_IF(
        [test "x$ac_cv_libfplist" = xyes],
        [ac_cv_libfplist_CPPFLAGS="$pkg_cv_libfplist_CFLAGS"
        ac_cv_libfplist_LIBADD="$pkg_cv_libfplist_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libfplist" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libfplist.h])

      AS_IF(
        [test "x$ac_cv_header_libfplist_h" = xno],
        [ac_cv_libfplist=no],
        [dnl Check for the individual functions
        ac_cv_libfplist=yes
        AC_CHECK_LIB(
          fplist,
          libfplist_get_version,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])

        dnl Plist functions
        AC_CHECK_LIB(
          fplist,
          libfplist_property_list_initialize,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])
        AC_CHECK_LIB(
          fplist,
          libfplist_property_list_free,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])
        AC_CHECK_LIB(
          fplist,
          libfplist_property_list_copy_from_byte_stream,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])
        AC_CHECK_LIB(
          fplist,
          libfplist_property_list_has_plist_root_element,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])
        AC_CHECK_LIB(
          fplist,
          libfplist_property_list_get_root_property,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])

        dnl Property functions
        AC_CHECK_LIB(
          fplist,
          libfplist_property_free,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])
        AC_CHECK_LIB(
          fplist,
          libfplist_property_get_value_type,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])

        AC_CHECK_LIB(
          fplist,
          libfplist_property_get_value_data_size,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])
        AC_CHECK_LIB(
          fplist,
          libfplist_property_get_value_data,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])

        AC_CHECK_LIB(
          fplist,
          libfplist_property_get_value_integer,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])
        AC_CHECK_LIB(
          fplist,
          libfplist_property_get_value_string,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])

        AC_CHECK_LIB(
          fplist,
          libfplist_property_value_uuid_string_copy_to_byte_stream,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])

        AC_CHECK_LIB(
          fplist,
          libfplist_property_get_array_number_of_entries,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])
        AC_CHECK_LIB(
          fplist,
          libfplist_property_get_array_entry_by_index,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])

        AC_CHECK_LIB(
          fplist,
          libfplist_property_get_sub_property_by_utf8_name,
          [ac_cv_libfplist_dummy=yes],
          [ac_cv_libfplist=no])

        ac_cv_libfplist_LIBADD="-lfplist"
        ])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libfplist" = xyes],
    [AC_DEFINE(
      [HAVE_LIBFPLIST],
      [1],
      [Define to 1 if you have the `fplist' library (-lfplist).])
    ])

  AS_IF(
    [test "x$ac_cv_libfplist" = xyes],
    [AC_SUBST(
      [HAVE_LIBFPLIST],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBFPLIST],
      [0])
    ])
  ])

dnl Function to detect if libfplist dependencies are available
AC_DEFUN([AX_LIBFPLIST_CHECK_LOCAL],
  [AM_PROG_LEX
  AC_PROG_YACC

  ac_cv_libfplist_CPPFLAGS="-I../libfplist";
  ac_cv_libfplist_LIBADD="../libfplist/libfplist.la";

  ac_cv_libfplist=local
  ])

dnl Function to detect how to enable libfplist
AC_DEFUN([AX_LIBFPLIST_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libfplist],
    [libfplist],
    [search for libfplist in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  AX_LIBFPLIST_CHECK_LIB

  AS_IF(
    [test "x$ac_cv_libfplist" != xyes],
    [AX_LIBFPLIST_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBFPLIST],
      [1],
      [Define to 1 if the local version of libfplist is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBFPLIST],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBFPLIST],
    [test "x$ac_cv_libfplist" = xlocal])
  AS_IF(
    [test "x$ac_cv_libfplist_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBFPLIST_CPPFLAGS],
      [$ac_cv_libfplist_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libfplist_LIBADD" != "x"],
    [AC_SUBST(
      [LIBFPLIST_LIBADD],
      [$ac_cv_libfplist_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libfplist" = xyes],
    [AC_SUBST(
      [ax_libfplist_pc_libs_private],
      [-lfplist])
    ])

  AS_IF(
    [test "x$ac_cv_libfplist" = xyes],
    [AC_SUBST(
      [ax_libfplist_spec_requires],
      [libfplist])
    AC_SUBST(
      [ax_libfplist_spec_build_requires],
      [libfplist-devel])
    ])
  ])

