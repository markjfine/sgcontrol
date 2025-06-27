/*
 * Smart Gnome Control - faceplate functions and hamlib interface file
 * Copyright (C) 2004, 2005, 2025 by Mark J. Fine <mark.fine@fineware-swl.com>
 *
 * This program provides a graphical user interface to multiple
 * communications receivers using the Hamlib library.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 *      Free Software Foundation, Inc.
 *      59 Temple Place - Suite 330
 *      Boston, MA  02111-1307
 *      USA
 *
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#define nullptr ((void*)0)

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include <hamlib/rig.h>

#include "extra.h"
#include "interface.h"
#include "database.h"
#include "logfile.h"

extern GtkWidget*	app1;
extern GtkWidget*	l_popup;
extern GtkWidget*	b_popup;
extern GtkWidget*	opt1;
extern GtkWidget*	mem1;
extern GtkWidget*	tun1;
extern GtkWidget*	log1;
extern GtkWidget*	entry_dlg;
extern GdkPixbuf*	meter_pix;
extern GdkPixbuf*	meters_pix;
extern GdkPixbuf*	digit_pix;
extern GdkPixbuf*	digits_pix;
extern guint		cur_rig_model; 
extern gchar*		cur_rig_make; 
extern gchar*		cur_rig_name;
extern gboolean		has_noise; 
extern gboolean		has_noise2; 
extern gboolean		has_dnr; 
extern gboolean		has_atten; 
extern gboolean		has_preamp; 
extern gboolean		has_notch;
extern gboolean		has_anf; 
extern gboolean		has_vfo;
extern gboolean		has_get_power;
extern gboolean		has_set_power;
extern gboolean		has_get_freq;
extern gboolean		has_get_vfo;
extern gboolean		has_set_vfo;
extern gboolean    	has_get_agc; 
extern gboolean    	has_set_agc; 
extern gboolean    	has_get_ant;
extern gboolean    	has_set_ant;
extern gboolean    	has_get_mode;
extern gboolean    	has_set_mode;
extern gboolean    	has_get_mem;
extern gboolean    	has_set_mem;
extern gboolean         has_get_channel;
extern gboolean         has_set_channel;
extern gboolean		has_meter;
extern gboolean		ispower;
extern gboolean    	isnoise;
extern gboolean    	isnoise2;
extern gboolean    	isdnr;
extern gboolean    	isnotch;
extern gboolean    	isanf;
extern gboolean    	istape;   
extern gboolean    	ismem;
extern gboolean    	isscan;
extern gboolean    	istimer;
extern gboolean    	issynch;
extern double		frequency;
extern guint       	vfo; 
extern guint       	ant;
extern guint       	agc;
extern guint       	bw;
extern guint       	mode;
extern int		memch; 
extern RIG*		hrig;
extern int		atten_val[5];
extern int		num_attens;
extern int		cur_atten;
extern int		preamp_val[5];
extern int		num_preamps;
extern int		cur_preamp;
extern GList*		bw_list;
extern GList*		mode_list; 
extern double		minf;
extern double		maxf;
extern int		min_mem_ch;
extern int		max_mem_ch;
extern int		min_mem_edge;  
extern int		max_mem_edge;
extern int		meter_level;
extern gboolean		tune_ok;
extern gchar*		e_text;
extern gboolean		has_af;
extern gran_t		af_range;
extern double		afval;
extern gboolean		has_rf;
extern gran_t		rf_range;
extern double		rfval;
extern gboolean		has_if;
extern gran_t		if_range;
extern double		ifval;
extern gboolean		has_pi;
extern gran_t		pi_range;
extern double		pival;
extern gboolean		has_po;
extern gran_t		po_range;
extern double		poval;
extern gboolean		has_cp;
extern gran_t		cp_range;
extern double		cpval;
extern gboolean		has_sq;
extern gran_t		sq_range;
extern double		sqval;
extern gboolean		has_no;
extern gran_t		no_range;
extern double		noval;
extern gboolean		has_nr;
extern gran_t		nr_range;
extern double		nrval;
extern gboolean		has_ap;
extern gran_t		ap_range;
extern double		apval;
extern gboolean		has_to;
extern gran_t		to_range;
extern double		toval;
extern gboolean		has_bwc;
extern gran_t		bwc_range;
extern double		bwcval;
extern int		hist_index;
extern int		hist_total;
extern double		sml_step;
extern double		lrg_step; 

extern gchar*		basepath;
extern gchar*      	db_ext_name;
extern int		num_freqs;

extern gchar*		log_name;

extern GtkBuilder* 	builder;
extern GSettings*	sgc_settings;
extern GtkCssProvider*	sgc_provider;

extern gint		currentDow;
extern double		curTime;

const guint		dWidth = 17;
const guint		dHeight = 24;
const guint		mWidth = 51;
const guint		mHeight = 31;

static GSList*		rig_list = NULL;
gchar*			mem_comment;
gchar*			cur_mem_file;
gboolean		cur_mem_default;

typedef struct {
	gchar*	label;
	double	start;
	double	end;
	gchar*	type;
} band_plan_t;

band_plan_t		plan_text[90] = {
  {" Nav",  100,  150,"N"},
  {" LW ",  150,  280,"B"},
  {" Nav",  280,  530,"N"},
  {" BCB",  530, 1710,"B"},
  {" Nav", 1710, 1800,"N"},
  {"160m", 1800, 2000,"A"},
  {" Mar", 2065, 2096,"M"},
  {" Mar", 2174, 2191,"M"},
  {"120m", 2300, 2495,"B"},
  {"Time", 2495, 2505,"T"},
  {"Fixd", 2505, 2850,"F"},
  {"Aero", 2850, 3155,"E"},
  {"Fixd", 3155, 3400,"F"},
  {" 90m", 3200, 3400,"B"},
  {"Aero", 3400, 3500,"E"},
  {" 80m", 3500, 4000,"A"},
  {" 75m", 3900, 4000,"B"},
  {"Fixd", 4000, 4065,"F"},
  {" Mar", 4065, 4438,"M"},
  {"Aero", 4438, 4750,"E"},
  {" 60m", 4750, 5060,"B"},
  {"Time", 4995, 5005,"T"},
  {"Fixd", 5060, 5450,"F"},
  {"Aero", 5450, 5730,"E"},
  {" 49m", 5730, 6295,"B"},
  {" Mar", 6200, 6525,"M"},
  {"Aero", 6525, 6765,"E"},
  {"Fixd", 6765, 7000,"F"},
  {" 40m", 7000, 7300,"A"},
  {" 41m", 6890, 7600,"B"},
  {"Fixd", 7600, 8195,"F"},
  {" Mar", 8195, 8815,"M"},
  {"Aero", 8815, 9040,"E"},
  {"Fixd", 9040, 9500,"F"},
  {" 31m", 9250, 9900,"B"},
  {"Fixd", 9900, 9995,"F"},
  {"Time", 9995,10005,"T"},
  {"Aero",10005,10100,"E"},
  {" 30m",10100,10150,"A"},
  {"Fixd",10150,11175,"F"},
  {"Aero",11175,11400,"E"},
  {"Fixd",11400,11700,"F"},
  {" 25m",11500,12160,"B"},
  {"Fixd",12160,12230,"F"},
  {" Mar",12230,13200,"M"},
  {"Aero",13200,13360,"E"},
  {"Fixd",13360,13600,"F"},
  {" 22m",13570,13870,"B"},
  {"Fixd",13870,14000,"F"},
  {" 20m",14000,14350,"A"},
  {"Fixd",14350,14990,"F"},
  {"Time",14990,15010,"T"},
  {"Aero",15010,15030,"E"},
  {" 19m",15030,15800,"B"},
  {"Fixd",15800,16360,"F"},
  {" Mar",16360,17410,"M"},
  {"Fixd",17410,17550,"F"},
  {" 16m",17480,17900,"B"},
  {"Aero",17900,18030,"E"},
  {"Fixd",18030,18060,"F"},
  {" 16m",18068,18168,"A"},
  {"Fixd",18168,18780,"F"},
  {" Mar",18780,18900,"M"},
  {" 15m",18900,19020,"B"},
  {"Fixd",19020,19680,"F"},
  {" Mar",19680,19800,"M"},
  {"Fixd",19800,19990,"F"},
  {"Time",19990,20010,"T"},
  {"Fixd",20010,21000,"F"},
  {" 15m",21000,21450,"A"},
  {" 13m",21450,21850,"B"},
  {"Astr",21850,21870,"R"},
  {"Aero",21870,22000,"E"},
  {" Mar",22000,22855,"M"},
  {"Fixd",22855,23200,"F"},
  {"Aero",23200,23350,"E"},
  {"Fixd",23350,24890,"F"},
  {" 12m",24890,24990,"A"},
  {"Time",24990,25010,"T"},
  {"Fixd",25010,25070,"F"},
  {" Mar",25070,25210,"M"},
  {"Fixd",25210,25550,"F"},
  {"Astr",25550,25600,"R"},
  {" 11m",25600,26100,"B"},
  {" Mar",26100,26200,"M"},
  {"Fixd",26200,26965,"F"},
  {" CB ",26965,27405,"C"},
  {"Fixd",27405,28000,"F"},
  {" 10m",28000,29700,"A"},
  {"Fixd",29700,30000,"F"}};

gint rig_list_make (const struct rig_caps *caps, gpointer data);
gint bwlist_compare_func                (gconstpointer s1, gconstpointer s2);
gint modelist_compare_func              (gconstpointer s1, gconstpointer s2);


GtkWidget*
lookup_widget                          (GtkWidget*	widget,
                                        const gchar*	widget_name)
{
  GtkWidget*	found_widget;

  found_widget = (GtkWidget*) gtk_builder_get_object(GTK_BUILDER(builder),widget_name);                                                
                                                   
  if (!found_widget)
    g_warning ("Widget not found: %s under %s\n", widget_name, gtk_widget_get_name(widget));
  return found_widget;
}


gboolean
check_schema_and_key			(const gchar*	schema_id,
                                         const gchar*	key_name)
{
    gboolean	schema_exists = FALSE;
    gboolean	key_exists = FALSE;

    // Get the default GSettings schema source
    GSettingsSchemaSource *source = g_settings_schema_source_get_default();

    if (source != NULL) {
        // Look up the schema
        GSettingsSchema *schema = g_settings_schema_source_lookup(source, schema_id, FALSE);

        if (schema != NULL) {
            schema_exists = TRUE;
            // Check if the key exists within the schema
            key_exists = g_settings_schema_has_key(schema, key_name);
            g_settings_schema_unref(schema); // Unreference the schema
        }
        // No need to unref the source as it's a global singleton
    }

    return (schema_exists && key_exists);
}


gint
get_theme_style()
{
  gchar*	theme;
  gint		result = 0;

  if (check_schema_and_key("org.gnome.desktop.interface", "color-scheme")) {
    GSettings* settings = g_settings_new("org.gnome.desktop.interface");
  
    theme = g_settings_get_string(settings, "color-scheme");
    if (strcmp(theme,"prefer-dark") == 0)
      result = 1;
    else
      result = 0;
    g_free(theme);
  }
  
  return result;
}


void
set_opt_glyphs()
{
  GdkRGBA	imgColor = {(double)192.0,(double)192.0,(double)192.0,(double) 1.0};

  gint theme = get_theme_style();
  if (theme == 1) {
    set_svg_color(opt1, "OptOkImg", PACKAGE_PIXMAPS_DIR"/mark.svg", imgColor);
    set_svg_color(opt1, "OptApplyImg", PACKAGE_PIXMAPS_DIR"/apply.svg", imgColor);
    set_svg_color(opt1, "OptCancelImg", PACKAGE_PIXMAPS_DIR"/cancel.svg", imgColor);
  }
}


void
set_glyphs_to_theme			(gint	theme_type)
{
  GdkRGBA	imgColor = {(double)192.0,(double)192.0,(double)192.0,(double) 1.0};
  
  if (theme_type == 1) {
    //Tuning buttons
    set_svg_color(app1, "EnterImg", PACKAGE_PIXMAPS_DIR"/enter.svg", imgColor);
    set_svg_color(app1, "BackspaceImg", PACKAGE_PIXMAPS_DIR"/backspace.svg", imgColor);
    set_svg_color(app1, "LLeftImg", PACKAGE_PIXMAPS_DIR"/backwards.svg", imgColor);
    set_svg_color(app1, "LeftImg", PACKAGE_PIXMAPS_DIR"/backward.svg", imgColor);
    set_svg_color(app1, "RightImg", PACKAGE_PIXMAPS_DIR"/forward.svg", imgColor);
    set_svg_color(app1, "RRightImg", PACKAGE_PIXMAPS_DIR"/forwards.svg", imgColor);
    //Radio buttons    
    set_svg_color(app1, "backarwimg", PACKAGE_PIXMAPS_DIR"/backarw.svg", imgColor);
    set_svg_color(app1, "forwarwimg", PACKAGE_PIXMAPS_DIR"/forwarw.svg", imgColor);
    set_svg_color(app1, "noiseimg", PACKAGE_PIXMAPS_DIR"/noise.svg", imgColor);
    set_svg_color(app1, "noiseimg1", PACKAGE_PIXMAPS_DIR"/noise.svg", imgColor);
    set_svg_color(app1, "dnrimg", PACKAGE_PIXMAPS_DIR"/dnr.svg", imgColor);
    set_svg_color(app1, "notchimg", PACKAGE_PIXMAPS_DIR"/notch.svg", imgColor);
    set_svg_color(app1, "anfimg", PACKAGE_PIXMAPS_DIR"/anf.svg", imgColor);
    set_svg_color(app1, "attenimg", PACKAGE_PIXMAPS_DIR"/atten.svg", imgColor);
    set_svg_color(app1, "preampimg", PACKAGE_PIXMAPS_DIR"/preamp.svg", imgColor);
    set_svg_color(app1, "commsimg", PACKAGE_PIXMAPS_DIR"/comms.svg", imgColor);
    set_svg_color(app1, "scanimg", PACKAGE_PIXMAPS_DIR"/scan.svg", imgColor);
    set_svg_color(app1, "memoryimg", PACKAGE_PIXMAPS_DIR"/memory.svg", imgColor);
    set_svg_color(app1, "showmemoryimg", PACKAGE_PIXMAPS_DIR"/showmemory.svg", imgColor);
    set_svg_color(app1, "logimg", PACKAGE_PIXMAPS_DIR"/log.svg", imgColor);
    set_svg_color(app1, "optimg", PACKAGE_PIXMAPS_DIR"/opt.svg", imgColor);
    set_svg_color(app1, "exitimg", PACKAGE_PIXMAPS_DIR"/exit.svg", imgColor);
    set_svg_color(app1, "AboutImg", PACKAGE_PIXMAPS_DIR"/about.svg", imgColor);
    //Faceplate Extension Buttons
    set_svg_color(app1, "CtlPnlCloseImg", PACKAGE_PIXMAPS_DIR"/close.svg", imgColor);
    set_svg_color(app1, "HistCloseImg", PACKAGE_PIXMAPS_DIR"/close.svg", imgColor);
    set_svg_color(app1, "PlanCloseImg", PACKAGE_PIXMAPS_DIR"/close.svg", imgColor);    
    //Database buttons    
    set_svg_color(app1, "openimg", PACKAGE_PIXMAPS_DIR"/open.svg", imgColor);
    set_svg_color(app1, "saveimg", PACKAGE_PIXMAPS_DIR"/save.svg", imgColor);
    set_svg_color(app1, "saveasimg", PACKAGE_PIXMAPS_DIR"/saveas.svg", imgColor);
    set_svg_color(app1, "printimg", PACKAGE_PIXMAPS_DIR"/print.svg", imgColor);
    set_svg_color(app1, "webimg", PACKAGE_PIXMAPS_DIR"/web.svg", imgColor);
    set_svg_color(app1, "dbviewimg", PACKAGE_PIXMAPS_DIR"/view.svg", imgColor);
    //Memory window buttons    
    set_svg_color(mem1, "NewImg", PACKAGE_PIXMAPS_DIR"/new.svg", imgColor);
    set_svg_color(mem1, "OpenImg", PACKAGE_PIXMAPS_DIR"/open.svg", imgColor);
    set_svg_color(mem1, "SaveImg", PACKAGE_PIXMAPS_DIR"/save.svg", imgColor);
    set_svg_color(mem1, "SaveAsImg", PACKAGE_PIXMAPS_DIR"/saveas.svg", imgColor);
    set_svg_color(mem1, "MemDownImg", PACKAGE_PIXMAPS_DIR"/backarw.svg", imgColor);
    set_svg_color(mem1, "MemUpImg", PACKAGE_PIXMAPS_DIR"/forwarw.svg", imgColor);
    set_svg_color(mem1, "MarkImg", PACKAGE_PIXMAPS_DIR"/mark.svg", imgColor);
    set_svg_color(mem1, "ModifyImg", PACKAGE_PIXMAPS_DIR"/modify.svg", imgColor);
    set_svg_color(mem1, "InsertImg", PACKAGE_PIXMAPS_DIR"/add.svg", imgColor);
    set_svg_color(mem1, "DeleteImg", PACKAGE_PIXMAPS_DIR"/delete.svg", imgColor);
    set_svg_color(mem1, "MemCloseImg", PACKAGE_PIXMAPS_DIR"/close.svg", imgColor);
    //Database window buttons    
    set_svg_color(tun1, "OpenDBImg", PACKAGE_PIXMAPS_DIR"/open.svg", imgColor);
    set_svg_color(tun1, "UpdateDBImg", PACKAGE_PIXMAPS_DIR"/web.svg", imgColor);
    set_svg_color(tun1, "RefreshImg", PACKAGE_PIXMAPS_DIR"/refresh.svg", imgColor);
    set_svg_color(tun1, "FreqTimeFilterImg", PACKAGE_PIXMAPS_DIR"/timefilt.svg", imgColor);
    set_svg_color(tun1, "StatTimeFilterImg", PACKAGE_PIXMAPS_DIR"/timefilt.svg", imgColor);
    set_svg_color(tun1, "DBCloseImg", PACKAGE_PIXMAPS_DIR"/close.svg", imgColor);
    //Log window buttons    
    set_svg_color(tun1, "LogNewImg", PACKAGE_PIXMAPS_DIR"/new.svg", imgColor);
    set_svg_color(tun1, "LogOpenImg", PACKAGE_PIXMAPS_DIR"/open.svg", imgColor);
    set_svg_color(tun1, "LogSaveImg", PACKAGE_PIXMAPS_DIR"/saveas.svg", imgColor);
    set_svg_color(tun1, "LogPrintImg", PACKAGE_PIXMAPS_DIR"/print.svg", imgColor);
    set_svg_color(tun1, "LogDownImg", PACKAGE_PIXMAPS_DIR"/backarw.svg", imgColor);
    set_svg_color(tun1, "LogUpImg", PACKAGE_PIXMAPS_DIR"/forwarw.svg", imgColor);
    set_svg_color(tun1, "LogClearImg", PACKAGE_PIXMAPS_DIR"/new.svg", imgColor);
    set_svg_color(tun1, "LogAddImg", PACKAGE_PIXMAPS_DIR"/add.svg", imgColor);
    set_svg_color(tun1, "LogUpdateImg", PACKAGE_PIXMAPS_DIR"/modify.svg", imgColor);
    set_svg_color(tun1, "LogDeleteImg", PACKAGE_PIXMAPS_DIR"/delete.svg", imgColor);
    set_svg_color(tun1, "LogCloseImg", PACKAGE_PIXMAPS_DIR"/close.svg", imgColor);
  }
}


void
set_power_color()
{
  GdkRGBA	imgColor;
  GdkRGBA	imgRed = {(double)255.0,(double)0.0,(double)0.0,(double) 1.0};
  GdkRGBA	imgGreen = {(double)0.0,(double)255.0,(double)0.0,(double) 1.0};

  if (ispower)
    imgColor = imgGreen;
  else
    imgColor = imgRed;
   
  set_svg_color(app1, "PowerImg", PACKAGE_PIXMAPS_DIR"/power.svg", imgColor);    
}


gint
get_dialog_type				(gchar* in_type)
{
  int dialogType = GTK_MESSAGE_OTHER;
  if (g_strcmp0(in_type,"info") == 0) {
    dialogType = GTK_MESSAGE_INFO;
  } else {
    if (g_strcmp0(in_type,"warning") == 0) {
      dialogType = GTK_MESSAGE_WARNING;
    } else {
      if (g_strcmp0(in_type,"question") == 0) {
        dialogType = GTK_MESSAGE_QUESTION;
      } else {
 	if (g_strcmp0(in_type,"error") == 0) {
  	  dialogType = GTK_MESSAGE_ERROR;
  	}
      }
    }
  }
  return dialogType;
}


gboolean
check_filename			       (gchar*		inStr)
{
  char		test[10] = {'*','\\',':',';','\"','\'','.','?','/',' '};
  gint		i;
  gboolean	result = false;
  
  if (strcmp(inStr,"") == 0) 
    result = true;
  else {
    for (i = 0; i < 10; i++) {
      if (strchr(inStr, test[i]) != NULL) {
        result = true;
        break;
      }  
    }
  }
  return result;
}


void
show_message                           (gchar*		in_title,
					GtkMessageType	in_type,
                                        gchar*		in_message)
{
  GtkWidget*	messagebox1;
  messagebox1 = gtk_message_dialog_new(GTK_WINDOW (app1),
                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                       in_type,
                                       GTK_BUTTONS_OK,
                                       "%s", in_message);
  				
  gtk_widget_set_name (messagebox1, "messagebox1");
  g_object_set_data (G_OBJECT (messagebox1), "messagebox1", messagebox1);
  gtk_window_set_title (GTK_WINDOW (messagebox1), in_title);
  gtk_window_set_position (GTK_WINDOW (messagebox1), GTK_WIN_POS_CENTER_ON_PARENT);
  gtk_window_set_modal (GTK_WINDOW (messagebox1), TRUE);
  gtk_dialog_run (GTK_DIALOG (messagebox1));
  gtk_widget_destroy (messagebox1);
}


gint
show_yes_no_message                    (gchar*		in_title,
					GtkMessageType	in_type,
                                        gchar*		in_message)
{
  GtkWidget*	YesNoDlg;
  gint		result;

  YesNoDlg = gtk_message_dialog_new(GTK_WINDOW (app1),
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    in_type,
                                    GTK_BUTTONS_YES_NO,
                                    "%s", in_message);
  
  gtk_widget_set_name (YesNoDlg, "YesNoDlg");
  g_object_set_data (G_OBJECT (YesNoDlg), "YesNoDlg", YesNoDlg);
  gtk_window_set_title (GTK_WINDOW (YesNoDlg), in_title);
  gtk_window_set_position (GTK_WINDOW (YesNoDlg), GTK_WIN_POS_CENTER_ON_PARENT);
  gtk_window_set_modal (GTK_WINDOW (YesNoDlg), TRUE);
  result = gtk_dialog_run (GTK_DIALOG (YesNoDlg));
  gtk_widget_destroy (YesNoDlg);
  
  return result;
}


gboolean
is_ctrl_set				(GdkEventKey* in_event)
{
  gboolean status = (((in_event->state & 0xf) & GDK_CONTROL_MASK) == GDK_CONTROL_MASK);
  return status;
}


void
process_keys                           (GtkWidget*   in_widget,
                                        GdkEventKey* in_event)
{
  switch (in_event->keyval) {
    case GDK_KEY_0 : ;
    case GDK_KEY_KP_0 : 
      set_focus(app1,"kp0");
      set_tuner_display(0);
      break;
    case GDK_KEY_1 : ;
    case GDK_KEY_KP_1 : 
      set_focus(app1,"kp1");
      set_tuner_display(1);
      break;
    case GDK_KEY_2 : ;
    case GDK_KEY_KP_2 : 
      set_focus(app1,"kp2");
      set_tuner_display(2);
      break;
    case GDK_KEY_3 : ;
    case GDK_KEY_KP_3 : 
      set_focus(app1,"kp3");
      set_tuner_display(3);
      break;
    case GDK_KEY_4 : ;
    case GDK_KEY_KP_4 : 
      set_focus(app1,"kp4");
      set_tuner_display(4);
      break;
    case GDK_KEY_5 : ;
    case GDK_KEY_KP_5 : 
      set_focus(app1,"kp5");
      set_tuner_display(5);
      break;
    case GDK_KEY_6 : ;
    case GDK_KEY_KP_6 : 
      set_focus(app1,"kp6");
      set_tuner_display(6);
      break;
    case GDK_KEY_7 : ;
    case GDK_KEY_KP_7 : 
      set_focus(app1,"kp7");
      set_tuner_display(7);
      break;
    case GDK_KEY_8 : ;
    case GDK_KEY_KP_8 : 
      set_focus(app1,"kp8");
      set_tuner_display(8);
      break;
    case GDK_KEY_9 : ;
    case GDK_KEY_KP_9 : 
      set_focus(app1,"kp9");
      set_tuner_display(9);
      break;    
    case GDK_KEY_period : ;
    case GDK_KEY_KP_Decimal : 
      set_focus(app1,"kpDot");
      set_tuner_display(10);
      break;    
    case GDK_KEY_BackSpace :
      set_focus(app1,"kpBS");
      set_tuner_display(11);
      break;
    case GDK_KEY_Return : ;
    case GDK_KEY_KP_Enter : 
      set_focus(app1,"kpEnter");
      set_tuner_display(13);
      break;    
    case GDK_KEY_Left : ;
    case GDK_KEY_KP_Left :
      if (is_ctrl_set(in_event)) {
        set_focus(app1,"LLeftBtn");
        tune_up_down(-sml_step);
      } else {
        set_focus(app1,"LeftBtn");
        tune_up_down(-lrg_step);
      }
      break;
    case GDK_KEY_Right : ;
    case GDK_KEY_KP_Right :
      if (is_ctrl_set(in_event)) {
        set_focus(app1,"RRightBtn");
        tune_up_down(sml_step);
      } else {
        set_focus(app1,"RightBtn");
        tune_up_down(lrg_step);
      }
      break;
  }
}


gchar*
get_pathname_from_full_path            (gchar* in_path)
{
  gint		len;
  gchar*	ptr;
  gchar*	pathname = NULL;
	
  ptr = strrchr(in_path,'/');
  if (ptr) {
    len = ptr - in_path + 1;
    pathname = g_strndup(in_path,len);
  } else
    pathname = g_strdup("<blank>");
  return pathname;
}


gchar*
get_filename_from_full_path            (gchar* in_path)
{
  gchar*	ptr;
  gchar*	filename = NULL;
	
  ptr = strrchr(in_path,'/');
  if (ptr)
    filename = g_strdup(ptr+1);
  else
    filename = g_strdup("<blank>");
  return filename;
}


gchar*
get_extension_from_full_path           (gchar* in_path)
{
  gchar*	ptr;
  gchar*	extension = NULL;
	
  ptr = strrchr(in_path,'.');
  if (ptr)
    extension = g_strdup(ptr+1);
  else
    extension = g_strdup("<blank>");
  return extension;
}


/** widget control primitives **/
void
set_app_title()
{
  gchar* title_str = g_strdup_printf("Smart Gnome Control %s - [%d] %s %s - [%s]",
                                      VERSION, cur_rig_model, cur_rig_make, cur_rig_name, db_ext_name);
  gtk_window_set_title(GTK_WINDOW(app1),title_str);
}


