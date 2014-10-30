dnl Copyright (C) 2010 Free Software Foundation, Inc.
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.

dnl usage: ACX_LT_HOST_FLAGS([default_flags])
dnl Defines and AC_SUBSTs lt_host_flags


AC_DEFUN([ACX_LT_HOST_FLAGS], [
AC_REQUIRE([AC_CANONICAL_SYSTEM])

case $host in
  *-cygwin* | *-mingw* | *-msys*)
    # 'host' will be top-level target in the case of a target lib,
    # we must compare to with_cross_host to decide if this is a native
    # or cross-compiler and select where to install dlls appropriately.
    if test -n "$with_cross_host" &&
	test x"$with_cross_host" != x"no"; then
      lt_host_flags='-no-undefined -bindir "$(toolexeclibdir)"';
    else
      lt_host_flags='-no-undefined -bindir "$(bindir)"';
    fi
    ;;
  *)
    lt_host_flags=[$1]
    ;;
esac

AC_SUBST(lt_host_flags)
])
