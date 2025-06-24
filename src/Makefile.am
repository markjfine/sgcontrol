## Process this file with automake to produce Makefile.in

AM_CFLAGS = \
	@PACKAGE_CFLAGS@ -I..
	
bin_PROGRAMS = sgcontrol

sgcontrol_SOURCES = \
	main.c \
	database.c database.h \
	extra.c extra.h \
	interface.c interface.h \
	logfile.c logfile.h \
	EntryDlg.glade \
	LogWindow.glade \
	MemWindow.glade \
	OptionBox.glade \
	TuningDlg.glade \
	SGControl.glade
	
sgcontrol_CFLAGS = @PACKAGE_CFLAGS@ -I..
sgcontrol_LDADD = @PACKAGE_LIBS@ 
sgcontrol_LDFLAGS = -rdynamic

