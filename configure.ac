AC_INIT([project], [0.1], [gauravjuvekar@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC
AC_PROG_CC_STDC

PKG_CHECK_MODULES(GTK3, gtk+-3.0 >= 3.10)

AC_CHECK_PROG([xxd],[xxd],[yes],[no])
AM_CONDITIONAL([FOUND_XXD], [test "x$xxd" = xyes])
AM_COND_IF([FOUND_XXD],,[AC_MSG_ERROR([required program 'xxd' not found.])])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_CONFIG_SRCDIR([src/main.c])

AC_OUTPUT