void
update_immediate()
{
  while (gtk_events_pending())
    gtk_main_iteration();
}


void
set_widget_color	               (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        GdkRGBA         inColor)
{
  GtkWidget* outWidget = lookup_widget(inWidget,widget_item);
  if (outWidget) {
    GtkStyleContext* context = gtk_widget_get_style_context(outWidget);
    if (context) {
      gtk_style_context_add_provider(context, (GtkStyleProvider*)sgc_provider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
  }
}


void
set_svg_color				(GtkWidget*	inWidget,
                                         gchar*		widget_item,
                                         gchar*		filename,
                                         GdkRGBA	inColor)
{
  GObject*	gen_obj;
  GdkPixbuf*	svg_img;
  GError*	err = nullptr;

  gen_obj = (GObject*)lookup_widget(inWidget,widget_item);
  if (gen_obj) {
    svg_img = gdk_pixbuf_new_from_file(filename, &err);
    if (err != NULL) {
      g_print("%s\n",err->message);
      g_clear_error(&err);
    }
    if (svg_img) {
      guint8 *pixels = gdk_pixbuf_get_pixels(svg_img);
      int rowstride = gdk_pixbuf_get_rowstride(svg_img);
      int width = gdk_pixbuf_get_width(svg_img);
      int height = gdk_pixbuf_get_height(svg_img);
      int bytes_per_pixel = 4;
      
      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
          guint8 *pixel = pixels + y * rowstride + x * bytes_per_pixel;
          if ((pixel[0] == 0) && (pixel[1] == 0) && (pixel[2] == 0)) {
            pixel[0] = inColor.red;
            pixel[1] = inColor.green;
            pixel[2] = inColor.blue;
          }
        }
      }
      gtk_image_set_from_pixbuf((GtkImage*)gen_obj, svg_img);
      g_object_unref(svg_img);
    }    
  }
}


void
set_visible                            (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        gboolean        status)
{
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item) {
    gtk_widget_set_visible(gen_item,status);
  }
}


gboolean
get_visible                            (GtkWidget*      inWidget,
                                        gchar*          widget_item)
{
  gboolean status=FALSE;
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item)
    status = gtk_widget_get_visible(gen_item);
  return status;
}


void
toggle_visible                         (GtkWidget*      item,
                                        gchar*          widget_item)
{
  GtkWidget* gen_item = lookup_widget(item,widget_item);
  if (gen_item) {
    if (gtk_widget_get_visible(gen_item))
      gtk_widget_hide(gen_item);
    else
      gtk_widget_show(gen_item);
  }
}


gboolean
get_focus                              (GtkWidget*      inWidget,
                                        gchar*          widget_item)
{
  GtkWidget*	gen_item;
  gboolean	has_focus;

  has_focus = FALSE;	
  gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item) {
    has_focus = gtk_widget_has_focus(gen_item);
  }
  return has_focus;
}


void
set_focus                              (GtkWidget*      inWidget,
                                        gchar*          widget_item)
{
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item) {
    if (gtk_widget_get_can_focus(gen_item))
      gtk_widget_grab_focus(gen_item);
  }
}


void
set_enabled                            (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        gboolean        status)
{
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item)
    gtk_widget_set_sensitive(gen_item,status);
}


gboolean
get_enabled                            (GtkWidget*      inWidget,
                                        gchar*          widget_item)
{
  int status = FALSE;

  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item)
    status = gtk_widget_get_sensitive(gen_item);
  return status;
}


void
toggle_enabled                         (GtkMenuItem*    menuitem,
                                        gchar*          widget_item)
{
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(menuitem),widget_item);
  if (gen_item) {
    if (gtk_widget_get_sensitive(gen_item))
      gtk_widget_set_sensitive(gen_item,FALSE);
    else
      gtk_widget_set_sensitive(gen_item,TRUE);
  }
}


void
set_down                               (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        gboolean        status)
{
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item) {
    if (GTK_IS_TOGGLE_TOOL_BUTTON(gen_item))
      gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(gen_item),status);
    else
      if (GTK_IS_TOGGLE_BUTTON(gen_item))
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gen_item), status);
  }
}


gboolean
get_down                               (GtkWidget*      inWidget,
                                        gchar*          widget_item)
{
  int status = FALSE;

  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item) {
    if (GTK_IS_TOGGLE_TOOL_BUTTON(gen_item))
      status = gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(gen_item));
    else
      if (GTK_IS_TOGGLE_BUTTON(gen_item))
        status = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gen_item));
  }
  return status;
}


void
set_checked                            (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        gboolean        status)
{
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item)
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gen_item),status);
}


gboolean
get_checked                            (GtkWidget*      inWidget,
                                        gchar*          widget_item)
{
  gboolean status = FALSE;
  
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item)
    status = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(gen_item));
  return status;
}


void
set_scale_value                        (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        gdouble         value)
{
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item) {
    GtkAdjustment* adj = gtk_range_get_adjustment(GTK_RANGE(gen_item));
    gtk_adjustment_set_value(adj,(gfloat)value);
  }
}


void
set_scale_range                        (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        gran_t          range,
                                        int             is_float)
{
  GtkAdjustment* adj;
  gfloat     min, max, step;
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item) {
    if (is_float) {
      min = -range.max.f;
      max = -range.min.f;
      step = range.step.f;
    } else {
      min = -range.max.i;
      max = -range.min.i;
      step = range.step.i;
    }
    adj = gtk_range_get_adjustment(GTK_RANGE(gen_item));
    gtk_adjustment_set_value(adj,0);
    gtk_adjustment_set_lower(adj,min);
    gtk_adjustment_set_upper(adj,max);
    gtk_adjustment_set_step_increment(adj,step);
    gtk_adjustment_set_page_increment(adj,step*3);
    gtk_adjustment_set_page_size(adj,0);
  }
}


gdouble
get_scale_value                        (GtkWidget*      inWidget,
                                        gchar*          widget_item)
{
  gdouble value = 0.0;
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item) {
    GtkAdjustment* adj = gtk_range_get_adjustment(GTK_RANGE(gen_item));
    value = gtk_adjustment_get_value(adj);
  }
  return value;
}


void
set_toolbar_style                      (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        GtkToolbarStyle style)
{
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item)
    gtk_toolbar_set_style(GTK_TOOLBAR(gen_item),style);
}


void
set_toolbar_icon_size                  (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        GtkIconSize 	icon_size)
{
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item)
    gtk_toolbar_set_icon_size(GTK_TOOLBAR(gen_item),icon_size);
}


void
add_menu_item                          (GtkWidget*	opt_menu,
                                        gchar*		menu_str)
{
  g_menu_append(G_MENU (opt_menu), menu_str, NULL);
}


void
set_option                             (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        guint           item)
{
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item)
    gtk_combo_box_set_active(GTK_COMBO_BOX(gen_item),item);
}


void
set_text                               (GtkWidget*      inWidget,
                                        gchar*		widget_item,
                                        gchar*		text)
{
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item) {
    if (GTK_IS_LABEL(gen_item))
      gtk_label_set_text(GTK_LABEL(gen_item),text);
    else if (GTK_IS_ENTRY(gen_item))
      gtk_entry_set_text(GTK_ENTRY(gen_item),text);
    else if (GTK_IS_MENU_ITEM(gen_item))
      gtk_menu_item_set_label(GTK_MENU_ITEM(gen_item),text);    
    //else if (GTK_IS_COMBO_BOX_TEXT(gen_item))
      //gtk_combo_box_text_set_text(GTK_ENTRY(GTK_COMBO_BOX_TEXT(gen_item)),text);
  }
}


void
get_text                               (GtkWidget*      inWidget,
                                        gchar*		widget_item,
                                        gchar**		text)
{
  GtkWidget* gen_item = lookup_widget(GTK_WIDGET(inWidget),widget_item);
  if (gen_item) {
    if (GTK_IS_LABEL(gen_item))
      *text = strdup(gtk_label_get_text(GTK_LABEL(gen_item)));
    else if (GTK_IS_ENTRY(gen_item))
      *text = strdup(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(gen_item))));
    else if (GTK_IS_MENU_ITEM(gen_item))
      *text = strdup(gtk_menu_item_get_label(GTK_MENU_ITEM(gen_item)));    
    else if (GTK_IS_COMBO_BOX_TEXT(gen_item))
      *text = strdup(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(gen_item)));
  }
}


/** special widget control primitives **/
void
set_display_cursor                     (GtkWidget*	inWidget,
                                        GdkCursorType	inCursor)
{
  const gchar*	w_name;
  GdkCursor 	*cursor;
  GdkWindow* 	digits_win = gtk_widget_get_parent_window(inWidget);
  GtkWidget* 	gen_item = lookup_widget(app1,"appbar1");

  if (ispower) {
    cursor = gdk_cursor_new_for_display (gdk_display_get_default(), inCursor);
    gdk_window_set_cursor (digits_win, cursor);
    if (inCursor == GDK_HAND2) {
      w_name = gtk_widget_get_name(inWidget);
      if (strcmp(w_name,"eventbox1") == 0)
        gtk_label_set_text(GTK_LABEL(gen_item),"Left mouse btn - tune down, right btn - tune up, middle btn - zero digit");
      else if (strcmp(w_name,"eventbox2") == 0)
        gtk_label_set_text(GTK_LABEL(gen_item),"Click to select memory channel");
      else if (strcmp(w_name,"eventbox3") == 0)
        gtk_label_set_text(GTK_LABEL(gen_item),"Click to display Band Plan");
      else if (strcmp(w_name,"clist1") == 0)
        gtk_label_set_text(GTK_LABEL(gen_item),"Left mouse btn - get parallels, right btn - create log entry");
    } else
      gtk_label_set_text(GTK_LABEL(gen_item),"");
  }
}


void
set_display_mouse                      (GtkWidget*	inWidget,
                                        GdkEventButton*	inEvent)
{
  guint		power;
  freq_t	freq, dfreq;
  gint		x;
  gint		y;
  gchar*	fStr;

  GdkWindow* digits_win = gtk_widget_get_window(inWidget);
  GdkSeat* dev_seat = gdk_display_get_default_seat(gdk_display_get_default());
  GdkDevice* dev = gdk_seat_get_pointer (dev_seat);
  gdk_window_get_device_position (digits_win, dev, &x, &y, NULL);
  gint inX2 = (gint) x / dWidth;
  if (inX2 != 7) {
    if (inX2 > 7)
      inX2--;
    freq = (frequency * 1000.0);
    power = 9 - inX2;
    dfreq = (guint64) pow (10, power);
    switch (inEvent->button) {
      case 3:
      case 4: /* RIGHT button or wheel up: increase digit */
        freq += dfreq;
        if (freq <= maxf) {
          frequency = freq / 1000.0;
          set_freq(FALSE);
        }
        break;
      case 2: /*MIDDLE button: zero digit */
        fStr = g_strdup_printf("%10d",(int)freq);
        fStr[inX2] = '0';
        freq = (freq_t)atol(fStr);
        if ((freq >= minf) && (freq <= maxf)) {
          frequency = freq / 1000.0;
          set_freq(FALSE);
        }
        g_free(fStr);
        break;
      case 1:
      case 5: /* LEFT button or wheel down: decrease digit */
        freq -= dfreq;
        if (freq >= minf) {
          frequency = freq / 1000.0;
          set_freq(FALSE);
        }
        break;
      default:
        break;
    }
  }
}


gint
get_list_num_rows                      (GtkWidget* inWidget,
                                        gchar*     list_name)
{
  GtkWidget*	gen_item;
  GtkTreeModel*	store;
  gint		row_nums;

  row_nums = -1;
  gen_item = lookup_widget(inWidget, list_name);
  if (gen_item) {
    store = gtk_tree_view_get_model((GtkTreeView*)gen_item);
    if (store)
      row_nums = gtk_tree_model_iter_n_children(store, NULL);
  }
  return row_nums;
}


gint
get_list_selected_row                  (GtkWidget* inWidget,
                                        gchar*     list_name)
{
  GtkWidget*		gen_item;
  GtkTreeSelection*	selection;
  GtkTreeModel*		model;
  GtkTreePath*		path = nullptr;
  GtkTreeIter		iter;
  gint			row_num;
	
  row_num = -1;
  gen_item = lookup_widget(inWidget, list_name);
  if (gen_item) {
    selection = gtk_tree_view_get_selection((GtkTreeView*)gen_item);
    if (selection) {
      gtk_tree_selection_get_selected(selection, &model, &iter);
      path = gtk_tree_model_get_path(model, &iter);
      if (path) {
        row_num = atoi(gtk_tree_path_to_string(path));
        gtk_tree_path_free(path);
      }
    }
  }
  return row_num;
}


void
set_list_selected_row                  (GtkWidget*	inWidget,
                                        gchar*		list_name,
                                        gint		in_row)
{
  GtkWidget*	gen_item;
  GtkTreePath*	list_path;
	
  gen_item = lookup_widget(inWidget,list_name);
  if (gen_item) {
    GtkTreeIter iter;
    GtkTreeModel* tree_model = gtk_tree_view_get_model((GtkTreeView*)gen_item);
    gtk_tree_model_iter_nth_child(tree_model,&iter,NULL,in_row);
    GtkTreeSelection* tree_sel = gtk_tree_view_get_selection((GtkTreeView*)gen_item);
    gtk_tree_selection_select_iter(tree_sel,&iter);
    list_path = gtk_tree_model_get_path(tree_model, &iter);
    if (list_path)
      gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(gen_item), list_path, NULL, false, 0, 0);  
  }
}


void
get_device_clicked_x_y                 (GtkWidget*	inWidget,
                                        gint*		inX,
                                        gint*		inY)
{
  GdkWindow* digits_win = gtk_widget_get_window(inWidget);
  GdkSeat* dev_seat = gdk_display_get_default_seat(gdk_display_get_default());
  GdkDevice* dev = gdk_seat_get_pointer (dev_seat);
  gdk_window_get_device_position (digits_win, dev, inX, inY, NULL);
}


void
select_from_x_y						(GtkWidget*	inWidget,
                                                         GtkWidget*	selection)
{
  GtkTreePath*	path;
  gchar*	pathStr;
  gint		in_row;
  gint		in_col;

  get_list_clicked_row_col(inWidget, NULL, &in_row, &in_col);
  if (in_row > -1) {
    pathStr = g_strdup_printf("%d", in_row);
    path = gtk_tree_path_new_from_string(pathStr);
    if (path)
      gtk_tree_selection_select_path((GtkTreeSelection*)selection, path);
    g_free(pathStr);
    gtk_tree_path_free(path);
  }
}


void
get_list_clicked_row_col               (GtkWidget*	inWidget,
                                        GdkEventButton*	inEvent,
                                        gint*		in_row,
                                        gint*		in_col)
{
  gint			x;
  gint			x1;
  gint			y;
  gint			y1;
  GtkTreePath*		path;
  GtkTreeViewColumn*	column;
  const gchar*		title;
  const gchar*		name;

  get_device_clicked_x_y(inWidget,&x,&y);
  gtk_tree_view_convert_widget_to_bin_window_coords((GtkTreeView*)inWidget, x, y, &x1, &y1);
  path = gtk_tree_path_new();
  gtk_tree_view_get_path_at_pos((GtkTreeView*)inWidget, x1, y1, &path, &column, NULL, NULL);
  if (path) {
    *in_row = atoi(gtk_tree_path_to_string(path));
    title = gtk_tree_view_column_get_title(column);    
    name = gtk_widget_get_name(inWidget);
    if ((strcmp(name,"BCList") == 0) || (strcmp(name,"UteList") == 0)) {
      if (strcmp(title,"Band") == 0)
        *in_col = 0;
      else if (strcmp(title,"Start") == 0)
        *in_col = 1;
      else if (strcmp(title,"End") == 0)
        *in_col = 2;
      else if (strcmp(title,"Type") == 0)
        *in_col = 3;
    }
    if (strcmp(name,"MemList") == 0) {
      if (strcmp(title,"Ch") == 0)
        *in_col = 0;
      else if (strcmp(title,"Frequency") == 0)
        *in_col = 1;
      else if (strcmp(title,"VFO") == 0)
        *in_col = 2;
      else if (strcmp(title,"Ant") == 0)
        *in_col = 3;
      else if (strcmp(title,"Mode") == 0)
        *in_col = 4;
      else if (strcmp(title,"BW") == 0)
        *in_col = 5;
      else if (strcmp(title,"Name") == 0)
        *in_col = 6;
    }
    if (strcmp(name,"clist1") == 0) {
      if (strcmp(title,"Freq.") == 0)
        *in_col = 0;
      else if (strcmp(title,"From") == 0)
        *in_col = 1;
      else if (strcmp(title,"To") == 0)
        *in_col = 2;
      else if (strcmp(title,"Days") == 0)
        *in_col = 3;
      else if (strcmp(title,"Language") == 0)
        *in_col = 4;
      else if (strcmp(title,"Target") == 0)
        *in_col = 5;
      else if (strcmp(title,"Country") == 0)
        *in_col = 6;
      else if (strcmp(title,"Station") == 0)
        *in_col = 7;
    }
  }
  gtk_tree_path_free(path);
}


