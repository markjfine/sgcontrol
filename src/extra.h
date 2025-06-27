/*
 * Smart Gnome Control - faceplate functions and hamlib interface definition file
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

#ifndef HAVE_EXTRA_H
#define HAVE_EXTRA_H 1
#pragma once

#include <hamlib/rig.h>

GtkWidget*
lookup_widget				(GtkWidget*	widget,
                                         const gchar*	widget_name);

gint
get_theme_style(void);

void
set_opt_glyphs(void);

void
set_glyphs_to_theme			(gint	theme_type);

void
set_power_color();

gboolean
check_filename			       (gchar*		inStr);

void
show_message                           (gchar*		in_title,
					GtkMessageType	in_type,
                                        gchar*		in_message);
										
gint
show_yes_no_message                    (gchar*		in_title,
					GtkMessageType	in_type,
                                        gchar*		in_message);

void
process_keys                           (GtkWidget*   in_widget,
                                        GdkEventKey* in_event);

gchar*
get_pathname_from_full_path            (gchar* in_path);

gchar*
get_filename_from_full_path            (gchar* in_path);

gchar*
get_extension_from_full_path           (gchar* in_path);

void
set_app_title(void);

void
update_immediate(void);

void
set_widget_color	               (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        GdkRGBA         inColor);

void
set_svg_color				(GtkWidget*	inWidget,
                                         gchar*		widget_item,
                                         gchar*		filename,
                                         GdkRGBA	inColor);

void
set_visible                            (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        gboolean        status);

gboolean
get_visible                            (GtkWidget*      inWidget,
                                        gchar*          widget_item);

void
toggle_visible                         (GtkWidget*      item,
                                        gchar*          widget_item);

gboolean
get_focus                              (GtkWidget*      inWidget,
                                        gchar*          widget_item);

void
set_focus                              (GtkWidget*      inWidget,
                                        gchar*          widget_item);

void
set_enabled                            (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        gboolean        status);

gboolean
get_enabled                            (GtkWidget*      inWidget,
                                        gchar*          widget_item);

void
toggle_enabled                         (GtkMenuItem*    menuitem,
                                        gchar*          widget_item);

void
set_down                               (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        gboolean        status);

gboolean
get_down                               (GtkWidget*      inWidget,
                                        gchar*          widget_item);

void
set_checked                            (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        gboolean        status);

gboolean
get_checked                            (GtkWidget*      inWidget,
                                        gchar*          widget_item);

void
set_scale_range                        (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        gran_t          range,
                                        int             is_float);

void
set_scale_value                        (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        gdouble         value);

gdouble
get_scale_value                        (GtkWidget*      inWidget,
                                        gchar*          widget_item);

void
set_toolbar_style                      (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        GtkToolbarStyle style);

void
set_toolbar_icon_size                  (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        GtkIconSize 	icon_size);

void
add_menu_item                          (GtkWidget*	opt_menu,
                                        gchar*		menu_str);

void
set_option                             (GtkWidget*      inWidget,
                                        gchar*          widget_item,
                                        guint           item);

void
set_text                               (GtkWidget*      inWidget,
                                        gchar*		widget_item,
                                        gchar*		text);

void
get_text                               (GtkWidget*      inWidget,
                                        gchar*		widget_item,
                                        gchar**		text);

void
set_display_cursor                     (GtkWidget*	inWidget,
                                        GdkCursorType	inCursor);

void
set_display_mouse                      (GtkWidget*	inWidget,
                                        GdkEventButton*	inEvent);

gint
get_list_num_rows                      (GtkWidget*	inWidget,
                                        gchar*		list_name);

gint
get_list_selected_row                  (GtkWidget*	inWidget,
                                        gchar*		list_name);

void
set_list_selected_row                  (GtkWidget*	inWidget,
                                        gchar*		list_name,
                                        gint		in_row);

void
select_from_x_y				(GtkWidget*	inWidget,
                                         GtkWidget*	selection);
                                        										
void
get_list_clicked_row_col               (GtkWidget*	inWidget,
                                        GdkEventButton*	inEvent,
                                        gint*		in_row,
                                        gint*		in_col);
										
gint
get_list_clicked_row                   (GtkWidget*	inWidget,
                                        GdkEventButton*	inEvent);

void
get_list_text			       (GtkWidget*	inWidget,
					gint		in_row,
					gint		in_col,
					gchar**		text);

gint
get_notebook_page                      (GtkWidget*	inWidget,
                                        gchar*		notebook);

gboolean
update_clocks(void);

void
updateLocal                            (GtkWidget* inWidget);

void
updateUTC                              (GtkWidget* inWidget);

gchar*
get_current_date(void);

char
get_current_dow_letter(void);

void
fill_band_display(void);

void
do_BandPlan_clicked                    (GtkWidget*      widget,
                                        GdkEventButton* event);

double
get_grid_freq                          (GtkWidget*	in_widget,
                                        gint		in_row,
                                        gint		in_col);

void
tune_from_db			       (double		inFreq);

void
tune_from_grid                         (GtkWidget*	in_widget,
                                        gint		in_row,
                                        gint		in_col);

void
init_hist_display(void);

double
get_history_freq                       (gint		in_row);

void
tune_from_history                      (gint		in_row,
                                        gint		set_row);

void
add_to_history(void);

void
set_history_buttons(void);

void
reset_all_radio_parameters(void);

void
get_all                                (int		do_history);

void
check_power(void);

void
set_power(void);

void
get_power(void);

void
set_power_display(void);

void
set_tuner_display			(guint		key_code);

void
set_tuning_help				(double		amount);

void
handle_tuning                          (GtkWidget*	in_widget,
                                        GdkEventButton*	in_event,
                                        double		amount);

void
tune_up_down                           (double		amount);

void
mem_up_down                            (double		amount);

void
check_freq(void);

void
set_freq                               (gboolean	do_history);

void
get_freq(void);

gchar*
get_meterband(void);

void
set_freq_display_value                 (GtkWidget*	inWidget,
                                        double		freq_val);

void
set_freq_display_text                  (GtkWidget*	inWidget,
                                        gchar*		freq_str);

void
set_freq_display(void);

void
update_freq_db_display(void);

void
check_meter(void);

gboolean
get_meter(void);

void
set_meter_display_value                (GtkWidget* inWidget);

void
set_meter_display(void);

void
check_vfo(void);

void
on_vfo_selected                        (GtkWidget*	opt_menu,
                                        gpointer        data);

void
set_vfo                                (gint	item_index);

void
get_vfo(void);

void
set_vfo_display(void);

void
check_ant(void);

void
on_ant_selected                        (GtkWidget*	opt_menu,
                                        gpointer        data);

void
set_ant                                (gint	item_index);

void
get_ant(void);

void
set_ant_display(void);

void
check_agc(void);

void
on_agc_selected                        (GtkWidget*	opt_menu,
                                        gpointer        data);

void
set_agc                                (gint	item_index);

void
get_agc(void);

void
set_agc_display(void);

void
check_bw(void);

void
on_bw_selected                         (GtkWidget*	opt_menu,
                                        gpointer        data);

void
set_bw                                 (gint	item_index);

void
set_bw_display(void);

gchar*
get_mode_str                           (gint	in_mode);

void
check_mode(void);

void
on_mode_selected                       (GtkWidget*	opt_menu,
                                        gpointer        data);

void
set_mode                               (gint	item_index);

void
set_mode_display(void);

void
get_mode_bw(void);

void
check_blanker(void);

void
set_blanker(void);

void
get_blanker(void);

void
set_blanker_display(void);

void
check_blanker2(void);

void
set_blanker2(void);

void
get_blanker2(void);

void
set_blanker2_display(void);

void
check_dnr(void);

void
set_dnr(void);

void
get_dnr(void);

void
set_dnr_display(void);

void
check_atten(void);

void
set_atten(void);

void
get_atten(void);

void
set_atten_display(void);

void
check_preamp(void);

void
set_preamp(void);

void
get_preamp(void);

void
set_preamp_display(void);

void
check_notch(void);

void
set_notch(void);

void
get_notch(void);

void
set_notch_display(void);

void
check_anf(void);

void
set_anf(void);

void
get_anf(void);

void
set_anf_display(void);

void
check_af(void);

void
set_af                                 (GtkAdjustment*	adj,
                                        gpointer	gptr);

void
get_af(void);

void
set_af_display(void);

void
check_rf(void);

void
set_rf                                 (GtkAdjustment*	adj,
                                        gpointer	gptr);

void
get_rf(void);

void
set_rf_display(void);

void
check_if(void);

void
set_if                                 (GtkAdjustment*	adj,
                                        gpointer	gptr);

void
get_if(void);

void
set_if_display(void);

void
check_pi(void);

void
set_pi                                 (GtkAdjustment*	adj,
                                        gpointer	gptr);

void
get_pi(void);

void
set_pi_display(void);

void
check_po(void);

void
set_po                                 (GtkAdjustment*	adj,
                                        gpointer	gptr);

void
get_po(void);

void
set_po_display(void);

void
check_cp(void);

void
set_cp                                 (GtkAdjustment*	adj,
                                        gpointer	gptr);

void
get_cp(void);

void
set_cp_display(void);

void
check_sq(void);

void
set_sq                                 (GtkAdjustment*	adj,
                                        gpointer	gptr);

void
get_sq(void);

void
set_sq_display(void);

void
check_no(void);

void
set_no                                 (GtkAdjustment*	adj,
                                        gpointer	gptr);

void
get_no(void);

void
set_no_display(void);

void
check_nr(void);

void
set_nr                                 (GtkAdjustment*	adj,
                                        gpointer	gptr);

void
get_nr(void);

void
set_nr_display(void);

void
check_ap(void);

void
set_ap                                 (GtkAdjustment*	adj,
                                        gpointer	gptr);

void
get_ap(void);

void
set_ap_display(void);

void
check_to(void);

/*
void
set_to                                 (GtkAdjustment*	adj,
                                        gpointer	gptr);

void
get_to(void);

void
set_to_display(void);*/

