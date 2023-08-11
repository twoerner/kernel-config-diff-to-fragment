dnl Copyright (C) 2023  Trevor Woerner <twoerner@gmail.com>

dnl AC_PREREQ(2.57)
AC_INIT([kernel config diff to fragment], 0.1.1, twoerner@gmail.com, kernel-config-diff-to-fragment)
AC_CONFIG_SRCDIR(src/kernel-config-diff-to-fragment.c)
AC_CONFIG_AUX_DIR(cfg)
AC_CONFIG_MACRO_DIRS([cfg])
AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-xz])
AM_CONFIG_HEADER(cfg/config.h)

SUBDIRS="src"

dnl **********************************
dnl checks for programs
dnl **********************************
AC_PROG_CC
AC_PROG_CPP

AC_PROG_MAKE_SET

AC_PROG_INSTALL
AC_PROG_LN_S

dnl **********************************
dnl checks for header files
dnl **********************************
AC_HEADER_STDC
AC_CHECK_HEADERS(stdio.h string.h stdlib.h)
AC_CHECK_HEADERS(errno.h unistd.h getopt.h)

dnl **********************************
dnl output
dnl **********************************
AC_SUBST(SUBDIRS)
AC_OUTPUT(Makefile
cfg/Makefile
src/Makefile)