gint
get_list_clicked_row                   (GtkWidget*	inWidget,
                                        GdkEventButton*	inEvent)
{
  GtkTreePath*	path;
  gint		result = -1;
  gint		x;
  gint		x1;
  gint		y;
  gint		y1;

  if (inEvent->button == 1) {
    get_device_clicked_x_y(inWidget,&x,&y);
    gtk_tree_view_convert_widget_to_bin_window_coords((GtkTreeView*)inWidget, x, y, &x1, &y1);
    path = gtk_tree_path_new();
    gtk_tree_view_get_path_at_pos((GtkTreeView*)inWidget, x1, y1, &path, NULL, NULL, NULL);
    if (path) {
      result = atoi(gtk_tree_path_to_string(path));
      gtk_tree_path_free(path);
    }
  }
  return result;
}


void
get_list_text			       (GtkWidget*	inWidget,
					gint		in_row,
					gint		in_col,
					gchar**		text)
{
  GtkTreeModel*	model;
  GValue	value = G_VALUE_INIT;
  GtkTreeIter	iter;

  if ((in_row > -1) && (in_col > -1)) {
    if (inWidget) {
      model = gtk_tree_view_get_model((GtkTreeView*)inWidget);
      gtk_tree_model_iter_nth_child(model,&iter,NULL,in_row);
      gtk_tree_model_get_value(model,&iter,in_col,&value);
      *text = strdup(g_value_get_string(&value));
    }
  }
  g_value_unset(&value);
}


gint
get_notebook_page                      (GtkWidget* inWidget,
                                        gchar*     notebook)
{
  GtkWidget*	gen_item;
  gint		page_num;
	
  page_num = -1;
  gen_item = lookup_widget(inWidget, notebook);
  if (gen_item)
    page_num = gtk_notebook_get_current_page(GTK_NOTEBOOK(gen_item));
  return page_num;
}


void
update_clocks()
{
  updateLocal(app1);
  updateUTC(app1);
}


void
updateLocal                            (GtkWidget* inWidget)
{
  time_t    s_time;
  struct tm l_time;

  time(&s_time);
  localtime_r(&s_time,&l_time);
  gchar* local_time = g_strdup_printf("%02d%02d",l_time.tm_hour,l_time.tm_min);
  set_text(app1,"Local2Lbl",local_time);
  g_free(local_time);
}


void
updateUTC                              (GtkWidget* inWidget)
{
  time_t    s_time;
  struct tm g_time;

  time(&s_time);
  gmtime_r(&s_time,&g_time);
  gchar* gmt_time = g_strdup_printf("%02d%02d",g_time.tm_hour,g_time.tm_min);
  currentDow = g_time.tm_wday - 1;
  if (currentDow == -1)
    currentDow = 6;
  currentDow = (gint)pow(2.0,(double)currentDow);
  curTime = (double)g_time.tm_hour + (double)(g_time.tm_min / 60.0);   
  set_text(app1,"UTC2Lbl",gmt_time);
  g_free(gmt_time);
}

char months[12][4] = {"Jan\x00", "Feb\x00", "Mar\x00", "Apr\x00", "May\x00", "Jun\x00", 
		      "Jul\x00", "Aug\x00", "Sep\x00", "Oct\x00", "Nov\x00", "Dec\x00"};
char days[7] = {'s','m','t','w','h','f','a'};

gchar*
get_current_date()
{
  time_t    s_time;
  struct tm g_time;
  gchar*    gmt_date;

  time(&s_time);
  gmtime_r(&s_time,&g_time);
  gmt_date = g_strdup_printf("%02d %s %02d",g_time.tm_mday, months[g_time.tm_mon], g_time.tm_year % 100);
  
  return gmt_date;
}



char
get_current_dow_letter()
{
  time_t    s_time;
  struct tm g_time;

  time(&s_time);
  gmtime_r(&s_time,&g_time);
  return days[g_time.tm_wday];
}


/* band plan functions */
void
set_cell_font_weight				(GtkTreeViewColumn*	col,
						 GtkCellRenderer*	cell,
						 GtkTreeModel*		model,
						 GtkTreeIter*		iter,
						 gpointer		data)
{
  GValue value = G_VALUE_INIT;
  
  gtk_tree_model_get_value(model, iter, 3, &value);
  const gchar* text = g_value_get_string(&value);
  if ((strcmp(text, "B") == 0) || (strcmp(text, "E") == 0)) {
    g_object_set(cell, "weight", PANGO_WEIGHT_BOLD, "weight-set", true, NULL);
  } else {
    g_object_set(cell, "weight", PANGO_WEIGHT_NORMAL, "weight-set", true, NULL);  
  }
  g_value_unset(&value);
}


void
fill_band_display()
{
  GtkWidget*	bc_list;
  GtkWidget*	ute_list;
  GtkListStore*	bc_store;
  GtkListStore*	ute_store;
  GtkTreeIter	bc_iter;
  GtkTreeIter	ute_iter;
  gchar*	buf;
  gchar**	text;
  guint		i;

  enum {
    BAND = 0,
    START,
    STOP,
    TYPE,
    NUM_COLS
  };

  bc_store = gtk_list_store_new( NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, -1);
  ute_store = gtk_list_store_new( NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, -1);
  
  if ((bc_store) && (ute_store)) {

    for (i = 0; i < 90; i++) {
      buf = g_strdup_printf("%s;%5.0f;%5.0f;%s", plan_text[i].label,
                                                 plan_text[i].start,
                                                 plan_text[i].end,
                                                 plan_text[i].type);
      text = g_strsplit(buf, ";", 4);
      switch(plan_text[i].type[0]) {
        case 'A' :
        case 'B' :
        case 'C' :
          gtk_list_store_append (bc_store, &bc_iter);        
          gtk_list_store_set (bc_store, &bc_iter,
                              BAND,  text[0],
                              START, text[1],
                              STOP,  text[2],
                              TYPE,  text[3],
                              -1);
          break;
        case 'E' :
        case 'F' :
        case 'M' :
        case 'N' :
        case 'T' :
        case 'R' :
          gtk_list_store_append (ute_store, &ute_iter);        
          gtk_list_store_set (ute_store, &ute_iter,
                              BAND,  text[0],
                              START, text[1],
                              STOP,  text[2],
                              TYPE,  text[3],
                              -1);
          break;
        default : break;
      }
      g_free(buf);
      g_strfreev(text);
    }
    
    bc_list = lookup_widget(app1,"BCList");
    ute_list = lookup_widget(app1,"UteList");
    
    gtk_tree_view_set_model ((GtkTreeView*)bc_list, (GtkTreeModel*)bc_store);
    gtk_tree_view_set_model ((GtkTreeView*)ute_list, (GtkTreeModel*)ute_store);

    g_object_unref(bc_store);
    g_object_unref(ute_store);

    gtk_tree_view_column_set_cell_data_func((GtkTreeViewColumn*)lookup_widget( app1, "band_col1"),
                                            (GtkCellRenderer*)lookup_widget( app1, "band1Renderer"),
                                            (GtkTreeCellDataFunc)set_cell_font_weight, NULL, NULL);
    gtk_tree_view_column_set_cell_data_func((GtkTreeViewColumn*)lookup_widget( app1, "start_col1"),
                                            (GtkCellRenderer*)lookup_widget( app1, "start1Renderer"),
                                            (GtkTreeCellDataFunc)set_cell_font_weight, NULL, NULL);
    gtk_tree_view_column_set_cell_data_func((GtkTreeViewColumn*)lookup_widget( app1, "stop_col1"),
                                            (GtkCellRenderer*)lookup_widget( app1, "stop1Renderer"),
                                            (GtkTreeCellDataFunc)set_cell_font_weight, NULL, NULL);
    gtk_tree_view_column_set_cell_data_func((GtkTreeViewColumn*)lookup_widget( app1, "type_col1"),
                                            (GtkCellRenderer*)lookup_widget( app1, "type1Renderer"),
                                            (GtkTreeCellDataFunc)set_cell_font_weight, NULL, NULL);
                                            
    gtk_tree_view_column_set_cell_data_func((GtkTreeViewColumn*)lookup_widget( app1, "band_col2"),
                                            (GtkCellRenderer*)lookup_widget( app1, "band2Renderer"),
                                            (GtkTreeCellDataFunc)set_cell_font_weight, NULL, NULL);
    gtk_tree_view_column_set_cell_data_func((GtkTreeViewColumn*)lookup_widget( app1, "start_col2"),
                                            (GtkCellRenderer*)lookup_widget( app1, "start2Renderer"),
                                            (GtkTreeCellDataFunc)set_cell_font_weight, NULL, NULL);
    gtk_tree_view_column_set_cell_data_func((GtkTreeViewColumn*)lookup_widget( app1, "stop_col2"),
                                            (GtkCellRenderer*)lookup_widget( app1, "stop2Renderer"),
                                            (GtkTreeCellDataFunc)set_cell_font_weight, NULL, NULL);
    gtk_tree_view_column_set_cell_data_func((GtkTreeViewColumn*)lookup_widget( app1, "type_col2"),
                                            (GtkCellRenderer*)lookup_widget( app1, "type2Renderer"),
                                            (GtkTreeCellDataFunc)set_cell_font_weight, NULL, NULL);
  }
}


void
do_BandPlan_clicked                    (GtkWidget*	widget,
                                        GdkEventButton*	event)
{
  gint	in_row = -1;
  gint	in_col = -1;

  if (event->button == 1) {
    get_list_clicked_row_col(widget,event,&in_row,&in_col);
    tune_from_grid(widget,in_row,in_col);
  }
}


double
get_grid_freq                          (GtkWidget*	in_widget,
                                        gint		in_row,
                                        gint		in_col)
{
  gchar*	text = "";
  double	result = 0;

  get_list_text(in_widget, in_row, in_col, &text);
  if (text)
    result = strtod(text,NULL);
  return result;
}


void
tune_from_db			       (double	inFreq)
{
  if ((ispower) && (inFreq > 0.0)) {
    frequency = inFreq;
    set_freq(true);
  }
}


void
tune_from_grid                         (GtkWidget*	in_widget,
                                        gint		in_row,
                                        gint		in_col)
{
  double	r_freq;

  r_freq = get_grid_freq(in_widget,in_row,in_col);
  if ((r_freq >= minf/1000)&&(r_freq <= maxf/1000)) {
    frequency = r_freq;
    set_freq(FALSE);
  }
}


/* frequency history functions */
void
init_hist_display()
{
  GtkWidget*	hist_list;
  GtkListStore*	hist_store;

  enum {
    TIME = 0,
    FREQ,
    NUM_COLS
  };

  hist_store = gtk_list_store_new( NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, -1);
  if (hist_store) {
    hist_list = lookup_widget(app1,"HistList");
    gtk_tree_view_set_model ((GtkTreeView*)hist_list, (GtkTreeModel*)hist_store);
    g_object_unref(hist_store);    
  }
}


double
get_history_freq                       (gint		in_row)
{
  gchar*	text;
  double	result = 0;

  if (in_row > -1) {
    GtkWidget* gen_item = lookup_widget(GTK_WIDGET(app1),"HistList");
    if (gen_item) {
      get_list_text(gen_item, in_row, 1, &text);
      result = strtod(text,NULL);
    }
  }
  return result;
}


void
tune_from_history                      (gint		in_row,
                                        gint		set_row)
{
  double	r_freq;

  r_freq = get_history_freq(in_row);
  if ((r_freq >= minf/1000)&&(r_freq <= maxf/1000)) {
    if (set_row)
      set_list_selected_row(app1,"HistList",in_row);
    else
      hist_index = in_row;
    set_history_buttons();
    frequency = r_freq;
    set_freq(FALSE);
  }
}


void
add_to_history()
{
  GtkTreeView*	hist_list;
  GtkListStore*	hist_store;
  GtkTreePath*	hist_path;
  GtkTreeIter	hist_iter;
  GtkTreeIter	hist_iter2;
  int		i;
  struct	tm g_time;
  time_t	s_time;
  gchar*	buf;
  gchar**	text;

  if (get_history_freq(hist_index)!=frequency) {
    hist_list = (GtkTreeView*)lookup_widget(app1,"HistList");
    if (hist_list) {
      hist_store = (GtkListStore*)gtk_tree_view_get_model(hist_list);
      time(&s_time);
      gmtime_r(&s_time,&g_time);
      buf = g_strdup_printf("%02d:%02d:%02d;%11.3f",g_time.tm_hour,
                                                    g_time.tm_min,
                                                    g_time.tm_sec,
                                                    frequency);
      text = g_strsplit(buf, ";", 2);
      hist_index++;
      if (hist_index == hist_total) {
        hist_total++;
        gtk_list_store_append(hist_store, &hist_iter);
      } else {
        gtk_list_store_insert(hist_store, &hist_iter, hist_index);
        for (i = hist_total; i > hist_index; --i) {
          gtk_tree_model_iter_nth_child((GtkTreeModel*)hist_store, &hist_iter2, NULL, i);
          gtk_list_store_remove(hist_store, &hist_iter2);
        }  
        hist_total = hist_index+1;
      }
      gtk_list_store_set(hist_store, &hist_iter, 0, text[0], 1, text[1], -1);
      g_free(buf);
      g_strfreev(text);
      if (hist_total > 200) {
        gtk_tree_model_iter_nth_child((GtkTreeModel*)hist_store, &hist_iter2, NULL, 0);
        gtk_list_store_remove(hist_store, &hist_iter2);
        hist_index -= 1;
        hist_total -= 1;
      }
      set_list_selected_row(app1,"HistList",hist_index);
      hist_path = gtk_tree_model_get_path((GtkTreeModel*)hist_store, &hist_iter);
      if (hist_path) {
        gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(hist_list), hist_path, NULL, false, 0, 0);
      }
    }
    set_history_buttons();
  }
}


void
set_history_buttons()
{
  set_enabled(app1,"HistBackBtn",ispower && (hist_index > 0));
  set_enabled(app1,"HistForwBtn",ispower && (hist_index < (hist_total-1)));
}


void
reset_all_radio_parameters()
{
  has_get_freq = TRUE;
  has_get_vfo = TRUE;
  has_set_vfo = TRUE;
  has_get_mode = TRUE;
  has_set_mode = TRUE;
  has_get_power = TRUE;
  has_set_power = TRUE;
  has_noise = TRUE;
  has_noise2 = TRUE;
  has_dnr = TRUE;
  has_atten = TRUE;
  has_preamp = TRUE;
  has_notch = TRUE;
  has_anf = TRUE;
  has_vfo = TRUE;
  has_get_agc = TRUE;
  has_set_agc = TRUE;
  has_get_ant = TRUE;
  has_set_ant = TRUE;
  has_meter = TRUE;
  ispower=FALSE;
  isnoise=FALSE;
  isnoise2=FALSE;
  isdnr=FALSE;
  isnotch=FALSE;
  isanf=FALSE;
  istape=FALSE;
  ismem=FALSE;
  isscan=FALSE;
  istimer=FALSE;
  vfo=1;
  ant=1;
  agc=3;
  bw=2;
  mode=1;
  memch=0;
  cur_atten=0;
  cur_preamp=0;
  frequency=0.0;
  atten_val[0]=0;
  preamp_val[0]=0;
  minf=0;
  maxf=30000000;
  meter_level = 0;
  tune_ok = TRUE;
  hist_index = -1;
  hist_total = 0;
  num_freqs = 0;
  sml_step = 0.01;
  lrg_step = 5.00;
}


void
get_all                                (int do_history)
{
  get_power();
  get_freq();
  if (do_history && ispower)
    add_to_history();
  get_vfo();
  get_ant();
  get_agc();
  get_mode_bw();
  get_blanker();
  get_blanker2();
  get_dnr();
  get_atten();
  get_preamp();
  get_notch();
  get_anf();
  get_af();
  get_rf();
  get_if();
  get_pi();
  get_po();
  get_cp();
  get_sq();
  get_no();
  get_nr();
  get_ap();
  //get_to();
  //get_bwc();
  get_mem();
  set_scan_display();
  set_timer_display();
}


/** power control interface **/
void
check_power()
{
  has_get_power = (hrig->caps->get_powerstat != NULL);
  has_set_power = (hrig->caps->set_powerstat != NULL);
  set_enabled(app1,"PwrBtn",has_set_power);
  if (!has_get_power)
    ispower = TRUE;
}


void
set_power()
{
  powerstat_t	status;

  ispower = !ispower;
  if (ispower)
    status = RIG_POWER_ON;
  else
    status = RIG_POWER_OFF;
  rig_set_powerstat(hrig, status);
  get_all(TRUE);
}


void
get_power()
{
  gint		retcode;
  powerstat_t	pwr1;

  if (has_get_power) {
    retcode = rig_get_powerstat(hrig, &pwr1);
    usleep(1000);
    retcode = rig_get_powerstat(hrig, &pwr1);
    if (retcode == RIG_OK) {
      switch (pwr1) {
        case RIG_POWER_OFF :
          ispower = false;
          break;
        case RIG_POWER_ON :
          ispower = true;
          break;
        case RIG_POWER_STANDBY :
          ispower = false;
          break;
        case RIG_POWER_OPERATE :
          ispower = true;
          break;
        default :
          break;
      }
    } else {
      ispower = false;
      g_warning("Hamlib error: Could not get power status.");
    }
  } //else {
    //ispower = TRUE;
  //}
  set_power_display();
}


void
set_power_display()
{
  set_power_color();
  set_history_buttons();
  set_enabled(app1,"Keypad",ispower);
  set_enabled(app1,"UpDownPad",ispower);
}


/** tuning controls interface **/
void
set_tuner_display			(guint	key_code)
{
  double	r_freq;
  int		len;
  gchar*	o_text;

  if (ispower) {
    if (tune_ok) {
      switch (key_code) {
        case 0 :
        case 1 :
        case 2 :
        case 3 :
        case 4 :
        case 5 :
        case 6 :
        case 7 :
        case 8 :
        case 9 :
          e_text = g_strdup_printf("%d",key_code);
          tune_ok = FALSE;
          break;
        case 10 :
          e_text = g_strdup("0.");
          tune_ok = FALSE;
          break;
        default : 
          break;
      }
    } else {
      if (e_text) {
        int len = strlen(e_text);
        int dot_pos = strcspn(e_text,".");
        switch (key_code) {
          case 0 :
          case 1 :
          case 2 :
          case 3 :
          case 4 :
          case 5 :
          case 6 :
          case 7 :
          case 8 :
          case 9 :
            if (len < 11)
              e_text = g_strdup_printf("%s%d",e_text,key_code);
            break;
          case 10 :
            if (len < 11) {
              if (dot_pos == len)
                e_text = g_strdup_printf("%s%c",e_text,'.');
            }
            break;
          case 11 :
            if (len > 0)
              e_text = g_strndup(e_text,len-1);
            if (len <= 1) 
              tune_ok = TRUE;
            break;
          case 13 :
            if (len > 0) {
              r_freq = strtod(e_text,NULL);
              if ((r_freq >= (minf/1000000.0)) && (r_freq <= (maxf/1000000.0)))
                r_freq = r_freq * 1000.0;
              if ((r_freq >= minf/1000.0) && (r_freq <= maxf/1000.0)) {
                frequency = r_freq;
                tune_ok = TRUE;
              }
            }
            break;
          default :
            break;
        }
        if (tune_ok) {
          g_free(e_text);
          set_freq(TRUE);
        }
      }
    }
    if (!tune_ok) {
      len = strlen(e_text);
      o_text = g_strndup("           ",(11-len));
      o_text = g_strdup_printf("%s%s", o_text, e_text);
      set_freq_display_text(app1, o_text);
    }
  }
}


void
set_tuning_help                        (double amount)
{
  GtkWidget*	gen_item;
  gchar*	gen_dir;
  gchar*	gen_amt;

  gen_item = lookup_widget(app1,"appbar1");
  if (amount < 0.00)
    gen_dir = g_strdup("down");
  else
    gen_dir = g_strdup("up");
  gen_amt = g_strdup_printf("Left mouse btn - tune %s %7.2f kHz, right btn - set stepsize",
    gen_dir,fabs(amount));
  if (amount != 0.00)
    gtk_label_set_text(GTK_LABEL(gen_item),gen_amt);
  else
    gtk_label_set_text(GTK_LABEL(gen_item),"");
  g_free(gen_dir);
  g_free(gen_amt);
}