void
check_bwc(void);

void
set_bwc                                (GtkAdjustment*	adj,
                                        gpointer	gptr);

void
get_bwc(void);

void
set_bwc_display(void);

void
set_tape(void);

void
set_tape_display(void);

void
check_mem(void);

void
set_mem(void);

void
get_mem(void);

void
fill_all_memory_channels_from_list(void);

void
set_mem_display(void);

void
get_memory_list(void);

void
set_mem_list_editing(void);

void
load_memories_from_file                (gchar*	infilename);

void
save_memories_to_file                  (gchar*	outfilename);

void
get_mem_file		               (gint	mem_file_mode);

void
select_from_memory_list                (GtkWidget*	inWidget,
                                        GdkEventButton*	inEvent);

void
set_scan(void);

void
set_scan_display(void);

void
set_timer(void);

void
set_timer_display(void);

void
hamlib_new                             (guint   rigid,
                                        gchar*  port,
                                        gchar*  port_text,
                                        gchar*  rig_make,
                                        gchar*  rig_name);

void
hamlib_destroy(void);

GSList*
get_hamlib_radio_list(void);


void
hamlib_fill_radio_list                 (GSList* rig_list,
                                        GtkTreeView* c_list);

void
hamlib_riglist_select_id		(guint	rig_id);

void
hamlib_riglist_set_defaults(void);

void
set_visual_defaults(void);

#endif
