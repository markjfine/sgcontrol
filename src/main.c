/*
 * Smart Gnome Control - main application
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

#include <locale.h>
#include <gtk/gtk.h>
#include <stdbool.h>

#include "interface.h"
#include "extra.h"
#include "database.h"
#include "logfile.h"

GtkWidget*	app1;
GtkWidget*	l_popup;
GtkWidget*	b_popup;
GtkWidget*	abt1;
GtkWidget*	opt1;
GtkWidget*	mem1;
GtkWidget*	tun1;
GtkWidget*	query1;
GtkWidget*	log1;
GtkWidget*	entry_dlg;
GtkWidget*	lentry;
GdkPixbuf*	meter_pix;
GdkPixbuf*	meters_pix;
GdkPixbuf*	digit_pix;
GdkPixbuf*	digits_pix;
gchar*		high_color_str;
gchar*		reg_color_str;
gchar*		inact_color_str;
gchar*		bg_color_str;
gchar*		bg_color_str2;
guint		cur_rig_model;
gchar*		cur_rig_make;
gchar*		cur_rig_name;
gboolean	has_noise;
gboolean	has_noise2;
gboolean	has_dnr;
gboolean	has_atten;
gboolean	has_preamp;
gboolean	has_notch;
gboolean	has_anf;
gboolean	has_vfo;
gboolean	has_get_power;
gboolean	has_set_power;
gboolean	has_get_freq;
gboolean	has_get_vfo;
gboolean	has_set_vfo;
gboolean    	has_get_agc;
gboolean    	has_set_agc;
gboolean    	has_get_ant;
gboolean    	has_set_ant;
gboolean    	has_get_mode;
gboolean    	has_set_mode;
gboolean    	has_get_mem;
gboolean    	has_set_mem;
gboolean    	has_get_channel;
gboolean    	has_set_channel;
gboolean	has_meter;
gboolean	ispower;
gboolean    	isnoise;
gboolean    	isnoise2;
gboolean    	isdnr;
gboolean    	isnotch;
gboolean    	isanf;
gboolean    	istape;
gboolean    	ismem;
gboolean    	isscan;
gboolean    	istimer;
gboolean    	issynch;
double		frequency;
guint       	vfo;
guint       	ant;
guint       	agc;
guint       	bw;
rmode_t       	mode;
int		memch;
RIG*		hrig;
int		atten_val[5];
int		num_attens;
int		cur_atten;
int		preamp_val[5];
int		num_preamps;
int		cur_preamp;
GList*		bw_list;
GList*		mode_list;
double		minf;
double		maxf;
int		min_mem_ch;
int		max_mem_ch;
int		min_mem_edge;
int		max_mem_edge;
int		meter_level;
gboolean	tune_ok;
gchar*		e_text;
gboolean	has_af;
gran_t		af_range;
double		afval;
gboolean	has_rf;
gran_t		rf_range;
double		rfval;
gboolean	has_if;
gran_t		if_range;
double		ifval;
gboolean	has_pi;
gran_t		pi_range;
double		pival;
gboolean	has_po;
gran_t		po_range;
double		poval;
gboolean	has_cp;
gran_t		cp_range;
double		cpval;
gboolean	has_sq;
gran_t		sq_range;
double		sqval;
gboolean	has_no;
gran_t		no_range;
double		noval;
gboolean	has_nr;
gran_t		nr_range;
double		nrval;
gboolean	has_ap;
gran_t		ap_range;
double		apval;
gboolean	has_to;
gran_t		to_range;
double		toval;
gboolean	has_bwc;
gran_t		bwc_range;
double		bwcval;
int		hist_index;
int		hist_total;
double		sml_step;
double		lrg_step;

gchar*		db_name;
gchar*		db_name2;
time_t		db_date;
gchar*      	db_ext_name;
gint		db_filt;
gchar*		db_language_filt;
gchar*		db_country_filt;
gchar*		db_region_filt;
gchar*		db_target_filt;
gint		currentDow;
double		curTime;
int		num_freqs;
int		num_xmtrs;
gint		cur_row;

gchar*		basepath;
gchar*		basedbpath;
gchar*		baselogpath;
gchar*		basemempath;
gchar*		log_name;
GtkWidget*	log_selector;
GtkBuilder* 	builder;
GSettings*	sgc_settings;
GtkCssProvider*	sgc_provider;

gboolean	auto_update = true;
gboolean	doIncludeBCB = true;

int
main (int argc, char *argv[])
{
  gint		theme_type;

  setlocale(LC_ALL, "");
  gtk_init (&argc, &argv);
    
  theme_type = get_theme_style();
     
  gtk_window_set_default_icon_from_file(PACKAGE_DATA_DIR "/sgcontrol.png", NULL);

  guint rig_model = 0;
  gchar* rig_make = "";
  gchar* rig_name = "";
  gchar* rig_port = "";
  gchar* port_text = "";
  guint show_pad = true;
  guint show_cpanel = false;
  guint show_hist = false; 
  guint show_plan = false;
  //guint lock_freq = false;

  db_name = "";
  db_name2 = "";
  db_date = 0;
  db_filt = 0;
  db_language_filt = "";
  db_country_filt = "";
  db_region_filt = "";
  db_target_filt = "";
  log_name = "";  
  db_ext_name = g_strdup("<none>");

  sgc_settings = g_settings_new ("org.markjfine.sgcontrol");
  if (sgc_settings) {
  	rig_model = g_settings_get_int(sgc_settings,"rig-model");
  	rig_make = g_settings_get_string(sgc_settings,"rig-make");
  	rig_name = g_settings_get_string(sgc_settings,"rig-name");
  	rig_port = g_settings_get_string(sgc_settings,"rig-port");
  	port_text = g_settings_get_string(sgc_settings,"port-text");
  	show_pad = g_settings_get_boolean(sgc_settings,"keypad");
  	show_cpanel = g_settings_get_boolean(sgc_settings,"controlpanel");
  	show_hist = g_settings_get_boolean(sgc_settings,"history");
  	show_plan = g_settings_get_boolean(sgc_settings,"bandplan");
  	db_name = g_settings_get_string(sgc_settings,"last-db");
  	db_date = g_settings_get_int(sgc_settings,"last-db-date");
  	db_filt = g_settings_get_int(sgc_settings,"last-filter");
  	db_language_filt = g_settings_get_string(sgc_settings,"last-language-filter");
  	db_country_filt = g_settings_get_string(sgc_settings,"last-country-filter");
  	db_region_filt = g_settings_get_string(sgc_settings,"last-region-filter");
  	db_target_filt = g_settings_get_string(sgc_settings,"last-target-filter");
  	auto_update = g_settings_get_boolean(sgc_settings,"autoupdate");
  	doIncludeBCB = g_settings_get_boolean(sgc_settings,"include-bcb");
  	//lock_freq = g_settings_get_boolean(sgc_settings,"lock-freq");
  	log_name = g_settings_get_string(sgc_settings,"last-log");
  	//btn_style = g_settings_get_string(sgc_settings,"toolbar_style");
  }
      
  app1 = create_app1 ();
  
  /*get off-screen images */
  meter_pix = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, 52, 32);
  meters_pix = gtk_image_get_pixbuf(GTK_IMAGE(lookup_widget(GTK_WIDGET(app1),"meters_pix")));
  digit_pix = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, 188, 25);
  digits_pix = gtk_image_get_pixbuf(GTK_IMAGE(lookup_widget(GTK_WIDGET(app1),"digits_pix")));
    
  //About, Options and Entry are created dynamically
  mem1 = create_MemWindow();
  tun1 = create_TuningDlg();
  log1 = create_LogWindow();

  /*set up faceplate colors */
  GdkRGBA	fg_color = {(double)0.0,(double)255.0,(double)0.0,(double) 1.0};
  GdkRGBA	bg_color = {(double)0.0,(double)0.0,(double)0.0,(double) 1.0};
  
  //Set up color changes
  sgc_provider = gtk_css_provider_new();
  if (sgc_provider) {
    if (gtk_css_provider_load_from_path(sgc_provider, PACKAGE_DATA_DIR"/radio_display.css", NULL)) {
    } else
      g_print("no css\n");
  } else
    g_print("no provider\n");
  
  gtk_widget_set_app_paintable(lookup_widget(app1,"RadioDisplay"),true);
  set_widget_color(app1,"RadioDisplay",bg_color);
  set_widget_color(app1,"eventbox1",bg_color);
  set_widget_color(app1,"eventbox2",bg_color);
  set_widget_color(app1,"eventbox3",bg_color);
  set_widget_color(app1,"MeterLbl",fg_color);
  set_widget_color(app1,"HzLbl",fg_color);
  set_widget_color(app1,"MemLbl",fg_color);
  set_widget_color(app1,"Mem2Lbl",fg_color);
  set_widget_color(app1,"LocalLbl",fg_color);
  set_widget_color(app1,"UTCLbl",fg_color);
  set_widget_color(app1,"Local2Lbl",fg_color);
  set_widget_color(app1,"UTC2Lbl",fg_color);
  set_widget_color(app1,"NoiseLbl",fg_color);
  set_widget_color(app1,"DNRLbl",fg_color);
  set_widget_color(app1,"AttLbl",fg_color);
  set_widget_color(app1,"PreampLbl",fg_color);
  set_widget_color(app1,"NotchLbl",fg_color);
  set_widget_color(app1,"ANFLbl",fg_color);
  set_widget_color(app1,"TapeLbl",fg_color);
  set_widget_color(app1,"TimerLbl",fg_color);
  set_widget_color(app1,"ScanLbl",fg_color);
  set_widget_color(app1,"Scan2Lbl",fg_color);
  set_widget_color(app1,"VFOLbl",fg_color);
  set_widget_color(app1,"AntLbl",fg_color);
  set_widget_color(app1,"AGCLbl",fg_color);
  set_widget_color(app1,"BWLbl",fg_color);
  set_widget_color(app1,"ModeLbl",fg_color);
  set_widget_color(app1,"ModeLbl",fg_color);

  /*hide some yet unimplemented things */

  /*Main Window*/
  /*File Menu*/
  set_visible(app1,"SaveBtn",FALSE);
  set_visible(app1,"save_database1",FALSE);
  set_visible(app1,"SaveAsBtn",FALSE);
  set_visible(app1,"save_database_as1",FALSE);
  set_visible(app1,"dump_database1",FALSE);
  set_visible(app1,"PrintBtn",FALSE);
  set_visible(app1,"printer_setup1",FALSE);
  set_visible(app1,"import_database1",FALSE);
  set_visible(app1,"separator4",FALSE);
  set_visible(app1,"ExitBtn",FALSE);
  /*Edit Menu*/
  set_visible(app1,"edit1",FALSE);
  set_visible(app1,"mark1",FALSE);
  set_visible(app1,"delete1",FALSE);
  /*View Menu*/
  set_visible(app1,"pan_display1",FALSE);
  /*Radio Menu*/
  set_visible(app1,"TapeBtn",FALSE);
  set_visible(app1,"tape_vox1",FALSE);
  set_visible(app1,"separator20",FALSE);
  set_visible(app1,"other1",FALSE);
  set_visible(app1,"vseparator3",FALSE);
  /*Tuning Menu*/
  set_visible(app1,"edit_step_sizes1",FALSE);
  set_visible(app1,"separator10",FALSE);
  set_visible(app1,"ScanBtn",FALSE);
  set_visible(app1,"scan_mode1",FALSE);
  set_visible(app1,"scan_pause1",FALSE);
  set_visible(app1,"scan_parameters1",FALSE);
  set_visible(app1,"separator12",FALSE);
  set_visible(app1,"TimerBtn",FALSE);
  set_visible(app1,"timer_mode1",FALSE);
  set_visible(app1,"ShowTimerBtn",FALSE);
  set_visible(app1,"timer_parameters1",FALSE);
  set_visible(app1,"separator13",FALSE);
  set_visible(app1,"ConnectBtn",FALSE);
  set_visible(app1,"disconnect1",FALSE);
  /*Memory Menu*/
  set_visible(app1,"save1",FALSE);
  set_visible(app1,"print1",FALSE);
  set_visible(app1,"separator14",FALSE);
  set_visible(app1,"insert_channel1",FALSE);
  /*Log Menu*/
  set_visible(app1,"log1",FALSE);
  /*Tools Menu*/
  set_visible(app1,"hf_prediction1",FALSE);
  set_visible(app1,"separator9",FALSE);
  /*Help Menu*/
  set_visible(app1,"contents1",FALSE);
  set_visible(app1,"topic_search1",FALSE);
  set_visible(app1,"using_help1",FALSE);
  set_visible(app1,"separator7",FALSE);
  set_visible(app1,"faceplate_operation1",FALSE);
  set_visible(app1,"memory_management1",FALSE);
  set_visible(app1,"logging_system1",FALSE);
  set_visible(app1,"separator8",FALSE);

  /*Memory Window*/
  /*File Menu*/
  set_visible(mem1,"save3",FALSE);
  set_visible(mem1,"button20",FALSE);
  set_visible(mem1,"print4",FALSE);
  set_visible(mem1,"button22",FALSE);

  /*Edit Menu*/
  set_visible(mem1,"edit4",FALSE);
  set_visible(mem1,"button25",FALSE); //set
  set_visible(mem1,"button26",FALSE); //modify
  set_visible(mem1,"button28",FALSE); //move
  set_visible(mem1,"button27",FALSE); //insert
  set_visible(mem1,"button29",FALSE); //clear

  /*Sort Menu*/
  set_visible(mem1,"sort1",FALSE);

  /*Tools Menu*/
  set_visible(mem1,"separator23",FALSE);
  set_visible(mem1,"edit_comment1",FALSE);

  /*Database Window*/
  /*File Menu*/
  set_visible(tun1,"save_database2",FALSE);
  set_visible(tun1,"save_database_as2",FALSE);
  /*Edit Menu*/
  set_visible(tun1,"edit2",FALSE);
  /*View Menu*/
  /*Filter Menu*/

  /*Log Window*/
  /*File Menu*/
  set_visible(log1,"separator24",FALSE);
  set_visible(log1,"LogExportBtn",FALSE);
  set_visible(log1,"log_export_all",FALSE);
  set_visible(log1,"log_export_selected",FALSE);
  set_visible(log1,"separator25",FALSE);
  set_visible(log1,"LogPrintBtn",FALSE);
  set_visible(log1,"log_print",FALSE);
  set_visible(log1,"log_print_report",FALSE);

  /*Sort Menu*/
  set_visible(log1,"log_sort",FALSE);

  /*Format Menu*/
  set_visible(log1,"log_format",FALSE);

  /*Tuning buttons*/
  set_visible(log1,"vseparator20",FALSE);
  set_visible(log1,"LogDownBtn",FALSE);
  set_visible(log1,"LogUpBtn",FALSE);
  
  
  /* set display highlighting colors */
  high_color_str = "#0000FF";
  reg_color_str = "#000000";
  inact_color_str = "#909090";
  bg_color_str = "#FFFFFF";
  bg_color_str2 = "#FFFFDF";

  /*set global l&f for the toolbars */
  GtkToolbarStyle style = GTK_TOOLBAR_ICONS;
  GtkIconSize icon_size = GTK_ICON_SIZE_SMALL_TOOLBAR;
  set_toolbar_style(app1,"RadioBar",style);
  set_toolbar_icon_size(app1,"RadioBar",icon_size);
  set_toolbar_style(app1,"DatabaseBar",style);
  set_toolbar_icon_size(app1,"DatabaseBar",icon_size);
  set_glyphs_to_theme(theme_type);

  /*set up justification in clist columns */
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( app1, "freqdbRenderer"), 1.0, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( app1, "fromdbRenderer"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( app1, "todbRenderer"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( app1, "daysdbRenderer"), 0.5, 0.0);  

  fill_band_display();  
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( app1, "band1Renderer"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( app1, "start1Renderer"), 1.0, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( app1, "stop1Renderer"), 1.0, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( app1, "type1Renderer"), 0.5, 0.0);  
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( app1, "band2Renderer"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( app1, "start2Renderer"), 1.0, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( app1, "stop2Renderer"), 1.0, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( app1, "type2Renderer"), 0.5, 0.0);  
  
  init_hist_display();
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( app1, "hist_time"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( app1, "hist_freq"), 1.0, 0.0);
  
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( mem1, "chRenderer"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( mem1, "memfreqRenderer"), 1.0, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( mem1, "vfoRenderer"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( mem1, "antRenderer"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( mem1, "modeRenderer"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( mem1, "bwRenderer"), 1.0, 0.0);
  
  //FreqViewList
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( tun1, "freqRenderer"), 1.0, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( tun1, "fromRenderer"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( tun1, "toRenderer"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( tun1, "daysRenderer"), 0.5, 0.0);

  //StatViewList  
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( tun1, "freqRenderer1"), 1.0, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( tun1, "fromRenderer1"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( tun1, "toRenderer1"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( tun1, "daysRenderer1"), 0.5, 0.0);

  //LogList
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( log1, "logfrequencyRenderer"), 1.0, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( log1, "logtimeRenderer"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( log1, "logdateRenderer"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( log1, "logmodeRenderer"), 0.5, 0.0);
  gtk_cell_renderer_set_alignment((GtkCellRenderer*)lookup_widget( log1, "logsinpoRenderer"), 0.5, 0.0);
    
  rig_set_debug(RIG_DEBUG_WARN);

  gtk_widget_show (app1);

  set_checked(app1,"keypad1",show_pad);
  set_checked(app1,"control_panel1",show_cpanel);
  set_checked(app1,"history1",show_hist);
  set_checked(app1,"band_plan1",show_plan);
  
  //(tun1,"FreqLockBtn",(lock_freq == 1));

  reset_all_radio_parameters();

  prep_user_dir();

  g_timeout_add(500, (GSourceFunc)get_meter, NULL);
  update_clocks();
  g_timeout_add(5000, (GSourceFunc)update_clocks, NULL);

  if (rig_model == 0) {
    rig_model = 1;
    rig_make = "Hamlib";
    rig_name = "Dummy";
  }
  hamlib_new(rig_model,rig_port,port_text,rig_make,rig_name);

  load_memories_from_file(NULL);
  
  if (auto_update)
    checkWebPage(true);
  
  if (db_name) {
    read_statsType();
    open_database();
  }
	  
  if (db_filt != 0) {
    gtk_combo_box_set_active((GtkComboBox*)lookup_widget(tun1, "FilterData"), db_filt);   
    set_filter_data_box(lookup_widget(tun1, "FilterDataBox"), db_filt);
  } else {
    set_filter_menu_items(0);
    get_stats_records(lookup_widget(tun1,"StationsList"), "", "", "", "", true);
  }

  if (strcmp(log_name,"") == 0)
    log_name = g_strdup_printf("%s/sgcontrol.log",basepath);
  if (log_name)
    open_logfile();
  set_sort_menu_items(8);

  gtk_main ();

  return 0;
}

