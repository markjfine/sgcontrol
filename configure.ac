dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])

AC_INIT([sgcontrol], [0.53], [mark.fine@fineware-swl.com],[sgcontrol],[https://github.com/markjfine/sgcontrol])

AC_CONFIG_SRCDIR([configure.ac])

AM_INIT_AUTOMAKE([-Wall subdir-objects])

AC_CONFIG_HEADERS(config.h)

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AM_MAINTAINER_MODE

AC_CONFIG_MACRO_DIR([macros])

dnl check for programs
AC_PROG_CC
AC_SEARCH_LIBS([strerror],[cposix])
AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

dnl LT_INIT

dnl LT_PREREQ([2.2.6b])
dnl LT_INIT([win32-dll])

AC_CHECK_HEADERS([sys/time.h sys/types.h sys/stat.h ctype.h getopt.h math.h string.h time.h unistd.h])

if test "${ac_cv_c_compiler_gnu}" = "yes"; then
  CFLAGS="${CFLAGS} -Wall"
fi

pkg_modules="gtk+-3.0 >= 3.24"
PKG_CHECK_MODULES(PACKAGE, [$pkg_modules])
AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)

dnl ALL_LINGUAS=""
dnl The gettext domain of the library
dnl GETTEXT_PACKAGE=${PACKAGE}
dnl AM_GNU_GETTEXT([external])
dnl AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE",[The gettext domain])
dnl AC_SUBST(GETTEXT_PACKAGE)
dnl AM_GNU_GETTEXT_VERSION([0.23.1])
dnl AM_XGETTEXT_OPTION([--from-code=UTF-8]) 

# Check intltool
#IT_PROG_INTLTOOL([0.33], [no-xml])

AC_CHECK_LIB([m], [sincos])

dnl Check hamlib
hamlib_modules="hamlib >= 4.6"
PKG_CHECK_MODULES(HAMLIB, [$hamlib_modules], [
  CFLAGS="$CFLAGS $HAMLIB_CFLAGS";
  LIBS="$LIBS $HAMLIB_LIBS";
], [
  AC_MSG_ERROR([Hamradio control libraries 4.6 or later not found...])
])

dnl Check curl
libcurl_modules="libcurl >= 7.60"
PKG_CHECK_MODULES(LIBCURL, [$libcurl_modules], [
  CFLAGS="$CFLAGS $LIBCURL_CFLAGS";
  LIBS="$LIBS $LIBCURL_LIBS";
], [
  AC_MSG_ERROR([libcurl 7.60 or later not found...])
])

GLIB_GSETTINGS

dnl store library versions
CURL_V=`pkg-config --modversion libcurl`
HAMLIB_V=`pkg-config --modversion hamlib`
GLIB_V=`pkg-config --modversion glib-2.0`
GDK_V=`pkg-config --modversion gdk-3.0`
GTK_V=`pkg-config --modversion gtk+-3.0`

AC_DEFINE_UNQUOTED([HAMLIB_VERSION],[`pkg-config --modversion hamlib`],["Hamlib version"])


AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)

dnl AC_PROG_INSTALL

dnl Pick up the Gnome macros.
dnl AM_ACLOCAL_INCLUDE(macros)

dnl GNOME macros
dnl GNOME_INIT
dnl GNOME_COMPILE_WARNINGS
dnl GNOME_X_CHECKS
dnl GNOME_WITH_VFS