void
handle_tuning                          (GtkWidget*	in_widget,
                                        GdkEventButton*	in_event,
                                        double		amount)
{
  GtkWidget*	gen_menu;
	
  switch (in_event->button) {
    case 1: // LEFT button - step up or down
      tune_up_down(amount);
      break;
    case 3: // RIGHT button - show menu 
      if (lrg_step == fabs(amount))
        gen_menu = b_popup;
      else
        gen_menu = l_popup;
      if (gen_menu)
        gtk_menu_popup_at_pointer(GTK_MENU(gen_menu),NULL);
      break;
    default:
      break;
  }
}


void
tune_up_down                           (double amount)
{
  gint	retcode;

  if (!ismem) {
    frequency += amount;
    set_freq(fabs(amount) != sml_step);
  } else {
    if (amount > 0) {
      memch++;
      if ((min_mem_edge != -1) && (max_mem_edge != -1)) {
        if (memch == (max_mem_edge+1))
          memch = min_mem_ch;
      }
      if (memch == (max_mem_ch+1)) {
        if ((min_mem_edge != -1) && (max_mem_edge != -1))
          memch = min_mem_edge;
        else
          memch = min_mem_ch;
      }
    } else {
      memch--;
      if (memch == (min_mem_ch-1)) {
        if ((min_mem_edge != -1) && (max_mem_edge != -1))
          memch = max_mem_edge;
        else
          memch = max_mem_ch;
      }
      if ((min_mem_edge != -1) && (max_mem_edge != -1)) {
        if (memch == (min_mem_edge-1))
          memch = max_mem_ch;
      }
    }

    retcode = rig_set_mem(hrig, RIG_VFO_CURR, memch);
    if (retcode != RIG_OK)
      g_warning("Hamlib error: Could not set memory channel.");
    get_all(FALSE);

    set_list_selected_row(mem1,"MemList",memch-min_mem_ch); 
  }
}


void
mem_up_down                            (double amount)
{
  if ((ispower) && (ismem)) {
    tune_up_down(amount);
  }
}


/** frequency control interface **/
void
check_freq()
{
  if (hrig->state.rx_range_list[0].endf != 0) {
    minf = hrig->state.rx_range_list[0].startf;
    maxf = hrig->state.rx_range_list[0].endf;
  } else {
    minf=0;
    maxf=30000000;
  }
  has_get_freq = (hrig->caps->get_freq != NULL);
}


void
set_freq                               (gboolean	do_history)
{
  if (frequency < (minf/1000.0))
    frequency = (maxf/1000.0);
  if (frequency > (maxf/1000.0))
    frequency = (minf/1000.0);
  freq_t freq1 = frequency * 1000;
  rig_set_freq(hrig, RIG_VFO_CURR, freq1);
  if (do_history)
    add_to_history();
  get_freq();
  get_mode_bw();
  //if (get_down(tun1,"FreqLockBtn"))
    //refresh_who_list(NULL);
}


void
get_freq()
{
  gint		retcode;
  freq_t	freq1;

  if (has_get_freq) {
    if (ispower) {
      retcode = rig_get_freq(hrig, RIG_VFO_CURR, &freq1);
      if (retcode != RIG_OK) {
        //freq1 = 100000;
        frequency = 0.0;
        g_warning("Hamlib error: Could not get frequency.");
      } else
        frequency = freq1/1000.0;
    } else
      frequency = 0.0;
  }
  if (ispower && !has_get_power && (frequency == 0.0)) {
    ispower = false;
    set_power_display();
  }
  set_freq_display();
}


gchar*
get_meterband()
{
  int		i;
  gchar*	result;

  if (!ispower)
    result = g_strdup("    ");
  else {
    if (frequency > 30000.0)
      result = g_strdup(" VHF");
    else {
      result = g_strdup(" SW ");
      for (i=89;i>=0;i--) {
        if ((frequency >= plan_text[i].start)&&(frequency <= plan_text[i].end)) {
	  g_free(result);
	  result = g_strdup(plan_text[i].label);
	  break;
	}
      }
    }
  }
  return result;
}

void
set_freq_display_value                 (GtkWidget*	inWidget,
                                        double		freq_val)
{
  gchar*	freqstr=NULL;
  gchar*	mb_str;

  if (ispower) {
    freqstr = g_strdup_printf("%11.3f",freq_val);
    mb_str = get_meterband();
  } else {
    freqstr = g_strdup("           ");
    mb_str = g_strdup("    ");
  }
  set_text(inWidget,"MeterLbl",mb_str);
  g_free(mb_str);
  set_freq_display_text(inWidget,freqstr);
}


void
set_freq_display_text                  (GtkWidget*	inWidget,
                                        gchar*		freq_str)
{
  guint		i;
  gint		src_x;
  gint		dest_x;
  GtkImage*	outWidget;
	
  src_x = 0;
  dest_x = 0;
  outWidget = (GtkImage*) lookup_widget(GTK_WIDGET(inWidget),"freq_display");
  if (outWidget) {
    for(i=0;i<11;i++) {
      switch(freq_str[i]){
	case '0' :
	  src_x = 0*dWidth;
	  break;
	case '1' :
	  src_x = 1*dWidth;
	  break;
  	case '2' :
  	  src_x = 2*dWidth;
  	  break;
  	case '3' :
  	  src_x = 3*dWidth;
  	  break;
	case '4' :
	  src_x = 4*dWidth;
	  break;
	case '5' :
	  src_x = 5*dWidth;
	  break;
	case '6' :
	  src_x = 6*dWidth;
	  break;
	case '7' :
	  src_x = 7*dWidth;
	  break;
	case '8' :
	  src_x = 8*dWidth;
	  break;
	case '9' :
	  src_x = 9*dWidth;
	  break;
	case ' ' :
	  src_x = 10*dWidth;
	  break;
	case '.' :
	  src_x = 11*dWidth;
	  break;
        default :
          break;
      }
      dest_x = i*dWidth;
      gdk_pixbuf_copy_area(digits_pix, src_x, 0, dWidth, dHeight, digit_pix, dest_x, 0);
    }
    g_free(freq_str);
    gtk_image_set_from_pixbuf(GTK_IMAGE(outWidget),digit_pix);
  }
  update_immediate();
}


void
set_freq_display()
{
  set_visible(app1,"MeterLbl",ispower);
  set_visible(app1,"HzLbl",ispower);
  set_freq_display_value(app1,frequency);
  update_freq_db_display();
}

void
update_freq_db_display()
{
  GtkWidget*	gen_item;
  GtkTreeModel*	model;
	
  gen_item = lookup_widget(GTK_WIDGET(app1),"clist1");
  if ((ispower) && (num_freqs > 0)) {
    if (gen_item)
      get_freq_records(GTK_WIDGET(gen_item), frequency);
  } else {
    if (gen_item) {
      model = gtk_tree_view_get_model(GTK_TREE_VIEW(gen_item));
      gtk_list_store_clear(GTK_LIST_STORE(model));
    }
  }
}


/** meter control interface **/
void
check_meter()
{
  has_meter = (rig_has_get_level(hrig,RIG_LEVEL_STRENGTH) != 0);
  set_visible(app1,"meter",has_meter);
}


gboolean
get_meter()
{
  gint		retcode;
  value_t	val;

  if (ispower) {
    if (has_meter) {
      retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_STRENGTH, &val);
      if (retcode != RIG_OK) {
        val.i = 0;
        meter_level = 0;
        g_warning("Hamlib error: Could not get meter.");
      } else {
        if (val.i <= 0) { //convert to S level
          meter_level = 9 + (val.i / 6);
          if (meter_level <= 0)
            meter_level = 1;
        } else { //already at S level use nearest mult of 10
          meter_level = (val.i / 10) * 10;
          if (meter_level == 0)
            meter_level = 9;
        }
      }
    } else
      meter_level = 0;
  } else
    meter_level = 0;
  set_meter_display();
  
  return TRUE;
}


void
set_meter_display_value                (GtkWidget* inWidget)
{
  gint		src_x;
  GtkImage*	outWidget;
	
  src_x = 0;
  outWidget = (GtkImage*) lookup_widget(GTK_WIDGET(inWidget),"meter");

  switch(meter_level) {
    case 1 :
    case 2 :
    case 3 :
    case 4 :
    case 5 :
    case 6 :
    case 7 :
    case 8 :
    case 9 :
    case 10 :
      src_x = meter_level*mWidth;
      break;
    case 20 :
      src_x = 11*mWidth;
      break;
    case 30 :
      src_x = 12*mWidth;
      break;
    case 40 :
      src_x = 13*mWidth;
      break;
    case 50 :
      src_x = 14*mWidth;
      break;
    case 60 :
      src_x = 15*mWidth;
      break;
    default :
      break;
  }
  gdk_pixbuf_copy_area(meters_pix, src_x, 0, mWidth, mHeight, meter_pix, 0, 0);
  gtk_image_set_from_pixbuf(GTK_IMAGE(outWidget),meter_pix);
  
  update_immediate();
}


void
set_meter_display()
{
  set_visible(app1,"meter",has_meter);
  set_meter_display_value(app1);
}


/** vfo control interface **/
void
check_vfo()
{
  GtkWidget*	opt_menu;
  GtkListStore*	opt_menu_menu;
  GtkTreeIter	iter;
  gchar*	m_item;
  int		i;
  int		num_vfos;
  int		vfo_num;

  has_get_vfo = (hrig->caps->get_vfo != NULL);
  has_set_vfo = (hrig->caps->set_vfo != NULL);
  num_vfos = hrig->state.vfo_list;
  num_vfos &= 0x3;
  has_vfo = (num_vfos > 1);
  set_enabled(app1,"VFOBox",(has_vfo && has_set_vfo));
  opt_menu = lookup_widget(app1,"VFOBox");
  if (opt_menu) {
    opt_menu_menu = gtk_list_store_new(1, G_TYPE_STRING, -1);       
    gtk_list_store_append(opt_menu_menu, &iter);
    gtk_list_store_set(opt_menu_menu, &iter, 0, " ", -1);
    for (i = 0; i < 8; i++) {
      vfo_num = num_vfos;
      vfo_num &= (1<<i);
      if (vfo_num > 0) {
        m_item = g_strdup_printf("%c",(0x41+i));
        gtk_list_store_append(opt_menu_menu, &iter);
        gtk_list_store_set(opt_menu_menu, &iter, 0, m_item, -1);
        g_free(m_item);
      }
    } 
    gtk_combo_box_set_model(GTK_COMBO_BOX(opt_menu),(GtkTreeModel*)opt_menu_menu);
    if (has_vfo)
      g_signal_connect (G_OBJECT (opt_menu), "changed", 
                        G_CALLBACK (on_vfo_selected), NULL);
    else {
      gtk_list_store_append(opt_menu_menu, &iter);
      gtk_list_store_set(opt_menu_menu, &iter, 0, "A", -1);
    }
  }
}


void
on_vfo_selected                        (GtkWidget*	opt_menu,
                                        gpointer        data)
{
  gint item_index = gtk_combo_box_get_active((GtkComboBox*) opt_menu);
  if (item_index != 0)
	set_vfo(item_index);
}


void
set_vfo                                (gint	item_index)
{
  vfo_t		vfo1 = 0;

  if (has_set_vfo) {
    vfo = item_index;
    switch (vfo) {
      case 1 :
        vfo1 = RIG_VFO_A;
        break;
      case 2 :
        vfo1 = RIG_VFO_B;
        break;
      case 3 :
        vfo1 = RIG_VFO_C;
        break;
      default :
        break;
    }
    rig_set_vfo(hrig, vfo1);
    get_all(FALSE);
  }
}


void
get_vfo()
{
  gint			retcode;
  vfo_t			vfo1;

  if (has_get_vfo) {
    if (ispower) {
      if (has_vfo) {
        retcode = rig_get_vfo(hrig, &vfo1);
        if (retcode != RIG_OK) {
          vfo1 = RIG_VFO_A;
          g_warning("Hamlib error: Could not get vfo.");
        } else {
          switch (vfo1 & 0x03) {
            case RIG_VFO_NONE :
              vfo = 0;
              break;
            case RIG_VFO_A :
              vfo = 1;
              break;
            case RIG_VFO_B :
              vfo = 2;
              break;
            case RIG_VFO_C :
              vfo = 3;
              break;
            default : break;
          }
        }
        ismem = ((vfo1 & RIG_VFO_MEM)==RIG_VFO_MEM);
      } else
        vfo = 0;
    } else
      vfo = 0;
  } else
    vfo = RIG_VFO_CURR;
  set_vfo_display();
  set_mem_display();
}


void
set_vfo_display()
{
  set_visible(app1,"VFOLbl",ispower);
  set_option(app1,"VFOBox", ispower ? vfo : 0);
  set_enabled(app1,"VFOBox",(ispower && has_vfo && has_set_vfo));
}

/** antenna control interface **/
void
check_ant()
{
  GtkWidget*	opt_menu;

  has_get_ant = (hrig->caps->get_ant != NULL);
  has_set_ant = (hrig->caps->set_ant != NULL);
  opt_menu = lookup_widget(app1,"AntBox");
  if (opt_menu) {
    g_signal_connect (G_OBJECT (opt_menu), "changed", 
                      G_CALLBACK (on_ant_selected), NULL);
  }
}


void
on_ant_selected                        (GtkWidget*	opt_menu,
                                        gpointer        data)
{
  gint item_index = gtk_combo_box_get_active((GtkComboBox*) opt_menu);
  if (item_index != 0)
    set_ant(item_index);
}


void
set_ant                                (gint	item_index)
{
  ant_t		ant1 = 0;
  value_t	option;

  option.cs="";
  option.f=0.0;
  option.i=0;
  option.s=NULL;
  if (has_set_ant) {
    ant = item_index;
    switch (ant) {
      case 1 :
        ant1 = RIG_ANT_1;
        break;
      case 2 :
        ant1 = RIG_ANT_2;
        break;
      case 3 :
        ant1 = RIG_ANT_3;
        break;
      default :
        break;
    }
    rig_set_ant(hrig, RIG_VFO_CURR, ant1, option);
    get_ant();
  }
}


void
get_ant()
{
  gint			retcode;
  ant_t			ant1;
  value_t		option;
  ant_t			ant_tx;
  ant_t			ant_rx;

  if (ispower) {
    if (has_get_ant) {
      retcode = rig_get_ant(hrig, RIG_VFO_CURR, RIG_ANT_CURR, &option, &ant1, &ant_tx, &ant_rx);
      if (retcode != RIG_OK) {
        ant1 = RIG_ANT_1;
        g_warning("Hamlib error: Could not get antenna.");
      } else {
        switch (ant1) {
          case RIG_ANT_NONE :
            ant = 0;
            break;
          case RIG_ANT_1 :
            ant = 1;
            break;
          case RIG_ANT_2 :
            ant = 2;
            break;
          case RIG_ANT_3 :
            ant = 3;
            break;
          default :
            break;
        }
      }
    } else
      ant = 1;
  } else
    ant = 0;
  set_ant_display();
}


void
set_ant_display()
{
  set_visible(app1,"AntLbl",ispower);
  set_option(app1,"AntBox", ispower ? ant : 0);
  set_enabled(app1,"AntBox",ispower&&has_set_ant);
}


/** agc control interface **/
void
check_agc()
{
 GtkWidget*	opt_menu;

  has_get_agc = (rig_has_get_level(hrig, RIG_LEVEL_AGC) != 0);
  has_set_agc = (rig_has_set_level(hrig, RIG_LEVEL_AGC) != 0);
  opt_menu = lookup_widget(app1,"AGCBox");
  if (opt_menu) {
    g_signal_connect (G_OBJECT (opt_menu), "changed", 
                      G_CALLBACK (on_agc_selected), NULL);
  }
}


void
on_agc_selected                        (GtkWidget*	opt_menu,
                                        gpointer        data)
{
  gint item_index = gtk_combo_box_get_active((GtkComboBox*) opt_menu);
  if (item_index != 0)
    set_agc(item_index);
}


void
set_agc                                (gint	item_index)
{
  value_t	agc1;

  if (has_set_agc) {
    agc = item_index;
    switch (agc) {
      case 1 :
        agc1.i = RIG_AGC_OFF;
        break;
      case 2 :
        agc1.i = RIG_AGC_SLOW;
        break;
      case 3 :
        agc1.i = RIG_AGC_MEDIUM;
        break;
      case 4 :
        agc1.i = RIG_AGC_FAST;
        break;
      case 5 :
        agc1.i = RIG_AGC_SUPERFAST;
        break;
      default :
        agc1.i = RIG_AGC_OFF;
        break;
    }
    rig_set_level(hrig, RIG_VFO_CURR, RIG_LEVEL_AGC, agc1);
    get_agc();
  }
}


void
get_agc()
{
  gint		retcode;
  value_t	val;

  if (ispower && has_get_agc) {
    retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_AGC, &val);
    if (retcode != RIG_OK) {
      val.i = RIG_AGC_OFF;
      g_warning("Hamlib error: Could not get AGC.");
    } else {
      switch (val.i) {
        case RIG_AGC_OFF :
          agc = 1;
          break;
        case RIG_AGC_SLOW :
          agc = 2;
          break;
        case RIG_AGC_MEDIUM :
          agc = 3;
          break;
        case RIG_AGC_FAST :
          agc = 4;
          break;
        case RIG_AGC_SUPERFAST :
          agc = 5;
          break;
        default :
          break;
      }
    }
  }
  else
    agc = 0;
  set_agc_display();
}


void
set_agc_display()
{
  set_visible(app1,"AGCLbl",ispower);
  set_option(app1,"AGCBox", ispower ? agc : 0);
  set_enabled(app1,"AGCBox",ispower&&has_set_agc);
}


/** mode and bandwidth controls interface **/
gint
bwlist_compare_func                    (gconstpointer s1,
                                        gconstpointer s2)
{
  double num1 = GPOINTER_TO_INT(s1)/1000.0;
  double num2 = GPOINTER_TO_INT(s2)/1000.0;
  gchar *d1;
  gchar *d2;
  gint result;

  d1 = g_strdup_printf("%4.1f",num1);
  d2 = g_strdup_printf("%4.1f",num2);
  result = strcmp(d2, d1);
  g_free(d1);
  g_free(d2);

  return result;
}


void
check_bw()
{
  GtkWidget*	opt_menu;
  GtkListStore*	opt_menu_menu;
  GtkTreeIter	iter;
  gchar*	m_item;
  int		i=0;

  has_get_mode = (hrig->caps->get_mode != NULL);
  has_set_mode = (hrig->caps->set_mode != NULL);
  if (bw_list != NULL) {
    g_list_free(bw_list);
    bw_list = NULL;
  }
  while (hrig->state.filters[i].width != 0) {
    gint pbw = hrig->state.filters[i].width;
    if ((pbw > 0) && (g_list_find(bw_list,GINT_TO_POINTER(pbw))==NULL))
      bw_list = g_list_insert_sorted(bw_list,GINT_TO_POINTER(pbw),
                                    (GCompareFunc)bwlist_compare_func);
    i++;
  }
  opt_menu = lookup_widget(app1,"BWBox");
  if (opt_menu) {
    opt_menu_menu = gtk_list_store_new(1, G_TYPE_STRING, -1);       
    gtk_list_store_append(opt_menu_menu, &iter);
    gtk_list_store_set(opt_menu_menu, &iter, 0, " ", -1);
    for (i = 0; i < g_list_length(bw_list); i++) {
      double pbwd = GPOINTER_TO_INT(g_list_nth_data(bw_list,i))/1000.0;
      m_item = g_strdup_printf("%4.1f",pbwd);
      gtk_list_store_append(opt_menu_menu, &iter);
      gtk_list_store_set(opt_menu_menu, &iter, 0, m_item, -1);
      g_free(m_item);
    }
    gtk_combo_box_set_model(GTK_COMBO_BOX(opt_menu),(GtkTreeModel*)opt_menu_menu);
    g_signal_connect (G_OBJECT (opt_menu), "changed", 
                      G_CALLBACK (on_bw_selected), NULL);
  }
}


void
on_bw_selected                         (GtkWidget*	opt_menu,
					gpointer        data)
{
  gint item_index = gtk_combo_box_get_active((GtkComboBox*) opt_menu);
  if (item_index != 0)
    set_bw(item_index);
}


void
set_bw                                 (gint	item_index)
{
  pbwidth_t	width1 = 0;
  rmode_t	mode1;

  if (has_set_mode) {
    bw = item_index;

    mode1 = GPOINTER_TO_INT(g_list_nth_data(mode_list,mode-1));
    width1 = GPOINTER_TO_INT(g_list_nth_data(bw_list,bw-1));

    rig_set_mode(hrig, RIG_VFO_CURR, mode1, width1);
    get_mode_bw();
    get_atten();
    get_preamp();
  }
}


void
set_bw_display()
{
  set_visible(app1,"BWLbl",ispower);
  set_option(app1,"BWBox", ispower ? bw : 0);
  set_enabled(app1,"BWBox",ispower&&has_set_mode);
}


gint
modelist_compare_func                  (gconstpointer s1,
                                        gconstpointer s2)
{
  double num1 = GPOINTER_TO_INT(s1);
  double num2 = GPOINTER_TO_INT(s2);
  gchar *d1;
  gchar *d2;
  gint result;

  d1 = g_strdup_printf("%16.0f",num1);
  d2 = g_strdup_printf("%16.0f",num2);
  result = strcmp(d1, d2);
  g_free(d1);
  g_free(d2);

  return result;
}


