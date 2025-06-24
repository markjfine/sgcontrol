## Please update this variable if any new macros are created

MACROS=	\
	build-to-host.m4 \
	gettext.m4\
	host-cpu-c-abi.m4 \
	iconv.m4 \
	intlmacosx.m4 \
	lib-ld.m4 \
	lib-link.m4 \
	lib-prefix.m4 \
	libtool.m4 \
	lt~obsolete.m4 \
	ltoptions.m4 \
	ltsugar.m4 \
	ltversion.m4 \
	nls.m4 \
	pkg.m4 \
	po.m4 \
	progtest.m4

EXTRA_DIST=
MAINTAINERCLEANFILES=macros.dep

@MAINT@ macros.dep: autogen.sh Makefile.am
@MAINT@	@echo '$$(top_srcdir)/aclocal.m4: $(MACROS:%=macros/%)' > $@