dnl Add the languages which your application supports here.
dnl ALL_LINGUAS=""
dnl AM_GNU_GETTEXT([external])
dnl AM_GNU_GETTEXT_VERSION(0.11.4


dnl***********************
dnl  Check for libraries
dnl***********************
dnl EVO_CHECK_LIB(gdk-pixbuf with GNOME canvas support, 0.11, gnomecanvaspixbuf)
dnl EVO_CHECK_LIB(GAL, 0.11, gal)
dnl EVO_CHECK_LIB(GnomeVFS, 1.0.0, vfs)
dnl EVO_CHECK_LIB(gnome-print, 0.25, print)

dnl check for GConf
dnl AM_PATH_GCONF(1.0.4,,AC_MSG_ERROR(Smart Gnome Control requires GConf 1.0.4 or later (including the 'devel' packages)))

dnl pkg_modules="hamlib >= 1.2.2"
dnl PKG_CHECK_MODULES(HAMLIB, [$pkg_modules])
dnl AC_SUBST(HAMLIB_CFLAGS)
dnl AC_SUBST(HAMLIB_LIBS)

dnl EXTRA_GNOME_CFLAGS=`gnome-config --cflags gnome gnomeui gdk_pixbuf gnomecanvaspixbuf vfs`
dnl EXTRA_GNOME_LIBS=`gnome-config --libs gnome gnomeui gdk_pixbuf gnomecanvaspixbuf vfs`
dnl AC_SUBST(EXTRA_GNOME_CFLAGS)
dnl AC_SUBST(EXTRA_GNOME_LIBS)

dnl Set PACKAGE_LOCALE_DIR in config.h.
if test "x${prefix}" = "xNONE"; then
  AC_DEFINE_UNQUOTED(PACKAGE_LOCALE_DIR, "${ac_default_prefix}/${DATADIRNAME}/locale")
else
  AC_DEFINE_UNQUOTED(PACKAGE_LOCALE_DIR, "${prefix}/${DATADIRNAME}/locale")
fi

dnl Set PACKAGE SOURCE DIR in config.h.
packagesrcdir=`cd $srcdir && pwd`

dnl Set PACKAGE PREFIX
if test "x${prefix}" = "xNONE"; then
  packageprefix=${ac_default_prefix}
else
  packageprefix=${prefix}
fi

dnl Set PACKAGE DATA & DOC DIR
packagedatadir=share/${PACKAGE}
packagedocdir=doc/${PACKAGE}

dnl Set PACKAGE DIRs in config.h.
packagepixmapsdir=share/pixmaps/${PACKAGE}
packagehelpdir=share/gnome/help/${PACKAGE}
packagemenudir=share/gnome/apps

dnl Subst PACKAGE_DATA_DIR.
NO_PREFIX_PACKAGE_DATA_DIR="${packagedatadir}"
AC_SUBST(NO_PREFIX_PACKAGE_DATA_DIR)
PACKAGE_DATA_DIR="${packageprefix}/${packagedatadir}"
AC_SUBST(PACKAGE_DATA_DIR)

dnl Subst PACKAGE_DOC_DIR.
NO_PREFIX_PACKAGE_DOC_DIR="${packagedocdir}"
AC_SUBST(NO_PREFIX_PACKAGE_DOC_DIR)
PACKAGE_DOC_DIR="${packageprefix}/${packagedocdir}"
AC_SUBST(PACKAGE_DOC_DIR)

dnl Subst PACKAGE_PIXMAPS_DIR.
NO_PREFIX_PACKAGE_PIXMAPS_DIR="${packagepixmapsdir}"
AC_SUBST(NO_PREFIX_PACKAGE_PIXMAPS_DIR)
PACKAGE_PIXMAPS_DIR="${packageprefix}/${packagepixmapsdir}"
AC_SUBST(PACKAGE_PIXMAPS_DIR)

dnl Subst PACKAGE_HELP_DIR.
NO_PREFIX_PACKAGE_HELP_DIR="${packagehelpdir}"
AC_SUBST(NO_PREFIX_PACKAGE_HELP_DIR)
PACKAGE_HELP_DIR="${packageprefix}/${packagehelpdir}"
AC_SUBST(PACKAGE_HELP_DIR)

dnl Subst PACKAGE_MENU_DIR.
NO_PREFIX_PACKAGE_MENU_DIR="${packagemenudir}"
AC_SUBST(NO_PREFIX_PACKAGE_MENU_DIR)
PACKAGE_MENU_DIR="${packageprefix}/${packagemenudir}"
AC_SUBST(PACKAGE_MENU_DIR)

AC_DEFINE_UNQUOTED(PACKAGE_DATA_DIR, "${packageprefix}/${packagedatadir}")
AC_DEFINE_UNQUOTED(PACKAGE_DOC_DIR, "${packageprefix}/${packagedocdir}")
AC_DEFINE_UNQUOTED(PACKAGE_PIXMAPS_DIR, "${packageprefix}/${packagepixmapsdir}")
AC_DEFINE_UNQUOTED(PACKAGE_HELP_DIR, "${packageprefix}/${packagehelpdir}")
AC_DEFINE_UNQUOTED(PACKAGE_MENU_DIR, "${packageprefix}/${packagemenudir}")
AC_DEFINE_UNQUOTED(PACKAGE_SOURCE_DIR, "${packagesrcdir}")

AC_CONFIG_FILES([
Makefile
src/Makefile
pixmaps/Makefile
macros/Makefile
])
AC_OUTPUT

echo
echo SUMMARY:
echo
echo SGControl version.. : $VERSION
echo libcurl version.... : $CURL_V
echo Hamlib version..... : $HAMLIB_V
echo Glib version....... : $GLIB_V
echo Gdk version........ : $GDK_V
echo Gtk+ version....... : $GTK_V
echo

