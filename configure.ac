AC_INIT([project], [0.1], [gauravjuvekar@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])


AC_PROG_CC
AC_PROG_CC_STDC

AM_PROG_AR

LT_INIT
AC_CONFIG_MACRO_DIRS([m4])
ACLOCAL_AMFLAGS=-I m4

PKG_CHECK_MODULES([GTK3],[gtk+-3.0 >= 3.10])
PKG_CHECK_MODULES([sqlite3],[sqlite3 >= 3.8],[HAVE_SQLITE3=1],[HAVE_SQLITE3=0])
AM_CONDITIONAL([USE_SQLITE3], [test "$HAVE_SQLITE3" -eq 1])

AC_CHECK_PROG([xxd],[xxd],[yes],[no])
AM_CONDITIONAL([FOUND_XXD],[test "x$xxd" = xyes])
AM_COND_IF([FOUND_XXD],,[AC_MSG_ERROR([required program 'xxd' not found. Please install it])])

AC_CONFIG_FILES([Makefile src/Makefile src/sqlite/Makefile])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADER([config.h])

AC_OUTPUT
