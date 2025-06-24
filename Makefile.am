## Process this file with automake to produce Makefile.in

SUBDIRS = src pixmaps macros

pkgdata_DATA = ABOUT-NLS AUTHORS ChangeLog COPYING.LIB INSTALL LICENSE NEWS README README.md src/*.glade src/*.css

EXTRA_DIST = \
	autogen.sh \
	sgcontrol.desktop

gsettings_SCHEMAS = org.markjfine.sgcontrol.gschema.xml
@GSETTINGS_RULES@

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
	  $(mkinstalldirs) $(DESTDIR)$(datadir)/pixmaps/$(PACKAGE); \
	  for pixmap in $(srcdir)/pixmaps/*; do \
	    if test -f $$pixmap; then \
	      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(datadir)/pixmaps/$(PACKAGE); \
	    fi \
	  done \
	fi
	if test -f $(srcdir)/sgcontrol.desktop; then\
	  $(mkinstalldirs) $(DESTDIR)$(datadir)/applications; \
	  $(INSTALL_DATA) sgcontrol.desktop $(DESTDIR)$(datadir)/applications; \
	fi
	if test -d $(srcdir)/data; then \
	  $(mkinstalldirs) $(DESTDIR)$(datadir)/$(PACKAGE); \
	  for db_file in $(srcdir)/data/*; do \
	    if test -f $$db_file; then \
	      $(INSTALL_DATA) $$db_file $(DESTDIR)$(datadir)/$(PACKAGE); \
	    fi \
	  done \
	fi

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/pixmaps; \
	    fi \
	  done \
	fi
	if test -f $(srcdir)/sgcontrol.desktop; then\
	  cp -p gcontrol.desktop $(distdir); \
	fi
	if test -d $(srcdir)/data; then \
	  $(mkinstalldirs) $(DESTDIR)$(datadir)/$(PACKAGE); \
	  for db_file in $(srcdir)/data/*; do \
	    if test -f $$db_file; then \
	      $(INSTALL_DATA) $$db_file $(DESTDIR)$(datadir)/$(PACKAGE); \
	    fi \
	  done \
	fi

ACLOCAL_AMFLAGS = -I macros --install