gchar*
get_mode_str                           (gint	in_mode)
{
    rmode_t	mode1;
    gchar*	m_item;
	
  mode1 = GPOINTER_TO_INT(g_list_nth_data(mode_list,in_mode-1));
  switch(mode1){
    case RIG_MODE_AM :
      m_item = strdup("AM");
      break;
    case RIG_MODE_CW :
      m_item = strdup("CW");
      break;
    case RIG_MODE_USB :
      m_item = strdup("USB");
      break;
    case RIG_MODE_LSB :
      m_item = strdup("LSB");
      break;
    case RIG_MODE_RTTY :
      m_item = strdup("RTTY");
      break;
    case RIG_MODE_FM :
      m_item = strdup("FM");
      break;
    case RIG_MODE_WFM :
      m_item = strdup("FM-Wide");
      break;
    case RIG_MODE_CWR :
      m_item = strdup("CW-R");
      break;
    case RIG_MODE_RTTYR :
      m_item = strdup("RTTY-R");
      break;
    case RIG_MODE_AMS :
      m_item = strdup("AMS");
      break;
    case RIG_MODE_PKTLSB :
      m_item = strdup("Packet-LSB");
      break;
    case RIG_MODE_PKTUSB :
      m_item = strdup("Packet-USB");
      break;
    case RIG_MODE_PKTFM :
      m_item = strdup("PKT-FM");
      break;
    case RIG_MODE_FAX :
      m_item = strdup("FAX");
      break;
    case RIG_MODE_ECSSLSB :
      m_item = strdup("ECSS-LSB");
      break;
    case RIG_MODE_ECSSUSB :
      m_item = strdup("ECSS-USB");
      break;
    default :
      m_item = strdup(" ");
      break;
  }
  return m_item;
}


void
check_mode()
{
  GtkWidget*	opt_menu;
  GtkListStore*	opt_menu_menu;
  GtkTreeIter	iter;
  gchar*	m_item;
  int		i = 0;
  int		j = 0;

  if (mode_list != NULL) {
    g_list_free(mode_list);
    mode_list = NULL;
  }
  while (hrig->state.filters[i].modes != 0) {
    gint mw = hrig->state.filters[i].modes;
    for (j = 0; j < 16; j++) {
      gint mode_num = mw;
      mode_num &= (1<<j);
      if ((mode_num > 0) && (g_list_find(mode_list,GINT_TO_POINTER(mode_num))==NULL))
        mode_list = g_list_insert_sorted(mode_list,GINT_TO_POINTER(mode_num),
                                        (GCompareFunc)modelist_compare_func);
    }
    i++;
  }
  opt_menu = lookup_widget(app1,"ModeBox");
  if (opt_menu) {
    opt_menu_menu = gtk_list_store_new(1, G_TYPE_STRING, -1);       
    gtk_list_store_append(opt_menu_menu, &iter);
    gtk_list_store_set(opt_menu_menu, &iter, 0, " ", -1);
    for ( i = 0; i < g_list_length(mode_list); i++) {
      int mwd = GPOINTER_TO_INT(g_list_nth_data(mode_list,i));
      switch(mwd){
        case RIG_MODE_AM :
          m_item = strdup("AM");
          break;
        case RIG_MODE_CW :
          m_item = strdup("CW");
          break;
        case RIG_MODE_USB :
          m_item = strdup("USB");
          break;
        case RIG_MODE_LSB :
          m_item = strdup("LSB");
          break;
        case RIG_MODE_RTTY :
          m_item = strdup("RTTY");
          break;
        case RIG_MODE_FM :
          m_item = strdup("FM");
          break;
        case RIG_MODE_WFM :
          m_item = strdup("FM-Wide");
          break;
        case RIG_MODE_CWR :
          m_item = strdup("CW-R");
          break;
        case RIG_MODE_RTTYR :
          m_item = strdup("RTTY-R");
          break;
        case RIG_MODE_AMS :
          m_item = strdup("AMS");
          break;
        case RIG_MODE_PKTLSB :
          m_item = strdup("Packet-LSB");
          break;
        case RIG_MODE_PKTUSB :
          m_item = strdup("Packet-USB");
          break;
        case RIG_MODE_PKTFM :
          m_item = strdup("PKT-FM");
          break;
        case RIG_MODE_FAX :
          m_item = strdup("FAX");
          break;
        case RIG_MODE_ECSSLSB :
          m_item = strdup("ECSS-LSB");
          break;
        case RIG_MODE_ECSSUSB :
          m_item = strdup("ECSS-USB");
          break;
        default :
          m_item = strdup(" ");
          break;
      }
      gtk_list_store_append(opt_menu_menu, &iter);
      gtk_list_store_set(opt_menu_menu, &iter, 0, m_item, -1);
      g_free(m_item);
    }
    gtk_combo_box_set_model(GTK_COMBO_BOX(opt_menu),(GtkTreeModel*)opt_menu_menu);
    g_signal_connect (G_OBJECT (opt_menu), "changed", 
                      G_CALLBACK (on_mode_selected), NULL);
  }
}


void
on_mode_selected                        (GtkWidget*	opt_menu,
                                         gpointer        data)
{
  gint item_index = gtk_combo_box_get_active((GtkComboBox*) opt_menu);
  if (item_index != 0)
    set_mode(item_index);
}


void
set_mode                               (gint	item_index)
{
  if (has_set_mode) {
    mode = item_index;
    set_bw((gint)bw);
  }
}


void
set_mode_display()
{
  set_visible(app1,"ModeLbl",ispower);
  set_option(app1,"ModeBox", ispower ? mode : 0);
  set_enabled(app1,"ModeBox",ispower&&has_set_mode);
}


void
get_mode_bw()
{
  gint		retcode;
  rmode_t	mode1;
  pbwidth_t	width1;
  int		i;

  if (ispower && has_get_mode) {
    retcode = rig_get_mode(hrig, RIG_VFO_CURR, &mode1, &width1);
    if (retcode != RIG_OK) {
      mode1 = 1;
      width1 = rig_passband_normal(hrig,mode1);
      g_warning("Hamlib error: Could not get mode and bandwidth.");
    } else {
      mode = 0;
      for (i = 0; i < g_list_length(mode_list); i++) {
        if (mode1 == GPOINTER_TO_INT(g_list_nth_data(mode_list,i)))
          mode = i+1;
      }
      bw = 0;
      for (i = 0; i < g_list_length(bw_list); i++) {
        if (width1 == GPOINTER_TO_INT(g_list_nth_data(bw_list,i)))
          bw = i + 1;
      }
    }
  } else {
    mode = 0;
    bw = 0;
  }
  set_bw_display();
  set_mode_display();
}


/** noise blanker control interface **/
void
check_blanker()
{
  has_noise = (rig_has_get_func(hrig, RIG_FUNC_NB) != 0);
  set_visible(app1,"NoiseBtn",has_noise);
  set_visible(app1,"noise_blanker1",has_noise);
}


void
check_blanker2()
{
  has_noise2 = (rig_has_get_func(hrig, RIG_FUNC_NB2) != 0);
  set_visible(app1,"NoiseBtn2",has_noise2);
  set_visible(app1,"noise_blanker2",has_noise2);
  if (has_noise2)
    set_text(app1,"noise_blanker1","Noise _blanker 1");
  else
    set_text(app1,"noise_blanker1","Noise _blanker");  
}


void
set_blanker()
{
  int	status;

  if (has_noise) {
    isnoise = !isnoise;
    if (isnoise)
      status = 1;
    else
      status = 0;
    rig_set_func(hrig, RIG_VFO_CURR, RIG_FUNC_NB, status);
    get_blanker();
  }
}


void
set_blanker2()
{
  int	status;

  if (has_noise2) {
    isnoise2 = !isnoise2;
    if (isnoise2)
      status = 1;
    else
      status = 0;
    rig_set_func(hrig, RIG_VFO_CURR, RIG_FUNC_NB2, status);
    get_blanker2();
  }
}


void
get_blanker()
{
  gint	retcode;
  int	status;

  if (ispower && has_noise) {
    retcode = rig_get_func(hrig, RIG_VFO_CURR, RIG_FUNC_NB, &status);
    if (retcode != RIG_OK) {
      isnoise = FALSE;
      g_warning("Hamlib error: Could not get noise blanker.");
    } else
      isnoise = (status == 1);
  } else
    isnoise = FALSE;
  set_blanker_display();
  if (isnoise && isnoise2) {
    isnoise2 = FALSE;
    set_blanker2_display();
  }
}


void
get_blanker2()
{
  gint	retcode;
  int	status;

  if (ispower && has_noise2) {
    retcode = rig_get_func(hrig, RIG_VFO_CURR, RIG_FUNC_NB2, &status);
    if (retcode != RIG_OK) {
      isnoise2 = FALSE;
      g_warning("Hamlib error: Could not get noise blanker 2.");
    } else
      isnoise2 = (status == 1);
  } else
    isnoise2 = FALSE;
  set_blanker2_display();
  if (isnoise2 && isnoise) {
    isnoise = FALSE;
    set_blanker_display();
  }
}


void
set_blanker_display()
{
  set_visible(app1,"NoiseLbl",ispower && ((isnoise && has_noise) || (isnoise2 && has_noise2)));
  if (has_noise) {
    set_enabled(app1,"NoiseBtn",ispower);
    set_down(app1,"NoiseBtn",ispower&&isnoise);
    set_enabled(app1,"noise_blanker1",ispower);
  }
}


void
set_blanker2_display()
{
  set_visible(app1,"NoiseLbl",ispower && ((isnoise && has_noise) || (isnoise2 && has_noise2)));
  if (has_noise2) {
    set_enabled(app1,"NoiseBtn2",ispower);
    set_down(app1,"NoiseBtn2",ispower&&isnoise2);
    set_enabled(app1,"noise_blanker2",ispower);
  }
}


/** noise reduction control interface **/
void
check_dnr()
{
    has_dnr = (rig_has_get_func(hrig,RIG_FUNC_NR) != 0);
    set_visible(app1,"DNRBtn",has_dnr);
    set_visible(app1,"noise_reduction1",has_dnr);
}


void
set_dnr()
{
  int	status;

  if (has_dnr) {
    isdnr = !isdnr;
    if (isdnr)
      status = 1;
    else
      status = 0;
    rig_set_func(hrig, RIG_VFO_CURR, RIG_FUNC_NR, status);
    get_dnr();
  }
}


void
get_dnr()
{
  gint	retcode;
  int	status;

  if (ispower && has_dnr) {
    retcode = rig_get_func(hrig, RIG_VFO_CURR, RIG_FUNC_NR, &status);
    if (retcode != RIG_OK) {
      isdnr = FALSE;
      g_warning("Hamlib error: Could not get noise reduction.");
    } else
      isdnr = (status == 1);
  } else
    isdnr = FALSE;
  set_dnr_display();
}


void
set_dnr_display()
{
  set_visible(app1,"DNRLbl",ispower&&isdnr&&has_dnr);
  if (has_dnr) {
    set_enabled(app1,"DNRBtn",ispower);
    set_down(app1,"DNRBtn",ispower&&isdnr);
    set_enabled(app1,"noise_reduction1",ispower);
  }
}


/** attenuator control interface **/
void
check_atten()
{
  int	i=0;

  num_attens = 0;
  has_atten = (rig_has_get_level(hrig,RIG_LEVEL_ATT) != 0);
  set_visible(app1,"AttBtn",has_atten);
  set_visible(app1,"attenuator1",has_atten);
  if (has_atten) {
    i = 0;
    atten_val[0] = 0;
    while (hrig->state.attenuator[i] != 0) {
      atten_val[i+1] = hrig->state.attenuator[i];
      i++;
    }
    num_attens = i;
  }
  if (num_attens <= 1)
    set_text(app1,"AttLbl","ATT");
}


void
set_atten()
{
  value_t	status;

  if (has_atten) {
    cur_atten++;
    if (cur_atten > num_attens)
      cur_atten = 0;
    status.i = atten_val[cur_atten];
    rig_set_level(hrig, RIG_VFO_CURR, RIG_LEVEL_ATT, status);
    get_atten();
    get_preamp();
  }
}


void
get_atten()
{
  int		i;
  gint		retcode;
  value_t	val;

  if (ispower && has_atten) {
    retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_ATT, &val);
    if (retcode != RIG_OK) {
      cur_atten = 0;
      g_warning("Hamlib error: Could not get attenuator.");
    } else
      for (i = 0; i <= num_attens; i++) {
        if (val.i == atten_val[i])
          cur_atten = i;
      }
  } else
    cur_atten = 0;
  set_atten_display();
}


void
set_atten_display()
{
  set_visible(app1,"AttLbl",ispower&&(cur_atten > 0)&&has_atten);
  if (has_atten) {
    if (num_attens > 1) {
      gchar* atext = g_strdup_printf("AT%d",cur_atten);
      set_text(app1,"AttLbl",atext);
      g_free(atext);
    }
    set_enabled(app1,"AttBtn",ispower);
    set_down(app1,"AttBtn",ispower&&(cur_atten > 0));
    set_enabled(app1,"attenuator1",ispower);
  }
}


/** preamp control interface **/
void
check_preamp()
{
  int	i;

  num_preamps = 0;
  has_preamp = (rig_has_get_level(hrig,RIG_LEVEL_PREAMP) != 0);
  set_visible(app1,"PreampBtn",has_preamp);
  set_visible(app1,"preamp1",has_preamp);
  if (has_preamp) {
    i = 0;
    preamp_val[0] = 0;
    while (hrig->state.preamp[i] != 0) {
      preamp_val[i+1] = hrig->state.preamp[i];
      i++;
    }
    num_preamps = i;
  }
  if (num_preamps <= 1)
    set_text(app1,"PreampLbl","PRE");
}


void
set_preamp()
{
  value_t	status;

  if (has_preamp) {
    cur_preamp++;
    if (cur_preamp > num_preamps)
      cur_preamp = 0;
    status.i = preamp_val[cur_preamp];
    rig_set_level(hrig, RIG_VFO_CURR, RIG_LEVEL_PREAMP, status);
    get_preamp();
    get_atten();
  }
}


void
get_preamp()
{
  int		i;
  gint		retcode;
  value_t	val;

  if (ispower && has_preamp) {
    retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_PREAMP, &val);
    if (retcode != RIG_OK) {
      cur_preamp = 0;
      g_warning("Hamlib error: Could not get preamp.");
    } else
      for (i = 0; i <= num_preamps; i++) {
        if (val.i == preamp_val[i])
          cur_preamp = i;
      }
  } else
    cur_preamp = 0;
  set_preamp_display();
}


void
set_preamp_display()
{
  set_visible(app1,"PreampLbl",ispower&&(cur_preamp > 0)&&has_preamp);
  if (has_preamp) {
    if (num_preamps > 1) {
      gchar* ptext = g_strdup_printf("PR%d",cur_preamp);
      set_text(app1,"PreampLbl",ptext);
      g_free(ptext);
    }
    set_enabled(app1,"PreampBtn",ispower);
    set_down(app1,"PreampBtn",ispower&&(cur_preamp > 0));
    set_enabled(app1,"preamp1",ispower);
  }
}


/** notch control interface **/
void
check_notch()
{
  has_notch = (rig_has_get_func(hrig,RIG_FUNC_MN) != 0);
  set_visible(app1,"NotchBtn",has_notch);
  set_visible(app1,"notch1",has_notch);
}


void
set_notch()
{
  int	status;

  if (has_notch) {
    isnotch = !isnotch;
    if (isnotch)
      status = 1;
    else
      status = 0;
    rig_set_func(hrig, RIG_VFO_CURR, RIG_FUNC_MN, status);
    get_notch();
  }
}


void
get_notch()
{
  gint		retcode;
  int		status;

  if (ispower && has_notch) {
    retcode = rig_get_func(hrig, RIG_VFO_CURR, RIG_FUNC_MN, &status);
    if (retcode != RIG_OK) {
      isnotch = FALSE;
      g_warning("Hamlib error: Could not get notch.");
    } else
      isnotch = (status == 1);
  } else
    isnotch = FALSE;
  set_notch_display();
}


void
set_notch_display()
{
  set_visible(app1,"NotchLbl",ispower&&isnotch&&has_notch);
  if (has_notch) {
    set_enabled(app1,"NotchBtn",ispower);
    set_down(app1,"NotchBtn",ispower&&isnotch);
    set_enabled(app1,"notch1",ispower);
  }
}


/** automatic notch filter control interface **/
void
check_anf()
{
  has_anf = (rig_has_get_func(hrig,RIG_FUNC_ANF) != 0);
  set_visible(app1,"ANFBtn",has_anf);
  set_visible(app1,"auto_notch1",has_anf);
}


void
set_anf()
{
  int	status;

  if (has_anf) {
    isanf = !isanf;
    if (isanf)
      status = 1;
    else
      status = 0;
    rig_set_func(hrig, RIG_VFO_CURR, RIG_FUNC_ANF, status);
    get_anf();
  }
}


void
get_anf()
{
  gint		retcode;
  int		status;

  if (ispower && has_anf) {
    retcode = rig_get_func(hrig, RIG_VFO_CURR, RIG_FUNC_ANF, &status);
    if (retcode != RIG_OK) {
      isanf = FALSE;
      g_warning("Hamlib error: Could not get automatic notch filter.");
    } else
      isanf = (status == 1);
  } else
    isanf = FALSE;
  set_anf_display();
}


void
set_anf_display()
{
  set_visible(app1,"ANFLbl",ispower&&isanf&&has_anf);
  if (has_anf) {
    set_enabled(app1,"ANFBtn",ispower);
    set_down(app1,"ANFBtn",ispower&&isanf);
    set_enabled(app1,"auto_notch1",ispower);
  }
}


/** AF level control interface **/
void
check_af()
{
  af_range.min.f = 0.0;
  af_range.max.f = 1.0;
  af_range.step.f = 0.01;
  has_af = (rig_has_get_level(hrig,RIG_LEVEL_AF) != 0);
  set_visible(app1,"AFBox",has_af);
  if (has_af) {
    if ((hrig->state.level_gran[3].min.f != 0.0) &&
        (hrig->state.level_gran[3].max.f != 0.0)) {
      af_range.min.f = hrig->state.level_gran[3].min.f;
      af_range.max.f = hrig->state.level_gran[3].max.f;
      af_range.step.f = hrig->state.level_gran[3].step.f;
    }
    if (af_range.step.f == 0.0)
      af_range.step.f = (af_range.max.f - af_range.min.f)/100;
    set_scale_range(app1,"AFScale",af_range,TRUE);
  }
}


void
set_af                                 (GtkAdjustment*	adj,
                                        gpointer	gptr)
{
  value_t	val;

  if (has_af) {
    val.f = -get_scale_value(app1,"AFScale");
    if (val.f != afval) {
      rig_set_level(hrig, RIG_VFO_CURR, RIG_LEVEL_AF, val);
      get_af();
    }
  }
}


void
get_af()
{
  gint		retcode;
  value_t	val;

  if (ispower && has_af) {
    retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_AF, &val);
    if (retcode != RIG_OK) {
      afval = 0.0;
      g_warning("Hamlib error: Could not get af level.");
    } else
      afval = val.f;
  }
  else
    afval = 0.0;
  set_af_display();
}


void
set_af_display()
{
  if (has_af) {
    gchar* afstr = g_strdup_printf("%03d",(int)(afval*100.0));
    set_text(app1,"AFAmt",afstr);
    set_scale_value(app1,"AFScale",-afval);
    g_free(afstr);
  }
}


/** RF level control interface **/
void
check_rf()
{
  rf_range.min.f = 0.0;
  rf_range.max.f = 1.0;
  rf_range.step.f = 0.01;
  has_rf = (rig_has_get_level(hrig,RIG_LEVEL_RF) != 0);
  set_visible(app1,"RFBox",has_rf);
  if (has_rf) {
    if ((hrig->state.level_gran[4].min.f != 0.0) &&
        (hrig->state.level_gran[4].max.f != 0.0)) {
      rf_range.min.f = hrig->state.level_gran[4].min.f;
      rf_range.max.f = hrig->state.level_gran[4].max.f;
      rf_range.step.f = hrig->state.level_gran[4].step.f;
    }
    if (rf_range.step.f == 0.0)
      rf_range.step.f = (rf_range.max.f - rf_range.min.f)/100;
    set_scale_range(app1,"RFScale",rf_range,TRUE);
  }
}


void
set_rf                                 (GtkAdjustment*	adj,
                                        gpointer	gptr)
{
  value_t	val;

  if (has_rf) {
    val.f = -get_scale_value(app1,"RFScale");
    if (val.f != rfval) {
      rig_set_level(hrig, RIG_VFO_CURR, RIG_LEVEL_RF, val);
      get_rf();
    }
  }
}


void
get_rf()
{
  gint		retcode;
  value_t	val;

  if (ispower && has_rf) {
    retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_RF, &val);
    if (retcode != RIG_OK) {
      rfval = 0.0;
      g_warning("Hamlib error: Could not get rf level.");
    } else
      rfval = val.f;
  } else
    rfval = 0.0;
  set_rf_display();
}


void
set_rf_display()
{
  if (has_rf) {
    gchar* rfstr = g_strdup_printf("%03d",(int)(rfval*100.0));
    set_text(app1,"RFAmt",rfstr);
    set_scale_value(app1,"RFScale",-rfval);
    g_free(rfstr);
  }
}


/** IF level control interface **/
void
check_if()
{
  if_range.min.i = -2000;
  if_range.max.i = +2000;
  if_range.step.i = 10;
  has_if = (rig_has_get_level(hrig,RIG_LEVEL_IF) != 0);
  set_visible(app1,"IFBox",has_if);
  if (has_if) {
    if ((hrig->state.level_gran[6].min.i != 0) &&
        (hrig->state.level_gran[6].max.i != 0)) {
      if_range.min.i = hrig->state.level_gran[6].min.i;
      if_range.max.i = hrig->state.level_gran[6].max.i;
      if_range.step.i = hrig->state.level_gran[6].step.i;
    }
    if (if_range.step.i == 0.0)
      if_range.step.i = (if_range.max.i - if_range.min.i)/100;
    set_scale_range(app1,"IFScale",if_range,FALSE);
  }
}


