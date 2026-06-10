## Process this file with automake to produce Makefile.in

SUBDIRS = src pixmaps macros

pkgdata_DATA = \
	$(srcdir)/src/*.glade \
	$(srcdir)/src/*.css \
	$(srcdir)/data/*.txt \
	$(srcdir)/data/sgcontrol.png

doc_DATA = AUTHORS LICENSE NEWS README README.md

applicationsdir = $(datadir)/applications
applications_DATA = sgcontrol.desktop

gsettings_SCHEMAS = org.markjfine.sgcontrol.gschema.xml
@GSETTINGS_RULES@

EXTRA_DIST = \
	ABOUT-NLS ChangeLog INSTALL \
	autogen.sh \
	$(pkgdata_DATA) \
	$(doc_DATA) \
	$(applications_DATA) \
	$(gsettings_SCHEMAS)

ACLOCAL_AMFLAGS = -I macros --install

