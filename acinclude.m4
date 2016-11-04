dnl Function to detect if libfvde dependencies are available
AC_DEFUN([AX_LIBFVDE_CHECK_LOCAL],
 [dnl Check for internationalization functions in libfvde/libfvde_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