void
set_if                                 (GtkAdjustment*	adj,
                                        gpointer	gptr)
{
  value_t	val;

  if (has_if) {
    val.i = -get_scale_value(app1,"IFScale");
    if (val.i != ifval) {
      rig_set_level(hrig, RIG_VFO_CURR, RIG_LEVEL_IF, val);
      get_if();
    }
  }
}


void
get_if()
{
  gint		retcode;
  value_t	val;

  if (ispower && has_if) {
    retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_IF, &val);
    if (retcode != RIG_OK) {
      ifval = 0.0;
      g_warning("Hamlib error: Could not get if level.");
    } else
      ifval = val.i;
  } else
    ifval = 0.0;
  set_if_display();
}


void
set_if_display()
{
  if (has_if) {
    gchar* ifstr = g_strdup_printf("%+05d",(int)ifval);
    set_text(app1,"IFAmt",ifstr);
    set_scale_value(app1,"IFScale",-ifval);
    g_free(ifstr);
  }
}


/** PBT IN level control interface **/
void
check_pi()
{
  pi_range.min.f = 0.0;
  pi_range.max.f = 1.0;
  pi_range.step.f = 0.01;
  has_pi = (rig_has_get_level(hrig,RIG_LEVEL_PBT_IN) != 0);
  set_visible(app1,"PIBox",has_pi);
  if (has_pi) {
    if ((hrig->state.level_gran[9].min.f != 0.0) &&
        (hrig->state.level_gran[9].max.f != 0.0)) {
      pi_range.min.f = hrig->state.level_gran[9].min.f;
      pi_range.max.f = hrig->state.level_gran[9].max.f;
      pi_range.step.f = hrig->state.level_gran[9].step.f;
    }
    if (pi_range.step.f == 0.0)
      pi_range.step.f = (pi_range.max.f - pi_range.min.f)/100;
    set_scale_range(app1,"PIScale",pi_range,TRUE);
  }
}


void
set_pi                                 (GtkAdjustment*	adj,
                                        gpointer	gptr)
{
  value_t	val;

  if (has_pi) {
    val.f = -get_scale_value(app1,"PIScale");
    if (val.f != pival) {
      rig_set_level(hrig, RIG_VFO_CURR, RIG_LEVEL_PBT_IN, val);
      get_pi();
    }
  }
}


void
get_pi()
{
  gint		retcode;
  value_t	val;

  if (ispower && has_pi) {
    retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_PBT_IN, &val);
    if (retcode != RIG_OK) {
      pival = 0.0;
      g_warning("Hamlib error: Could not get pbt in level.");
    } else
      pival = val.f;
  } else
    pival = 0.0;
  set_pi_display();
}


void
set_pi_display()
{
  if (has_pi) {
    gchar* pistr = g_strdup_printf("%+05d",(int)pival);
    set_text(app1,"PIAmt",pistr);
    set_scale_value(app1,"PIScale",-pival);
    g_free(pistr);
  }
}


/** PBT OUT level control interface **/
void
check_po()
{
  po_range.min.f = 0.0;
  po_range.max.f = 1.0;
  po_range.step.f = 0.01;
  has_po = (rig_has_get_level(hrig,RIG_LEVEL_PBT_OUT) != 0);
  set_visible(app1,"POBox",has_po);
  if (has_po) {
    if ((hrig->state.level_gran[10].min.f != 0.0) &&
        (hrig->state.level_gran[10].max.f != 0.0)) {
      po_range.min.f = hrig->state.level_gran[10].min.f;
      po_range.max.f = hrig->state.level_gran[10].max.f;
      po_range.step.f = hrig->state.level_gran[10].step.f;
    }
    if (po_range.step.f == 0.0)
      po_range.step.f = (po_range.max.f - po_range.min.f)/100;
    set_scale_range(app1,"POScale",po_range,TRUE);
  }
}


void
set_po                                 (GtkAdjustment*	adj,
                                        gpointer	gptr)
{
  value_t	val;

  if (has_po) {
    val.f = -get_scale_value(app1,"POScale");
    if (val.f != poval) {
      rig_set_level(hrig, RIG_VFO_CURR, RIG_LEVEL_PBT_OUT, val);
      get_po();
    }
  }
}


void
get_po()
{
  gint		retcode;
  value_t	val;

  if (ispower && has_po) {
    retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_PBT_OUT, &val);
    if (retcode != RIG_OK) {
      poval = 0.0;
      g_warning("Hamlib error: Could not get pbt out level.");
    } else
      poval = val.f;
  } else
    poval = 0.0;
  set_po_display();
}


void
set_po_display()
{
  if (has_po) {
    gchar* postr = g_strdup_printf("%+05d",(int)poval);
    set_text(app1,"POAmt",postr);
    set_scale_value(app1,"POScale",-poval);
    g_free(postr);
  }
}


/** CW PITCH level control interface **/
void
check_cp()
{
  cp_range.min.i = -5000;
  cp_range.max.i = +5000;
  cp_range.step.i = 50;
  has_cp = (rig_has_get_level(hrig,RIG_LEVEL_CWPITCH) != 0);
  set_visible(app1,"CPBox",has_cp);
  if (has_cp) {
    if ((hrig->state.level_gran[11].min.i != 0) &&
        (hrig->state.level_gran[11].max.i != 0)) {
      cp_range.min.i = hrig->state.level_gran[11].min.i;
      cp_range.max.i = hrig->state.level_gran[11].max.i;
      cp_range.step.i = hrig->state.level_gran[11].step.i;
    }
    if (cp_range.step.i == 0.0)
      cp_range.step.i = (cp_range.max.i - cp_range.min.i)/100;
    set_scale_range(app1,"CPScale",cp_range,FALSE);
  }
}


void
set_cp                                 (GtkAdjustment*	adj,
                                        gpointer	gptr)
{
  value_t	val;

  if (has_cp) {
    val.i = -get_scale_value(app1,"CPScale");
    if (val.i != cpval) {
      rig_set_level(hrig, RIG_VFO_CURR, RIG_LEVEL_CWPITCH, val);
      get_cp();
    }
  }
}


void
get_cp()
{
  gint		retcode;
  value_t	val;

  if (ispower && has_cp) {
    retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_CWPITCH, &val);
    if (retcode != RIG_OK) {
      cpval = 0.0;
      g_warning("Hamlib error: Could not get cw pitch level.");
    } else {
      cpval = val.i;
    }
  } else
    cpval = 0.0;
  set_cp_display();
}


void
set_cp_display()
{
  if (has_cp) {
    gchar* cpstr = g_strdup_printf("%+05d",(int)cpval);
    set_text(app1,"CPAmt",cpstr);
    set_scale_value(app1,"CPScale",-cpval);
    g_free(cpstr);
  }
}


/** SQUELCH level control interface **/
void
check_sq()
{
  sq_range.min.f = 0.0;
  sq_range.max.f = 1.0;
  sq_range.step.f = 0.01;
  has_sq = (rig_has_get_level(hrig,RIG_LEVEL_SQL) != 0);
  set_visible(app1,"SQBox",has_sq);
  if (has_sq) {
    if ((hrig->state.level_gran[5].min.f != 0.0) &&
        (hrig->state.level_gran[5].max.f != 0.0)) {
      sq_range.min.f = hrig->state.level_gran[5].min.f;
      sq_range.max.f = hrig->state.level_gran[5].max.f;
      sq_range.step.f = hrig->state.level_gran[5].step.f;
    }
    if (sq_range.step.f == 0.0)
      sq_range.step.f = (sq_range.max.f - sq_range.min.f)/100;
    set_scale_range(app1,"SQScale",sq_range,TRUE);
  }
}


void
set_sq                                 (GtkAdjustment*	adj,
                                        gpointer	gptr)
{
  value_t	val;

  if (has_sq) {
    val.f = -get_scale_value(app1,"SQScale");
    if (val.f != sqval) {
      rig_set_level(hrig, RIG_VFO_CURR, RIG_LEVEL_SQL, val);
      get_sq();
    }
  }
}


void
get_sq()
{
  gint		retcode;
  value_t	val;

  if (ispower && has_sq) {
    retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_SQL, &val);
    if (retcode != RIG_OK) {
      sqval = 0.0;
      g_warning("Hamlib error: Could not get squelch level.");
    } else
      sqval = val.f;
  } else
    sqval = 0.0;
  set_sq_display();
}


void
set_sq_display()
{
  if (has_sq) {
    gchar* sqstr = g_strdup_printf("%03d",(int)(sqval*100.0));
    set_text(app1,"SQAmt",sqstr);
    set_scale_value(app1,"SQScale",-sqval);
    g_free(sqstr);
  }
}


/** NOTCH level control interface **/
void
check_no()
{
  no_range.min.i = 0;
  no_range.max.i = +5000;
  no_range.step.i = 10;
  has_no = (rig_has_get_level(hrig,RIG_LEVEL_NOTCHF) != 0);
  set_visible(app1,"NOBox",has_no);
  if (has_no) {
    if ((hrig->state.level_gran[15].min.i != 0) &&
        (hrig->state.level_gran[15].max.i != 0)) {
      no_range.min.i = hrig->state.level_gran[15].min.i;
      no_range.max.i = hrig->state.level_gran[15].max.i;
      no_range.step.i = hrig->state.level_gran[15].step.i;
    }
    if (no_range.step.i == 0.0)
      no_range.step.i = (no_range.max.i - no_range.min.i)/100;
    set_scale_range(app1,"NOScale",no_range,FALSE);
  }
}


void
set_no                                 (GtkAdjustment*	adj,
                                        gpointer	gptr)
{
  value_t	val;

  if (has_no) {
    val.i = -get_scale_value(app1,"NOScale");
    if (val.i != noval) {
      rig_set_level(hrig, RIG_VFO_CURR, RIG_LEVEL_NOTCHF, val);
      get_no();
    }
  }
}


void
get_no()
{
  gint		retcode;
  value_t	val;

  if (ispower && has_no) {
    retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_NOTCHF, &val);
    if (retcode != RIG_OK) {
      noval = 0.0;
      g_warning("Hamlib error: Could not get notch level.");
    } else
      noval = val.i;
  } else
    noval = 0.0;
  set_no_display();
}


void
set_no_display()
{
  if (has_no) {
    gchar* nostr = g_strdup_printf("%04d",(int)noval);
    set_text(app1,"NOAmt",nostr);
    set_scale_value(app1,"NOScale",-noval);
    g_free(nostr);
  }
}


/** NOISE REDUCTION level control interface **/
void
check_nr()
{
  nr_range.min.f = 0.0;
  nr_range.max.f = 1.0;
  nr_range.step.f = 0.1;
  has_nr = (rig_has_get_level(hrig,RIG_LEVEL_NR) != 0);
  set_visible(app1,"NRBox",has_nr);
  if (has_nr) {
    if ((hrig->state.level_gran[8].min.f != 0.0) &&
        (hrig->state.level_gran[8].max.f != 0.0)) {
      nr_range.min.f = hrig->state.level_gran[8].min.f;
      nr_range.max.f = hrig->state.level_gran[8].max.f;
      nr_range.step.f = hrig->state.level_gran[8].step.f;
    }
    if (nr_range.step.f == 0.0)
      nr_range.step.f = (nr_range.max.f - nr_range.min.f)/100;
    set_scale_range(app1,"NRScale",nr_range,TRUE);
  }
}


void
set_nr                                 (GtkAdjustment*	adj,
                                        gpointer	gptr)
{
  value_t	val;

  if (has_nr) {
    val.f = -get_scale_value(app1,"NRScale");
    if (val.f != nrval) {
      rig_set_level(hrig, RIG_VFO_CURR, RIG_LEVEL_NR, val);
      get_nr();
    }
  }
}


void
get_nr()
{
  gint		retcode;
  value_t	val;

  if (ispower && has_nr) {
    retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_NR, &val);
    if (retcode != RIG_OK) {
      nrval = 0.0;
      g_warning("Hamlib error: Could not get noise reduction level.");
    } else
      nrval = val.f;
  } else
    nrval = 0.0;
  set_nr_display();
}


void
set_nr_display()
{
  if (has_nr) {
    gchar* nrstr = g_strdup_printf("%03d",(int)(nrval*100.0));
    set_text(app1,"NRAmt",nrstr);
    set_scale_value(app1,"NRScale",-nrval);
    g_free(nrstr);
  }
}


/** AUTO PASSBAND FILTER level control interface **/
void
check_ap()
{
  ap_range.min.f = 0.0;
  ap_range.max.f = 1.0;
  ap_range.step.f = 0.1;
  has_ap = (rig_has_get_level(hrig,RIG_LEVEL_APF) != 0);
  set_visible(app1,"APBox",has_ap);
  if (has_ap) {
    if ((hrig->state.level_gran[7].min.f != 0.0) &&
        (hrig->state.level_gran[7].max.f != 0.0)) {
      ap_range.min.f = hrig->state.level_gran[7].min.f;
      ap_range.max.f = hrig->state.level_gran[7].max.f;
      ap_range.step.f = hrig->state.level_gran[7].step.f;
    }
    if (ap_range.step.f == 0.0)
      ap_range.step.f = (ap_range.max.f - ap_range.min.f)/100;
    set_scale_range(app1,"APScale",ap_range,TRUE);
  }
}


void
set_ap                                 (GtkAdjustment*	adj,
                                        gpointer	gptr)
{
  value_t	val;

  if (has_ap) {
    val.f = -get_scale_value(app1,"APScale");
    if (val.f != apval) {
      rig_set_level(hrig, RIG_VFO_CURR, RIG_LEVEL_APF, val);
      get_ap();
    }
  }
}


void
get_ap()
{
  gint		retcode;
  value_t	val;

  if (ispower && has_ap) {
    retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_APF, &val);
    if (retcode != RIG_OK) {
      apval = 0.0;
      g_warning("Hamlib error: Could not get apf level.");
    } else
      apval = val.f;
  } else
    apval = 0.0;
  set_ap_display();
}


void
set_ap_display()
{
  if (has_ap) {
    gchar* apstr = g_strdup_printf("%03d",(int)(apval*100.0));
    set_text(app1,"APAmt",apstr);
    set_scale_value(app1,"APScale",-apval);
    g_free(apstr);
  }
}

//LATER for when a tone control becomes available in hamlib
/** Tone level control interface **/
void
check_to()
{
  to_range.min.f = 0.0;
  to_range.max.f = 1.0;
  to_range.step.f = 0.1;
  //has_to = (rig_has_get_level(hrig,RIG_LEVEL_TONE));
  has_to = false;
  set_visible(app1,"TOBox",has_to);
  /*if (has_to) {
    if ((hrig->state.level_gran[32].min.f != 0.0) &&
        (hrig->state.level_gran[32].max.f != 0.0)) {
      to_range.min.f = hrig->state.level_gran[32].min.f;
      to_range.max.f = hrig->state.level_gran[32].max.f;
      to_range.step.f = hrig->state.level_gran[32].step.f;
    }
    if (to_range.step.f == 0.0)
      to_range.step.f = (to_range.max.f - to_range.min.f)/100;
    set_scale_range(app1,"TOScale",to_range,TRUE);
  }*/
}

/*
void
set_to                                 (GtkAdjustment*	adj,
                                        gpointer	gptr)
{
  value_t	val;

  val.f = -get_scale_value(app1,"TOScale");
  if (val.f != toval) {
    rig_set_level(hrig, RIG_VFO_CURR, RIG_LEVEL_TONE, val);
    get_to();
  }
}


void
get_to()
{
  gint		retcode;
  value_t	val;

  if (ispower) {
    if (has_to) {
      retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_TONE, &val);
      if (retcode != RIG_OK) {
        toval = 0.0;
        g_warning("Hamlib error: Could not get tone level.");
      } else
        toval = val.f;
    } else
      toval = 0.0;
  } else
    toval = 0.0;
  set_to_display();
}


void
set_to_display()
{
  if (has_to) {
    gchar* tostr = g_strdup_printf("%03d",(int)(toval*100.0));
    set_text(app1,"TOAmt",tostr);
    set_scale_value(app1,"TOScale",-toval);
    g_free(tostr);
  }
}*/

//LATER for when BWC becomes available in hamlib
/** BWC level control interface **/
void
check_bwc()
{
  bwc_range.min.i = 10;
  bwc_range.max.i = 9990;
  bwc_range.step.i = 10;
  //has_bwc = (rig_has_get_level(hrig,RIG_LEVEL_BWC));
  has_bwc = FALSE;
  set_visible(app1,"BWCBox",has_bwc);
  /*if (has_bwc) {
    if ((hrig->state.level_gran[31].min.i != 0) &&
        (hrig->state.level_gran[31].max.i != 0)) {
      bwc_range.min.i = hrig->state.level_gran[31].min.i;
      bwc_range.max.i = hrig->state.level_gran[31].max.i;
      bwc_range.step.i = hrig->state.level_gran[31].step.i;
    }
    if (bwc_range.step.i == 0.0)
      bwc_range.step.i = (bwc_range.max.i - bwc_range.min.i)/100;
    set_scale_range(app1,"BWCScale",bwc_range,FALSE);
  }*/
}

/*
void
set_bwc                                (GtkAdjustment*	adj,
                                        gpointer	gptr)
{
  value_t	val;

  if (has_bwc) {
    val.i = -get_scale_value(app1,"BWCScale");
    if (val.i != bwcval) {
      rig_set_level(hrig, RIG_VFO_CURR, RIG_LEVEL_BWC, val);
      get_bwc();
    }
  }
}


void
get_bwc()
{
  gint		retcode;
  value_t	val;

  if (ispower && has_bwc) {
    retcode = rig_get_level(hrig, RIG_VFO_CURR, RIG_LEVEL_BWC, &val);
    if (retcode != RIG_OK) {
      bwcval = 0.0;
      g_warning("Hamlib error: Could not get bwc level.");
    } else
      bwcval = val.i;
  } else
    bwcval = 0.0;
  set_bwc_display();
}


void
set_bwc_display()
{
  if (has_bwc) {
    gchar* bwcstr = g_strdup_printf("%04d",(int)bwcval);
    set_text(app1,"BWCAmt",bwcstr);
    set_scale_value(app1,"BWCScale",-bwcval);
    g_free(bwcstr);
  }
}*/


/** external tape control interface **/
void
set_tape()
{
  istape = !istape;
  set_tape_display();
}


void
set_tape_display()
{
  set_visible(app1,"TapeLbl",ispower&&istape);
  set_enabled(app1,"TapeBtn",ispower);
  set_down(app1,"TapeBtn",ispower&&istape);
  set_enabled(app1,"tape_vox1",ispower);
}


/** memory mode control interface **/
void
check_mem()
{
  GtkWidget*	mem_list;
  GtkTreeModel*	model;
  GtkTreeIter	iter;
  int		i;
  gchar*	buf;
  gchar**	text;

  enum {
  CHAN = 0,
  FREQ,
  VFO,
  ANT,
  MODE,
  WIDTH,
  NAME,
  NUM_COLS};

  has_get_mem = (hrig->caps->get_mem != NULL);
  has_set_mem = (hrig->caps->set_mem != NULL);
  has_get_channel = (hrig->caps->get_channel != NULL);
  has_set_channel = (hrig->caps->set_channel != NULL);
  i = 0;
  min_mem_ch = -1;
  max_mem_ch = -1;
  min_mem_edge = -1;
  max_mem_edge = -1;
  while (hrig->state.chan_list[i].type != 0) {
    switch (hrig->state.chan_list[i].type) {
      case RIG_MTYPE_MEM :
        min_mem_ch = hrig->state.chan_list[i].startc;
        max_mem_ch = hrig->state.chan_list[i].endc;
        break;
      case RIG_MTYPE_EDGE :
        min_mem_edge = hrig->state.chan_list[i].startc;
        max_mem_edge = hrig->state.chan_list[i].endc;
        break;
      default :
        break;
    }
    i++;
  }
  if (!has_get_mem)
    memch = min_mem_ch;
  ismem = FALSE;
  
  mem_list = lookup_widget(mem1, "MemList");
  if (mem_list) {
    model = (GtkTreeModel*)gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                                              G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, -1);           
    for (i = min_mem_ch; i < (max_mem_ch+1); i++) {
      buf = g_strdup_printf("%03d;-;-;-;-;-; ", i);
      if (buf) {
        text = g_strsplit(buf, ";", 7);
        gtk_list_store_append((GtkListStore*)model, &iter);
        gtk_list_store_set((GtkListStore*)model, &iter, CHAN, text[0], FREQ, text[1], VFO, text[2],
                                       ANT, text[3], MODE, text[4], WIDTH, text[5], NAME, text[6], -1);
        g_free(buf);
        g_strfreev(text);
      }
    }
    if ((min_mem_edge != -1) && (max_mem_edge != -1)) {
      for (i = min_mem_edge; i < (max_mem_edge+1); i++) {
        buf = g_strdup_printf("%03d;-;-;-;-;-; ", i);
        if (buf) {
          text = g_strsplit(buf, ";", 7);
          gtk_list_store_append((GtkListStore*)model, &iter);
          gtk_list_store_set((GtkListStore*)model, &iter, CHAN, text[0], FREQ, text[1], VFO, text[2],
                                         ANT, text[3], MODE, text[4], WIDTH, text[5], NAME, text[6], -1);
          g_free(buf);
          g_strfreev(text);
        }
      }
    }
    gtk_tree_view_set_model((GtkTreeView*)mem_list, model);
    g_object_unref(model);
    set_list_selected_row (mem1,"MemList",0);
  }
}


void
set_mem()
{
  gint	retcode;
  vfo_t	new_vfo;

  if (has_set_mem) {
    ismem = !ismem;
    if (ispower) {
      if (ismem)
        new_vfo = RIG_VFO_MEM;
      else
        new_vfo = RIG_VFO_VFO;

      retcode = rig_set_vfo(hrig, new_vfo);
      if (retcode != RIG_OK)
        g_warning("Hamlib error: Could not set vfo/memory.");

      if (ismem) {
        retcode = rig_set_mem(hrig, RIG_VFO_CURR, memch);
        if (retcode != RIG_OK)
          g_warning("Hamlib error: Could not set memory channel.");
      }
    }
    get_all(FALSE);
  }
}


void
get_mem()
{
  gint		retcode;
  int		ch1;
  channel_t	chan_data;

  if (ispower && has_get_mem) {
    retcode = rig_get_mem(hrig, RIG_VFO_CURR, &ch1);
    if (retcode != RIG_OK) {
      memch = min_mem_ch;
      g_warning("Hamlib error: Could not get memory channel.");
    } else {
      memch = ch1;
      if (ismem) {
        chan_data.channel_num = memch;
        retcode = rig_get_channel(hrig, RIG_VFO_CURR, &chan_data, true);
        if (retcode == RIG_OK) {
          frequency = chan_data.freq/1000.0;
          set_freq_display();
        }
      }
    }
  }
  set_mem_display();
}


void
set_mem_display()
{
  set_enabled(app1,"MemBtn",ispower);
  set_down(app1,"MemBtn",ispower && ismem);
  set_visible(app1,"MemLbl",ispower && ismem);
  set_visible(app1,"Mem2Lbl",ispower);
  gchar* chstr = g_strdup_printf("%03d",memch);
  set_text(app1,"Mem2Lbl",chstr);
  g_free(chstr);
  set_list_selected_row (mem1,"MemList",memch-min_mem_ch);
  set_enabled(mem1,"button23",(ispower && ismem));
  set_enabled(mem1,"button24",(ispower && ismem));
}


void
get_memory_list()
{
  GtkWidget*	mem_list;
  GtkWidget*	app_bar;
  GtkWidget*	prog_bar;
  GtkTreeModel*	model;
  GtkTreeIter	iter;
  guint 	i;
  guint		delta;
  guint		resval;
  gchar* 	buf;
  gchar*	status;
  gchar**	text;
  channel_t	chan;
  gchar*	vfo_str;
  gchar*	ant_str;
  gchar*	mode_str;
  gfloat	pcent;

  enum {
  CHAN = 0,
  FREQ,
  VFO,
  ANT,
  MODE,
  WIDTH,
  NAME,
  NUM_COLS};
  
  if (!ispower)
    show_message("Memory Capturre", GTK_MESSAGE_WARNING, "Radio must be powered to retrieve memories.");
  else {
    if (!has_get_channel)
      show_message("Memory Capturre", GTK_MESSAGE_WARNING, "Radio must be able to provide memories.");
    else {  
      app_bar = lookup_widget(app1, "appbar1");
      prog_bar = lookup_widget(app1, "progbar1");
      mem_list = lookup_widget(mem1, "MemList");
      if (mem_list) {
        model = (GtkTreeModel*)gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                                                  G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, -1);           
        delta = max_mem_ch - min_mem_ch + 1;
        for (i = min_mem_ch; i < (max_mem_ch+1); i++) {
          chan.channel_num = i;
          resval = rig_get_channel(hrig, RIG_VFO_CURR, &chan, true);
          if ((resval == RIG_OK) && (chan.freq != 0)) {
      
            if ((chan.vfo & 0x03) > 0)
              vfo_str = g_strdup_printf("%c",(0x40+chan.vfo));
            else
              vfo_str = g_strdup_printf("-");
          
            if (chan.ant > 0)
              ant_str = g_strdup_printf("%c",(0x30+chan.ant));
            else
              ant_str = g_strdup_printf("-");
        
            switch(chan.mode) {
              case RIG_MODE_AM :
                mode_str = strdup("AM");
                break;
              case RIG_MODE_CW :
                mode_str = strdup("CW");
                break;
              case RIG_MODE_USB :
                mode_str = strdup("USB");
                break;
              case RIG_MODE_LSB :
                mode_str = strdup("LSB");
                break;
              case RIG_MODE_RTTY :
                mode_str = strdup("RTTY");
                break;
              case RIG_MODE_FM :
                mode_str = strdup("FM");
                break;
              case RIG_MODE_WFM :
                mode_str = strdup("FM-Wide");
                break;
              case RIG_MODE_CWR :
                mode_str = strdup("CW-R");
                break;
              case RIG_MODE_RTTYR :
                mode_str = strdup("RTTY-R");
                break;
              case RIG_MODE_AMS :
                mode_str = strdup("AMS");
                break;
              case RIG_MODE_PKTLSB :
                mode_str = strdup("Packet-LSB");
                break;
              case RIG_MODE_PKTUSB :
                mode_str = strdup("Packet-USB");
                break;
              case RIG_MODE_PKTFM :
                mode_str = strdup("PKT-FM");
                break;
              case RIG_MODE_FAX :
                mode_str = strdup("FAX");
                break;
              case RIG_MODE_ECSSLSB :
                mode_str = strdup("ECSS-LSB");
                break;
              case RIG_MODE_ECSSUSB :
                mode_str = strdup("ECSS-USB");
                break;
              default :
                mode_str = strdup("-");
                break;
            }
              
            buf = g_strdup_printf("%03d;%.3f;%s;%s;%s;%.1f;%s", chan.channel_num,
                                                                 chan.freq/1000.0,
                                                                 vfo_str,
                                                                 ant_str,
                                                                 mode_str,
                                                                 chan.width/1000.0,
                                                                 chan.channel_desc);
            g_free(vfo_str);
            g_free(ant_str);
            g_free(mode_str);
          } else
            buf = g_strdup_printf("%03d;%s;%s;%s;%s;%s;%s", chan.channel_num,
                                                           "-","-","-","-","-"," ");

          text = g_strsplit(buf, ";", 7);
          gtk_list_store_append((GtkListStore*)model, &iter);
          gtk_list_store_set((GtkListStore*)model, &iter, CHAN, text[0], FREQ, text[1], VFO, text[2],
                                           ANT, text[3], MODE, text[4], WIDTH, text[5], NAME, text[6], -1);
          g_free(buf);
          g_strfreev(text);
          if (app_bar && prog_bar) {
            pcent = (gfloat)(i - min_mem_ch + 1) / (gfloat)delta;
	    status = g_strdup_printf("Collecting memory #%d",i);
    	    gtk_label_set_text(GTK_LABEL(app_bar), status);
	    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(prog_bar), pcent);
	    update_immediate();
	    g_free(status);
          }
        }
        if ((min_mem_edge != -1) && (max_mem_edge != -1)) {
          delta = max_mem_edge - min_mem_edge + 1;
          for (i = min_mem_edge; i < (max_mem_edge+1); i++) {
            chan.channel_num = i;
            resval = rig_get_channel(hrig, RIG_VFO_CURR, &chan, true);
            if ((resval == RIG_OK) && (chan.freq != 0)) {

              if ((chan.vfo & 0x03) > 0)
                vfo_str = g_strdup_printf("%c",(0x40+chan.vfo));
              else
                vfo_str = g_strdup_printf("-");
  
              if (chan.ant > 0)
                ant_str = g_strdup_printf("%c",(0x30+chan.vfo));
              else
                ant_str = g_strdup_printf("-");
        
              switch(chan.mode) {
                case RIG_MODE_AM :
                  mode_str = strdup("AM");
                  break;
                case RIG_MODE_CW :
                  mode_str = strdup("CW");
                  break;
                case RIG_MODE_USB :
                  mode_str = strdup("USB");
                  break;
                case RIG_MODE_LSB :
                  mode_str = strdup("LSB");
                  break;
                case RIG_MODE_RTTY :
                  mode_str = strdup("RTTY");
                  break;
                case RIG_MODE_FM :
                  mode_str = strdup("FM");
                  break;
                case RIG_MODE_WFM :
                  mode_str = strdup("FM-Wide");
                  break;
                case RIG_MODE_CWR :
                  mode_str = strdup("CW-R");
                  break;
                case RIG_MODE_RTTYR :
                  mode_str = strdup("RTTY-R");
                  break;
                case RIG_MODE_AMS :
                  mode_str = strdup("AMS");
                  break;
                case RIG_MODE_PKTLSB :
                  mode_str = strdup("Packet-LSB");
                  break;
                case RIG_MODE_PKTUSB :
                  mode_str = strdup("Packet-USB");
                  break;
                case RIG_MODE_PKTFM :
                  mode_str = strdup("PKT-FM");
                  break;
                case RIG_MODE_FAX :
                  mode_str = strdup("FAX");
                  break;
                case RIG_MODE_ECSSLSB :
                  mode_str = strdup("ECSS-LSB");
                  break;
                case RIG_MODE_ECSSUSB :
                  mode_str = strdup("ECSS-USB");
                  break;
                default :
                  mode_str = strdup("-");
                  break;
              }
              
              buf = g_strdup_printf("%03d;%.3f;%s;%s;%s;%.1f;%s", chan.channel_num,
                                                                   chan.freq/1000.0,
                                                                   vfo_str,
                                                                   ant_str,
                                                                   mode_str,
                                                                   chan.width/1000.0,
                                                                   chan.channel_desc);
              g_free(vfo_str);
              g_free(ant_str);
              g_free(mode_str);
            } else
              buf = g_strdup_printf("%03d;%s;%s;%s;%s;%s;%s", chan.channel_num,
                                                           "-","-","-","-","-"," ");
            text = g_strsplit(buf, ";", 7);
            gtk_list_store_append((GtkListStore*)model, &iter);
            gtk_list_store_set((GtkListStore*)model, &iter, CHAN, text[0], FREQ, text[1], VFO, text[2],
                                             ANT, text[3], MODE, text[4], WIDTH, text[5], NAME, text[6], -1);
            g_free(buf);
            g_strfreev(text);
            if (app_bar && prog_bar) {
              pcent = (i - min_mem_edge + 1) / delta;
              status = g_strdup_printf("Collecting memory edge #%d",i);
              gtk_label_set_text(GTK_LABEL(app_bar), status);
              gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(prog_bar), pcent);
              g_free(status);
            }
          }
        }
        if (app_bar && prog_bar) {
          status = g_strdup_printf(" ");
          gtk_label_set_text(GTK_LABEL(app_bar), status);
          gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(prog_bar), 0);
          g_free(status);
        }
        gtk_tree_view_set_model((GtkTreeView*)mem_list, model);
        g_object_unref(model);
        set_list_selected_row (mem1,"MemList",memch-min_mem_ch);
        
        //now save as default memory file
        save_memories_to_file(NULL);
        //ensure mem window matches radio with default info
        if (!cur_mem_default)
          load_memories_from_file(NULL);
      }
    }
  }
}


void
fill_memory_channel_from_list_item	(gint	in_row)
{
  GtkTreeModel* store;
  GtkTreeIter	iter;
  GValue	value = G_VALUE_INIT;
  const gchar*	valStr;
  int		chNum;
  vfo_t		nVFO;
  ant_t		nAnt;
  freq_t	nFreq;
  rmode_t	nMode;
  pbwidth_t	nWidth;
  channel_t	chan_data;
  
  //this is only called if editing a default file or if filling the entire set of memeories.
  
  if (!ispower)
    show_message("Memory Fill", GTK_MESSAGE_WARNING, "Radio must be powered to fill memories.");
  else {
    if (!has_set_channel)
      show_message("Memory Fill", GTK_MESSAGE_WARNING, "Radio must be able to set memories.");
    else {
      store = gtk_tree_view_get_model((GtkTreeView*)lookup_widget(mem1,"MemList")); 
      gtk_tree_model_iter_nth_child(store, &iter, NULL, in_row);
  
      //get channel number to fill and other data
      gtk_tree_model_get_value(store, &iter, 0, &value);
      valStr = g_value_get_string(&value);
      chNum = atoi(valStr);
      g_value_unset(&value);
  
      chan_data.channel_num = chNum;
      rig_get_channel(hrig, RIG_VFO_CURR, &chan_data, true);  

      gtk_tree_model_get_value(store, &iter, 1, &value);
      valStr = g_value_get_string(&value);
      if (strcmp(valStr, "-") != 0) {
        nFreq = (freq_t)atof(valStr) * 1000.0;
        g_value_unset(&value);
        chan_data.freq = nFreq;

        if ((has_vfo) && (has_get_vfo) && (has_set_vfo)) {
          gtk_tree_model_get_value(store, &iter, 2, &value);
          valStr = g_value_get_string(&value);
          switch (valStr[0]) {
            case 'A' :
              nVFO = RIG_VFO_A;
              break;
            case 'B' :
              nVFO = RIG_VFO_B;
              break;
            case 'C' :
              nVFO = RIG_VFO_C;
              break;
            default :
              nVFO = RIG_VFO_MEM;
              break;
          }
          g_value_unset(&value);
          chan_data.vfo = nVFO;
        }

        if ((has_get_ant) && (has_set_ant)) {
          gtk_tree_model_get_value(store, &iter, 3, &value);
          valStr = g_value_get_string(&value);
          switch (valStr[0]) {
        case '1' :
          nAnt = RIG_ANT_1;
          break;
        case '2' :
          nAnt = RIG_ANT_2;
          break;
        case '3' :
          nAnt = RIG_ANT_3;
          break;
        default :
          nAnt = RIG_ANT_1;
          break;
          }
          g_value_unset(&value);
          chan_data.ant = nAnt;
        }

        if ((has_get_mode) && (has_set_mode)) {
          gtk_tree_model_get_value(store, &iter, 4, &value);
          valStr = g_value_get_string(&value);
          if (strcmp(valStr,"AM") == 0)
            nMode = RIG_MODE_AM;
          else
            if (strcmp(valStr,"CW") == 0)
              nMode = RIG_MODE_CW;
            else
              if (strcmp(valStr,"USB") == 0)
                nMode = RIG_MODE_USB;
              else
                if (strcmp(valStr,"LSB") == 0)
                  nMode = RIG_MODE_LSB;
                else
                  if (strcmp(valStr,"RTTY") == 0)
                    nMode = RIG_MODE_RTTY;
                  else
                    if (strcmp(valStr,"FM") == 0)
                      nMode = RIG_MODE_FM;
                    else
                      if (strcmp(valStr,"FM-Wide") == 0)
                        nMode = RIG_MODE_WFM;
                      else
                        if (strcmp(valStr,"CW-R") == 0)
                          nMode = RIG_MODE_CWR;
                        else
                          if (strcmp(valStr,"RTTY-R") == 0)
                            nMode = RIG_MODE_RTTYR;
                          else
                            if (strcmp(valStr,"AMS") == 0)
                              nMode = RIG_MODE_AMS;
                            else
                              if (strcmp(valStr,"Packet-LSB") == 0)
                                nMode = RIG_MODE_PKTLSB;
                              else
                                if (strcmp(valStr,"Packet-USB") == 0)
                                  nMode = RIG_MODE_PKTUSB;
                                else
                                  if (strcmp(valStr,"PKT-FM") == 0)
                                    nMode = RIG_MODE_PKTFM;
                                  else
                                    if (strcmp(valStr,"FAX") == 0)
                                      nMode = RIG_MODE_FAX;
                                    else
                                      if (strcmp(valStr,"ECSS-LSB") == 0)
                                        nMode = RIG_MODE_ECSSLSB;
                                      else
                                        if (strcmp(valStr,"ECSS-USB") == 0)
                                          nMode = RIG_MODE_ECSSUSB;
                                        else
                                          nMode = RIG_MODE_AM;
          g_value_unset(&value);
          chan_data.mode = nMode;

          gtk_tree_model_get_value(store, &iter, 5, &value);
          valStr = g_value_get_string(&value);
          nWidth = (int)(atof(valStr) * 1000.0);
          g_value_unset(&value);
          chan_data.width = nWidth;
  
          //now fill the radio with this info    
          if (rig_set_channel(hrig, RIG_VFO_CURR, &chan_data) != RIG_OK)
            show_message("Fill Memory Channel", GTK_MESSAGE_ERROR, "Failed to update the radio with the updated channel.");
        }
      }
      g_value_unset(&value);
    }
  }
}


void
fill_all_memory_channels_from_list()
{
  GtkWidget*	app_bar;
  GtkWidget*	prog_bar;
  gchar*	status;
  gfloat	pcent;
  gint		num_mems;
  gint		result;
  gint		i;
  
  if (!ispower)
    show_message("Memory Fill", GTK_MESSAGE_WARNING, "Radio must be powered to fill memories.");
  else {
    if (!has_set_mem)
      show_message("Memory Fill", GTK_MESSAGE_WARNING, "Radio must be able to set memories.");
    else {
      result = true;
      if (cur_mem_default) {
        result = (show_yes_no_message("Memory Fill", GTK_MESSAGE_WARNING,
            "You are about to fill the radio with the default memory file.\nDid you really want to re-synch the radio to it?") == GTK_RESPONSE_YES);
      }
      if (result) {
        app_bar = lookup_widget(app1, "appbar1");
        prog_bar = lookup_widget(app1, "progbar1");
    
        // loop thorough each row and set the memories
        num_mems = max_mem_ch - min_mem_ch + 1;
        for (i = 0; i < num_mems; i++) {
          fill_memory_channel_from_list_item(i);
          if (app_bar && prog_bar) {
            pcent = (gfloat)(i - min_mem_ch + 1) / (gfloat)num_mems;
            status = g_strdup_printf("Filling memory #%d",i);
            gtk_label_set_text(GTK_LABEL(app_bar), status);
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(prog_bar), pcent);
            update_immediate();
            g_free(status);
          }
        }
        if (app_bar && prog_bar) {
          status = g_strdup_printf(" ");
          gtk_label_set_text(GTK_LABEL(app_bar), status);
          gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(prog_bar), 0);
          g_free(status);
        }
        //whatever this was, it now should be in the default file
        save_memories_to_file(NULL);
        //ensure mem window matches radio with default info
        if (!cur_mem_default)
          load_memories_from_file(NULL);
      }
    }
  }    
}


void
set_mem_window_title()
{
  gchar*	title_str;
  
  title_str = g_strdup_printf("Memories - [%s]", cur_mem_file);
  gtk_window_set_title(GTK_WINDOW(mem1),title_str);
  g_free(title_str);
}


void
mem_list_cell_edited			(GtkCellRendererText*	cell,
                                         char*			path_string,
                                         char*			new_text,
                                         gpointer		user_data)
{
  GtkTreeModel* store;
  GtkTreeIter	iter;
  GValue	value = G_VALUE_INIT;
  const gchar*	old_text;
  gchar*	temp;
  double	inFreq;
  gint		in_row = -1;
  gint		in_col = -1;

  in_row = atoi(path_string);  
  if (cell == (GtkCellRendererText*)lookup_widget(mem1,"memfreqRenderer"))
    in_col = 1;
  else if (cell == (GtkCellRendererText*)lookup_widget(mem1,"vfoRenderer"))
    in_col = 2;
  else if (cell == (GtkCellRendererText*)lookup_widget(mem1,"antRenderer"))
    in_col = 3;
  else if (cell == (GtkCellRendererText*)lookup_widget(mem1,"modeRenderer"))
    in_col = 4;
  else if (cell == (GtkCellRendererText*)lookup_widget(mem1,"bwRenderer"))
    in_col = 5;
  else if (cell == (GtkCellRendererText*)lookup_widget(mem1,"nameRenderer"))
    in_col = 6;
  
  store = gtk_tree_view_get_model((GtkTreeView*)lookup_widget(mem1,"MemList")); 
  gtk_tree_model_iter_nth_child(store, &iter, NULL, in_row);
  
  //get value it was changed from
  gtk_tree_model_get_value(store, &iter, in_col, &value);
  old_text = g_value_get_string(&value);
  
  if (in_col == 1) {
    inFreq = atof(new_text);
    temp = g_strdup_printf("%.3f",inFreq);
    gtk_list_store_set((GtkListStore*)store, &iter, in_col, temp, -1);
    g_free(temp);
  } else  
    gtk_list_store_set((GtkListStore*)store, &iter, in_col, new_text, -1);

  // only fill radio if default, if it wasn't just the name, and new_text is different than old_text:
  if ((cur_mem_default) && (in_col != 6) && (strcmp(new_text, old_text) != 0)) {
    if (ispower) {
      fill_memory_channel_from_list_item(in_row);
    } else {
      show_message("Fill Memory Channel", GTK_MESSAGE_WARNING, "Radio is not currently on and channel can't be updated.\nPlease remember to do a Memory Fill the next time the radio is on.");
    }
  }
  //update the appropriate memory file
  if (cur_mem_default)
    save_memories_to_file(NULL);
  else
    save_memories_to_file(cur_mem_file);
  g_value_unset(&value);
}


void
set_mem_list_editing()
{
  GValue	value = G_VALUE_INIT;
  
  g_value_init(&value, G_TYPE_BOOLEAN);

  g_value_set_boolean(&value, true);
  g_object_set_property((GObject*)lookup_widget(mem1, "memfreqRenderer"), "editable", &value);
  g_object_set_property((GObject*)lookup_widget(mem1, "nameRenderer"), "editable", &value);

  g_value_set_boolean(&value, ((has_vfo) && (has_get_vfo) && (has_set_vfo)));
  g_object_set_property((GObject*)lookup_widget(mem1, "vfoRenderer"), "editable", &value);

  g_value_set_boolean(&value, ((has_get_ant) && (has_set_ant)));
  g_object_set_property((GObject*)lookup_widget(mem1, "antRenderer"), "editable", &value);
  
  g_value_set_boolean(&value, ((has_get_mode) && (has_set_mode)));
  g_object_set_property((GObject*)lookup_widget(mem1, "modeRenderer"), "editable", &value);
  g_object_set_property((GObject*)lookup_widget(mem1, "bwRenderer"), "editable", &value);
  
  g_signal_connect(lookup_widget(mem1, "memfreqRenderer"), "edited",
                   G_CALLBACK(mem_list_cell_edited), NULL);
  g_signal_connect(lookup_widget(mem1, "vfoRenderer"), "edited",
                   G_CALLBACK(mem_list_cell_edited), NULL);
  g_signal_connect(lookup_widget(mem1, "antRenderer"), "edited",
                   G_CALLBACK(mem_list_cell_edited), NULL);
  g_signal_connect(lookup_widget(mem1, "modeRenderer"), "edited",
                   G_CALLBACK(mem_list_cell_edited), NULL);
  g_signal_connect(lookup_widget(mem1, "bwRenderer"), "edited",
                   G_CALLBACK(mem_list_cell_edited), NULL);
  g_signal_connect(lookup_widget(mem1, "nameRenderer"), "edited",
                   G_CALLBACK(mem_list_cell_edited), NULL);
  
  g_value_unset(&value);
}


void
load_memories_from_file                (gchar*	infilename)
{
  GtkWidget*		gen_item;
  GtkListStore*		store;
  GtkTreeIter		iter;
  GFile*		infile;
  GFileInputStream*	input;
  GDataInputStream*	input2;
  GError*		err = nullptr;
  gsize			insize;
  gchar*		default_file;
  gchar*		filename;
  gchar*		buf;
  gchar**		text;
  gboolean		result;

  enum {
  CHAN = 0,
  FREQ,
  VFO,
  ANT,
  MODE,
  WIDTH,
  NAME,
  NUM_COLS};

  default_file = g_strdup_printf("%s/%d-default.mem", basepath, cur_rig_model);
  if (infilename == NULL) {
    filename = strdup(default_file);
  } else {
    filename = g_strdup_printf("%s/%s",basepath, infilename);
    cur_mem_default = false;
  }
  cur_mem_default = (strcmp(filename, default_file) == 0);
  g_free(default_file);
  
  if (g_file_test(filename, G_FILE_TEST_EXISTS)) {
    gen_item = lookup_widget(mem1, "MemList");
    if (gen_item) {
      infile = g_file_new_for_path(filename);
      input = g_file_read(infile, NULL, &err);    
      if (!err) {
        store = gtk_list_store_new( NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, -1);
        if (store) {
          input2 = g_data_input_stream_new((GInputStream*)input);
          result = true;
          while (result) {
            buf = g_data_input_stream_read_line(input2, &insize, NULL, &err);
            result = (insize > 0);
            if (result) {
              if ((strlen(buf) > 0) && (!err)) {
                if (buf[0] == ';') {
                  if (mem_comment)
                    g_free(mem_comment);
                  mem_comment = g_strdup(buf);
                  memmove(mem_comment, mem_comment+1, strlen(mem_comment));
                } else {
                  gtk_list_store_append (store, &iter);
                  text = g_strsplit(buf, ";", 7);
                  gtk_list_store_set (store, &iter,
                                      CHAN, text[0],
                                      FREQ, text[1],
                                      VFO, text[2],
                                      ANT, text[3],
                                      MODE, text[4],
                                      WIDTH, text[5],
                                      NAME, text[6],
                                      -1);
                  g_strfreev(text);
                }
              } else {
                g_print("problem reading a line from memory file %s\n",filename);
                g_error_free(err);
              }
            }
            g_free(buf);
          }
          gtk_tree_view_set_model ((GtkTreeView*)gen_item, (GtkTreeModel*)store);
          set_list_selected_row (mem1,"MemList",memch-min_mem_ch);
          g_object_unref(store);
          
          cur_mem_file = get_filename_from_full_path(filename);
          set_mem_window_title();
        }  
      }
      g_object_unref(input);
      g_object_unref(infile);
    }
    g_free(filename);
  }
}


void
save_memories_to_file                  (gchar*	outfilename)
{
  GtkWidget*		gen_item;
  GFile*		outfile;
  GFileOutputStream*	output;
  GError*		err = nullptr;
  gsize			outsize;
  gchar*		filename;
  gchar*		chan_str;
  gchar*		freq_str;
  gchar*		vfo_str;
  gchar*		ant_str;
  gchar*		mode_str;
  gchar*		bw_str;
  gchar*		name_str;
  gint			num_rows;
  gint			i;
  gboolean		result;

  enum {
  CHAN = 0,
  FREQ,
  VFO,
  ANT,
  MODE,
  WIDTH,
  NAME,
  NUM_COLS};

  if (outfilename == NULL) {
    filename = g_strdup_printf("%s/%d-default.mem",basepath,cur_rig_model);
  } else {
    filename = g_strdup_printf("%s/%s.mem",basepath,outfilename);
  }

  gen_item = lookup_widget(mem1, "MemList");
  if (gen_item) {
    num_rows = get_list_num_rows(mem1, "MemList");
    if (num_rows > -1) {
      outfile = g_file_new_for_path(filename);
      output = g_file_replace(outfile, NULL, false, G_FILE_CREATE_REPLACE_DESTINATION, NULL, &err);
      if (!err) {
        for (i = 0; i < num_rows; i++) {
          get_list_text(gen_item, i, CHAN, &chan_str);
          get_list_text(gen_item, i, FREQ, &freq_str);
          get_list_text(gen_item, i, VFO, &vfo_str);
          get_list_text(gen_item, i, ANT, &ant_str);
          get_list_text(gen_item, i, MODE, &mode_str);
          get_list_text(gen_item, i, WIDTH, &bw_str);
          get_list_text(gen_item, i, NAME, &name_str);
          result = g_output_stream_printf((GOutputStream*)output, &outsize, NULL, &err, "%s;%s;%s;%s;%s;%s;%s\n",
                                     chan_str, freq_str, vfo_str, ant_str, mode_str, bw_str, name_str);
          if (result == false) {
            if (err) {
              g_print("writing failed - %s\n",err->message);
              g_error_free(err);
            } else
              g_print("writing failed\n");
          }
          g_free(chan_str);
          g_free(freq_str);
          g_free(vfo_str);
          g_free(ant_str);
          g_free(mode_str);
          g_free(bw_str);
          g_free(name_str);
        }
        g_output_stream_close((GOutputStream*)output, NULL, &err);
        if (err) {
          g_print("stream closure failed - %s\n",err->message);
          g_error_free(err);
        }
        g_object_unref(output);
        g_object_unref(outfile);
      } else {
        g_print("stream creation failed - %s\n",err->message);
        g_error_free(err);
      }
    }
  }
  g_free(filename);
}


void
get_mem_file		                (gint	mem_file_mode)
{
  GtkWidget*		file_selector;
  GtkFileFilter*	file_filter;
  GError*		err = nullptr;
  FILE*			fp1;
  GFile*		fromPath;
  GFile*		toPath;
  gchar*		temp;
  gchar*		temp2;
  gchar*		new_file;
  gchar*		prompt;
  gchar*		title;
  gchar*		file_name;
  gchar*		outStr;
  gint			i;
  gint			num_mems;
  gint			result;
  
  enum {
    SGC_MEM_FILE_NEW = 0,
    SGC_MEM_FILE_OPEN,
    SGC_MEM_FILE_SAVEAS,
    SGC_NUM_MEM_FILE_OPTIONS
  };
  
  switch (mem_file_mode) {
    case SGC_MEM_FILE_NEW :
      title = "New Memory File";
      temp = "create a new";
      break;
    case SGC_MEM_FILE_SAVEAS :
      title = "Save Memory File As";
      temp = "save the current";
      break;
    default :
      title = "";
      temp = "";
      break;
  }
  
  prompt = g_strdup_printf("%s %s %s\n\n%s\n%s\n%s\n", "Enter a filename to", temp, "memory file to in the box below.",
                                                           "Please use a simple filename with no path or extension.",
                                                           "The filename can't be blank nor contain spaces or special characters.",
                                                           "And, the file can't already exist.");
  
  if (mem_file_mode == SGC_MEM_FILE_OPEN) { 
    file_selector = gtk_file_chooser_dialog_new("Open Memory File",
                                                GTK_WINDOW(app1),
                                                GTK_FILE_CHOOSER_ACTION_OPEN,
                                                "_Cancel",
                                                GTK_RESPONSE_CANCEL,
                                                "_Open",
                                                GTK_RESPONSE_ACCEPT,
                                                NULL);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_selector), basepath);
    file_filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(file_filter, "*.mem");
    gtk_file_filter_set_name(file_filter, "Memory files (*.mem)");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_selector), file_filter);
    gtk_window_set_position(GTK_WINDOW(file_selector), GTK_WIN_POS_CENTER);
    gtk_window_set_modal(GTK_WINDOW(file_selector), TRUE);
    result = gtk_dialog_run(GTK_DIALOG(file_selector));
    if (result == GTK_RESPONSE_ACCEPT) {
      new_file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_selector));
      file_name = get_filename_from_full_path(new_file);
      load_memories_from_file(file_name);
      g_free(new_file);
      g_free(file_name);
    }
    gtk_widget_destroy(file_selector);
  } else {
    //prompt for a filename with no path nor extension into temp
    entry_dlg = create_EntryDlg();
    gtk_window_set_title((GtkWindow*)entry_dlg, title);
    set_text(entry_dlg,"PromptLbl", prompt);
    result = gtk_dialog_run(GTK_DIALOG(entry_dlg));
    if (result == GTK_RESPONSE_OK) {
      get_text(entry_dlg,"TextEntry", &temp);
      if (check_filename(temp)) {
        temp2 = g_strdup_printf("Filename must not be blank, nor include a path, extension, or special characters.\n\nYou provideded \"%s\".\n",temp);
        show_message("Enter Memory File", GTK_MESSAGE_WARNING, temp2);
        g_free(temp2);
      } else {
        new_file = g_strdup_printf("%s/%s.mem", basepath, temp);
        if (file_exists(new_file)) {
          // LATER - User may actually want to overwrite so need to eventually handle that...
          temp2 = g_strdup_printf("File should not already exist.\n%s already exists.\n",temp);
          show_message("Enter Memory File", GTK_MESSAGE_WARNING, temp2);
          g_free(temp2);
        } else {
          if (mem_file_mode == SGC_MEM_FILE_NEW) {
            //create blank new_file
            if (new_file) {     
              fp1 = fopen(new_file,"w");
              if (!fp1) {
                temp2 = g_strdup_printf("Can't create new memory file.\n");
                show_message("New Memory File", GTK_MESSAGE_WARNING, temp2);
                g_free(temp2);
              } else {
                //need to write out a bunch of empty memory lines before closing it
                num_mems = max_mem_ch - min_mem_ch + 1;
                for (i = 0; i < num_mems; i++) {
                  outStr = g_strdup_printf("%03d;-;-;-;-;-; \n",min_mem_ch + i);
                  fputs(outStr,fp1);
                }
                fclose(fp1);
              }
            }          
          } else {
            if (mem_file_mode == SGC_MEM_FILE_SAVEAS) {
              //copy cur_mem_file to new_file
              if ((cur_mem_file) && (new_file)) {
                temp2 = g_strdup_printf("%s/%s", basepath, cur_mem_file);
                fromPath = g_file_new_for_path(temp2);
                toPath = g_file_new_for_path(new_file);
                result = g_file_copy(fromPath, toPath, G_FILE_COPY_NONE, NULL, NULL, NULL, &err);
                if ((result != true) || (err)) {
                  g_print("error saving file %s - %s\n", new_file, err->message);
                  g_error_free(err);
                }
                g_free(temp2);
                g_object_unref(fromPath);
                g_object_unref(toPath);
              }
            }
          }
          //now open whatever we just created
          if (file_exists(new_file)) {
            temp2 = get_filename_from_full_path(new_file);
            load_memories_from_file(temp2);
            g_free(temp2);
          }
        }
        g_free(new_file);
      }
      g_free(temp);
    }
    gtk_widget_destroy(entry_dlg);
  }
  g_free(prompt);
}


void
select_from_memory_list                (GtkWidget*	inWidget,
                                        GdkEventButton*	inEvent)
{
  GtkWidget*	gen_item;
  gint		in_row;
  gint		ch;
  gchar*	text;
  gchar*	text1;

  if ((ispower) && (ismem) && (cur_mem_default)) {
    in_row = get_list_clicked_row(inWidget, inEvent);
    if (in_row != -1) {
      gen_item = lookup_widget(GTK_WIDGET(mem1),"MemList");
      if (gen_item) {
        get_list_text(gen_item,in_row,0,&text);
        get_list_text(gen_item,in_row,1,&text1);
        if (strcmp(text1,"-") != 0) {
          ch = strtol(text,NULL,10);
          if (rig_set_mem(hrig, RIG_VFO_CURR, ch) == RIG_OK) {
            ismem = TRUE;
            memch = ch;
          }
          get_all(FALSE);
        }
      }
    }
  }
}


/** scanning mode control interface **/
void
set_scan()
{
  isscan = !isscan;
  set_scan_display();
}


void
set_scan_display()
{
  set_enabled(app1,"ScanBtn",ispower);
  set_visible(app1,"ScanLbl",ispower&&isscan);
  //LATER set_visible(app1,"Scan2Lbl",ispower);
}


/** timer control interface **/
void
set_timer()
{
  istimer = !istimer;
  set_timer_display();
}


void
set_timer_display()
{
  set_enabled(app1,"TimerBtn",ispower);
  set_visible(app1,"TimerLbl",ispower&&istimer);
}


/** hamlib general control interface **/
void
hamlib_new                             (guint   rigid,
                                        gchar*  port,
                                        gchar*  port_text,
                                        gchar*  rig_make,
                                        gchar*  rig_name)
{
  const gchar*	retStr;
  gchar*	temp;
  gint		retcode;
  int		has_controls;

  cur_rig_model = rigid;
  if (cur_rig_make)
    g_free(cur_rig_make);
  cur_rig_make = g_strdup(rig_make);
  if (cur_rig_name)
    g_free(cur_rig_name);
  cur_rig_name = g_strdup(rig_name);

  set_app_title();

  hrig = rig_init(rigid);
  if (hrig) {
    if (strcmp(port,"Other...") == 0)
      strncpy(hrig->state.rigport.pathname,port_text,80);
    else
      strncpy(hrig->state.rigport.pathname,port,80);
    retcode = rig_open(hrig);
    if (retcode != RIG_OK) {
      //rig_cleanup(hrig);
      retStr = "";
      retStr = rigerror(retcode);
      temp = g_strdup_printf("There wss a problem connecting to your %s %s.\nIs it connected and powered on?\n\nHamlib error %d: %s\n", rig_make, rig_name, -retcode, retStr);
      show_message("Radio Settings", GTK_MESSAGE_ERROR, temp);
      return;
    }
    
    check_power();
    check_meter();
    check_blanker();
    check_blanker2();
    check_dnr();
    check_atten();
    check_preamp();
    check_notch();
    check_anf();
    check_vfo();
    check_bw();
    check_ant();
    check_agc();
    check_mode();
    check_freq();
    check_mem();
    check_af();
    check_rf();
    check_if();
    check_pi();
    check_po();
    check_cp();
    check_sq();
    check_no();
    check_nr();
    check_ap();
    check_to();
    check_bwc();
    has_controls = (has_af || has_rf || has_if || has_pi || has_po ||
      has_cp || has_sq || has_no || has_nr || has_ap ||
      has_to || has_bwc);
    if (get_visible(app1,"CPanel") && !has_controls)
      set_checked(app1,"control_panel1",FALSE);
    set_visible(app1,"control_panel1",has_controls);

    set_mem_list_editing();

    get_all(TRUE);
  }
}


void
hamlib_destroy()
{
  if (hrig) {
    rig_close(hrig);
    //rig_cleanup(hrig);
  }
}


/** hamlib radio list control interface **/
GSList*
get_hamlib_radio_list()
{
  if (!rig_list) {
    rig_load_all_backends();
    rig_list_foreach(rig_list_make, NULL);
  }
  return rig_list;
}


gint
rig_list_make                          (const struct rig_caps*	caps,
                                        gpointer		data)
{
  rig_list = g_slist_append(rig_list, (gpointer) caps);
  return 1;
}


void
hamlib_fill_radio_list                 (GSList* rig_list,
                                        GtkTreeView* c_list)
{
  GtkTreeIter		rig_iter;
  GtkListStore*		rig_store;
  struct rig_caps*	rcaps;
  gchar*		buf;
  gchar**		text;
  guint			num_rigs;
  guint			i;

  enum {
    MODEL = 0,
    BRAND,
    MNAME,
    VERSN,
    NUM_COLS
  };
    
  rig_store = gtk_list_store_new( NUM_COLS, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, -1);
  if (rig_store) {
    num_rigs = g_slist_length(rig_list);
    for (i=0; i < num_rigs; i++) {
      rcaps = (struct rig_caps*) g_slist_nth_data(rig_list,i);
      gtk_list_store_append (rig_store, &rig_iter);
      buf = g_strdup_printf("%d;%s;%s;%s", rcaps->rig_model,
	                                   rcaps->mfg_name,
	                                   rcaps->model_name,
                                           rcaps->version);
      text = g_strsplit(buf, ";", 4);
      gtk_list_store_set (rig_store, &rig_iter,
                          MODEL, atoi(text[0]),
                          BRAND, text[1],
                          MNAME, text[2],
  			  VERSN, text[3],
  			  -1);
      g_free(buf);
      g_strfreev(text);
    }
  }
  gtk_tree_view_set_model (c_list, (GtkTreeModel*)rig_store);
  g_object_unref(rig_store);
}


void
hamlib_riglist_select_id		(guint	rig_id)
{
  GtkWidget*	gen_item;
  GtkTreeModel*	list_model;
  GtkTreeSelection*	list_sel;
  GtkTreePath*	list_path;
  GtkTreeIter	iter;
  guint		r_id;
  gboolean	r_found;
  
  r_found = false;
  gen_item = lookup_widget(opt1, "RadioList");
  if (gen_item) {
    list_model = gtk_tree_view_get_model(GTK_TREE_VIEW(gen_item));
    if (list_model) {
      if (gtk_tree_model_get_iter_first(list_model, &iter)) {
        gtk_tree_model_get(list_model, &iter, 0, &r_id, -1);
        r_found = (r_id == rig_id);
        if (!r_found) {
          while ((!r_found) && gtk_tree_model_iter_next(list_model, &iter)) {
            gtk_tree_model_get(list_model, &iter, 0, &r_id, -1);
            r_found = (r_id == rig_id);
          }
        }
        if (r_found) {
          list_sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(gen_item));
          if (list_sel) {
            gtk_tree_selection_select_iter(list_sel, &iter);
            list_path = gtk_tree_model_get_path(list_model, &iter);
            if (list_path) {
              gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(gen_item), list_path, NULL, false, 0, 0);
            }
          }       
        }
      }
    }
  }
}


void
hamlib_riglist_set_defaults()
{
  GtkWidget* port_widget;
  GtkTreeModel* port_model;
  GtkTreeSelection* port_select;
  GtkTreeIter port_iter;
  guint rig_id = -1;
  gchar* rig_make[25];
  gchar* rig_name[25];
  gchar* rig_ver[25];
  gchar* port_str = NULL;
  gchar* port_text = NULL;

  port_widget = lookup_widget(opt1,"RadioList");
  if (port_widget) {
    port_select = gtk_tree_view_get_selection(GTK_TREE_VIEW(port_widget));
    if (port_select) {    
      port_model = gtk_tree_view_get_model(GTK_TREE_VIEW(port_widget));
      if (gtk_tree_selection_get_selected (port_select, &port_model, &port_iter))  {
        gtk_tree_model_get(port_model, &port_iter, 0, &rig_id, 1, &rig_make, 2, &rig_name, 3, &rig_ver, -1);
        g_settings_set_int(sgc_settings,"rig-model",rig_id);
        g_settings_set_string(sgc_settings,"rig-make",rig_make[0]);
        g_settings_set_string(sgc_settings,"rig-name",rig_name[0]);
      }
    }
  }
  port_widget = lookup_widget(opt1,"PortBoxEdit");
  if (port_widget) {
    port_str = gtk_editable_get_chars(GTK_EDITABLE(port_widget),0,-1);
    g_settings_set_string(sgc_settings,"rig-port",port_str);
  }
  port_widget = lookup_widget(opt1,"PortEdit");
  if (port_widget) {
    port_text = gtk_editable_get_chars(GTK_EDITABLE(port_widget),0,-1);
    g_settings_set_string(sgc_settings,"port-text",port_text);
  }
  hamlib_destroy();
  hamlib_new(rig_id,port_str,port_text,rig_make[0],rig_name[0]);
}


void
set_visual_defaults()
{
  hamlib_destroy();

  g_settings_set_boolean(sgc_settings,"keypad",get_checked(app1,"keypad1"));
  g_settings_set_boolean(sgc_settings,"controlpanel",get_checked(app1,"control_panel1"));
  g_settings_set_boolean(sgc_settings,"history",get_checked(app1,"history1"));
  g_settings_set_boolean(sgc_settings,"bandplan",get_checked(app1,"band_plan1"));
  //LATER g_settings_set_boolean(sgc_settings,"lock_freq",get_down(tun1,"FreqLockBtn")?);
  g_settings_sync();
  
  g_object_unref(builder);	
}
