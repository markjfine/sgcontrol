/*
 * Smart Gnome Control - GUI interface file
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include <gtk/gtk.h>

#include "database.h"
#include "extra.h"
#include "interface.h"
#include "logfile.h"

extern GtkBuilder* 	builder;
extern GtkWidget*	app1;
extern GtkWidget* 	l_popup;
extern GtkWidget* 	b_popup;
extern GtkWidget*	abt1;
extern GtkWidget*	opt1;
extern GtkWidget*	mem1;
extern GtkWidget*	tun1;
extern GtkWidget*	log1;
extern GtkWidget*	entry_dlg;
extern GSettings*	sgc_settings;
extern int		hist_index;
extern double		sml_step;
extern double		lrg_step;
extern guint		cur_rig_model;
extern gboolean		auto_update;
extern gboolean		doIncludeBCB;

//MAIN APP CALLBACKS
void
on_about1_activate                     (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  abt1 = create_AboutDlg();
  gtk_widget_show (abt1);
}

void
on_exit1_activate                      (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  set_visual_defaults();
  exit(0);
}

void
on_mark1_activate                      (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{

}


void
on_delete1_activate                    (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{

}

void
on_keypad1_activate                    (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  toggle_visible(GTK_WIDGET(menuitem),"Keypad");
}


void
on_band_plan1_activate                 (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  toggle_visible(GTK_WIDGET(menuitem),"BandPlanBox");
}


void
on_control_panel1_activate             (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  toggle_visible(GTK_WIDGET(menuitem),"CPanelBox");
}

void
on_history1_activate                   (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  toggle_visible(GTK_WIDGET(menuitem),"HistoryBox");
}

void
on_ExitBtn_clicked                     (GtkButton*       button,
                                        gpointer         user_data)
{
  set_visual_defaults();
  exit(0);
}

void
on_AboutBtn_clicked                     (GtkButton*	button,
                                        gpointer	user_data)
{
  abt1 = create_AboutDlg();
  gtk_widget_show (abt1);
}

void
on_app1_destroy                        (GObject*         object,
                                        gpointer         user_data)
{
  set_visual_defaults();
  exit(0);
}

void
on_PwrBtn_clicked                      (GtkButton*       button,
                                        gpointer         user_data)
{
  set_power();
}

void
on_power1_activate                     (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  set_power();
}

void
on_NoiseBtn2_released                  (GtkButton*       button,
                                        gpointer         user_data);

void
on_togglebutton1_released              (GtkButton*       button,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"NoiseBtn"), G_CALLBACK(on_togglebutton1_released), NULL);
  g_signal_handlers_block_by_func(lookup_widget(app1,"NoiseBtn2"), G_CALLBACK(on_NoiseBtn2_released), NULL);
  set_blanker();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"NoiseBtn"), G_CALLBACK(on_togglebutton1_released), NULL);
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"NoiseBtn2"), G_CALLBACK(on_NoiseBtn2_released), NULL);
}

void
on_NoiseBtn2_released                  (GtkButton*       button,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"NoiseBtn"), G_CALLBACK(on_togglebutton1_released), NULL);
  g_signal_handlers_block_by_func(lookup_widget(app1,"NoiseBtn2"), G_CALLBACK(on_NoiseBtn2_released), NULL);
  set_blanker2();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"NoiseBtn"), G_CALLBACK(on_togglebutton1_released), NULL);
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"NoiseBtn2"), G_CALLBACK(on_NoiseBtn2_released), NULL);
}

void
on_noise_blanker1_activate             (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"NoiseBtn"), G_CALLBACK(on_togglebutton1_released), NULL);
  g_signal_handlers_block_by_func(lookup_widget(app1,"NoiseBtn2"), G_CALLBACK(on_NoiseBtn2_released), NULL);
  set_blanker();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"NoiseBtn"), G_CALLBACK(on_togglebutton1_released), NULL);
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"NoiseBtn2"), G_CALLBACK(on_NoiseBtn2_released), NULL);
}

void
on_noise_blanker2_activate             (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"NoiseBtn"), G_CALLBACK(on_togglebutton1_released), NULL);
  g_signal_handlers_block_by_func(lookup_widget(app1,"NoiseBtn2"), G_CALLBACK(on_NoiseBtn2_released), NULL);
  set_blanker2();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"NoiseBtn"), G_CALLBACK(on_togglebutton1_released), NULL);
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"NoiseBtn2"), G_CALLBACK(on_NoiseBtn2_released), NULL);
}

void
on_DNRBtn_released                     (GtkButton*       button,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"DNRBtn"), G_CALLBACK(on_DNRBtn_released), NULL);
  set_dnr();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"DNRBtn"), G_CALLBACK(on_DNRBtn_released), NULL);
}

void
on_noise_reduction1_activate           (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"DNRBtn"), G_CALLBACK(on_DNRBtn_released), NULL);
  set_dnr();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"DNRBtn"), G_CALLBACK(on_DNRBtn_released), NULL);
}

void
on_attbtn_released                     (GtkButton*       button,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"AttBtn"), G_CALLBACK(on_attbtn_released), NULL);
  set_atten();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"AttBtn"), G_CALLBACK(on_attbtn_released), NULL);
}

void
on_attenuator1_activate                (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"AttBtn"), G_CALLBACK(on_attbtn_released), NULL);
  set_atten();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"AttBtn"), G_CALLBACK(on_attbtn_released), NULL);
}

void
on_preampbtn_released                  (GtkButton*       button,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"PreampBtn"), G_CALLBACK(on_preampbtn_released), NULL);
  set_preamp();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"PreampBtn"), G_CALLBACK(on_preampbtn_released), NULL);
}

void
on_preamp1_activate                    (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"PreampBtn"), G_CALLBACK(on_preampbtn_released), NULL);
  set_preamp();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"PreampBtn"), G_CALLBACK(on_preampbtn_released), NULL);
}

void
on_togglebutton2_released              (GtkButton*       button,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"NotchBtn"), G_CALLBACK(on_togglebutton2_released), NULL);
  set_notch();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"NotchBtn"), G_CALLBACK(on_togglebutton2_released), NULL);
}

void
on_notch1_activate                     (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"NotchBtn"), G_CALLBACK(on_togglebutton2_released), NULL);
  set_notch();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"NotchBtn"), G_CALLBACK(on_togglebutton2_released), NULL);
}

void
on_ANFBtn_released                     (GtkButton*       button,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"ANFBtn"), G_CALLBACK(on_ANFBtn_released), NULL);
  set_anf();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"ANFBtn"), G_CALLBACK(on_ANFBtn_released), NULL);
}

void
on_auto_notch1_activate                (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"ANFBtn"), G_CALLBACK(on_ANFBtn_released), NULL);
  set_anf();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"ANFBtn"), G_CALLBACK(on_ANFBtn_released), NULL);
}

void
on_togglebutton3_released              (GtkButton*       button,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"TapeBtn"), G_CALLBACK(on_togglebutton3_released), NULL);
  set_tape();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"TapeBtn"), G_CALLBACK(on_togglebutton3_released), NULL);
}

void
on_tape_vox1_activate                  (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"TapeBtn"), G_CALLBACK(on_togglebutton3_released), NULL);
  set_tape();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"TapeBtn"), G_CALLBACK(on_togglebutton3_released), NULL);
}

void
on_togglebutton4_released              (GtkButton*	button,
                                        gpointer	user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"MemBtn"), G_CALLBACK(on_togglebutton4_released), NULL);
  set_mem();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"MemBtn"), G_CALLBACK(on_togglebutton4_released), NULL);
}

void
on_memory_mode1_activate               (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(app1,"MemBtn"), G_CALLBACK(on_togglebutton4_released), NULL);
  set_mem();
  g_signal_handlers_unblock_by_func(lookup_widget(app1,"MemBtn"), G_CALLBACK(on_togglebutton4_released), NULL);
}

void
on_eventbox1_enter_notify_event        (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_display_cursor(widget,GDK_HAND2);
}


void
on_eventbox1_leave_notify_event        (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_display_cursor(widget,GDK_LEFT_PTR);
}


void
on_eventbox1_button_press_event        (GtkWidget*       widget,
                                        GdkEventButton*  event,
                                        gpointer         user_data)
{
  set_display_mouse(widget,event);
}


void
on_options1_activate                   (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  opt1 = create_OptionsDlg();
  set_opt_glyphs();
  gtk_widget_show (GTK_WIDGET(opt1));
}


void
on_OptionsBtn_clicked                  (GtkButton*       button,
                                        gpointer         user_data)
{
  opt1 = create_OptionsDlg();
  set_opt_glyphs();
  gtk_widget_show (GTK_WIDGET(opt1));
}

void
on_Local2Lbl_draw                      (GtkWidget*       widget,
                                        gpointer         user_data)
{
  updateLocal(widget);
}


void
on_UTC2Lbl_realize                     (GtkWidget*       widget,
                                        gpointer         user_data)
{
  updateUTC(widget);
}


void
on_update_display1_activate            (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  get_all(FALSE);
}

void
on_RefreshBtn2_clicked                 (GtkButton*       button,
                                        gpointer         user_data)
{
  get_all(FALSE);
}

void
on_kp7_clicked                         (GtkButton*       button,
                                        gpointer         user_data)
{
  set_focus(app1,"kp7");
  set_tuner_display(7);
}


void
on_kp8_clicked                         (GtkButton*       button,
                                        gpointer         user_data)
{
  set_focus(app1,"kp8");
  set_tuner_display(8);
}


void
on_kp9_clicked                         (GtkButton*       button,
                                        gpointer         user_data)
{
  set_focus(app1,"kp9");
  set_tuner_display(9);
}


void
on_kpBS_clicked                        (GtkButton*       button,
                                        gpointer         user_data)
{
  set_focus(app1,"kpBS");
  set_tuner_display(11);
}


void
on_kp4_clicked                         (GtkButton*       button,
                                        gpointer         user_data)
{
  set_focus(app1,"kp4");
  set_tuner_display(4);
}


void
on_kp5_clicked                         (GtkButton*       button,
                                        gpointer         user_data)
{
  set_focus(app1,"kp5");
  set_tuner_display(5);
}


void
on_kp6_clicked                         (GtkButton*       button,
                                        gpointer         user_data)
{
  set_focus(app1,"kp6");
  set_tuner_display(6);
}


void
on_kpEnter_clicked                     (GtkButton*       button,
                                        gpointer         user_data)
{
  set_focus(app1,"kpEnter");
  set_tuner_display(13);
}


void
on_kp1_clicked                         (GtkButton*       button,
                                        gpointer         user_data)
{
  set_focus(app1,"kp1");
  set_tuner_display(1);
}


void
on_kp2_clicked                         (GtkButton*       button,
                                        gpointer         user_data)
{
  set_focus(app1,"kp2");
  set_tuner_display(2);
}


void
on_kp3_clicked                         (GtkButton*       button,
                                        gpointer         user_data)
{
  set_focus(app1,"kp3");
  set_tuner_display(3);
}


void
on_kp0_clicked                         (GtkButton*       button,
                                        gpointer         user_data)
{
  set_focus(app1,"kp0");
  set_tuner_display(0);
}


void
on_kpDot_clicked                       (GtkButton*       button,
                                        gpointer         user_data)
{
  set_focus(app1,"kpDot");
  set_tuner_display(10);
}


void
on_HistBackBtn_clicked                 (GtkButton*       button,
                                        gpointer         user_data)
{
  hist_index--;
  tune_from_history(hist_index,TRUE);
}


void
on_HistForwBtn_clicked                 (GtkButton*       button,
                                        gpointer         user_data)
{
  hist_index++;
  tune_from_history(hist_index,TRUE);
}


void
on_HistList_button_press_event         (GtkWidget*	widget,
                                        GdkEventButton*	event,
                                        gpointer	user_data)
{
  gint in_row = get_list_clicked_row(widget,event);
  tune_from_history(in_row,FALSE);
}


void
on_new1_activate                       (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  get_mem_file(0);
}


void
on_open1_activate                       (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  get_mem_file(1);
}


void
on_save1_activate                       (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
    save_memories_to_file(NULL);
}


void
on_save_as1_activate                       (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  get_mem_file(2);
}


void
on_capture1_activate                   (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  get_memory_list();
}


void
on_fill1_activate	               (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  fill_all_memory_channels_from_list();
}


void
on_open_database1_activate             (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  get_database_file();
}


void
on_OpenBtn_clicked                     (GtkButton*	button,
                                        gpointer	user_data)
{
  get_database_file();
}


void
on_update_database1_activate           (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  checkWebPage(false);
}


void
on_UpdateBtn_clicked                   (GtkButton*	button,
                                        gpointer	user_data)
{
  checkWebPage(false);
}


void
on_show_list1_activate                 (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  gtk_widget_show (GTK_WIDGET(mem1));
}


void
on_ShowMemBtn_clicked                  (GtkButton*	button,
                                        gpointer	user_data)
{
  gtk_widget_show (GTK_WIDGET(mem1));
}


void
on_MemList_button_press_event          (GtkWidget*	widget,
                                        GdkEventButton*	event,
                                        gpointer	user_data)
{
  if (event->type == GDK_BUTTON_RELEASE)
    select_from_memory_list(widget, event);
}


void
on_show_log1_activate                  (GtkMenuItem*	menuitem,
                                        gpointer         user_data)
{
  gtk_widget_show (GTK_WIDGET(log1));
}


void
on_ShowLogBtn_clicked                  (GtkButton*	button,
                                        gpointer	user_data)
{
  gtk_widget_show (GTK_WIDGET(log1));
}


void
on_eventbox2_enter_notify_event        (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_display_cursor(widget,GDK_HAND2);
}


void
on_eventbox2_leave_notify_event        (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_display_cursor(widget,GDK_LEFT_PTR);
}


void
on_eventbox2_button_press_event        (GtkWidget*        widget,
                                        GdkEventButton*   event,
                                        gpointer          user_data)
{
  if ((event->button) == 1) {
    gtk_widget_show (mem1);
  }
}


void
on_clist1_enter_notify_event           (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_display_cursor(widget,GDK_HAND2);
}


void
on_clist1_leave_notify_event           (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_display_cursor(widget,GDK_LEFT_PTR);
}


void
on_clist1_button_press_event 		 (GtkWidget*		widget,
                                          GdkEventButton*	event,
                                          gpointer		user_data)
{
  if ((event->button) == 1)
    get_station_data(lookup_widget(app1,"FaceplateSelection"), false);
  else
    if ((event->button) == 3)
      fill_log_display_from_faceplate(lookup_widget(app1,"FaceplateSelection"));
}


void
on_button4_clicked                     (GtkButton*       button,
                                        gpointer         user_data)
{
  set_checked(app1,"control_panel1",FALSE);
}


void
on_button5_clicked                     (GtkButton*       button,
                                        gpointer         user_data)
{
  set_checked(app1,"history1",FALSE);
}


void
on_button6_clicked                     (GtkButton*       button,
                                        gpointer         user_data)
{
  set_checked(app1,"band_plan1",FALSE);
}


void
on_tuning_database1_activate           (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  gtk_widget_set_visible(tun1, true);
}


void
on_DBViewBtn_clicked                    (GtkButton*	button,
                                        gpointer	user_data)
{
  gtk_widget_set_visible(tun1, true);
}


void
on_eventbox3_enter_notify_event        (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_display_cursor(widget,GDK_HAND2);
}


void
on_eventbox3_leave_notify_event        (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_display_cursor(widget,GDK_LEFT_PTR);
}


void
on_eventbox3_button_press_event        (GtkWidget*        widget,
                                        GdkEventButton*   event,
                                        gpointer          user_data)
{
  if ((event->button) == 1)
    set_checked(app1,"band_plan1",!get_visible(app1,"BandPlanBox"));
}


void
on_BCBandList_button_press_event       (GtkWidget*	widget,
                                        GdkEventButton*	event,
                                        gpointer	user_data)
{
  do_BandPlan_clicked(widget,event);
}


void
on_UteBandList_button_press_event      (GtkWidget*	widget,
                                        GdkEventButton*	event,
                                        gpointer	user_data)
{
  do_BandPlan_clicked(widget,event);
}


void
on_LLeftBtn_button_press_event         (GtkWidget*       widget,
                                        GdkEventButton*  event,
                                        gpointer         user_data)
{
  handle_tuning(widget,event,-sml_step);
}


void
on_LeftBtn_button_press_event          (GtkWidget*       widget,
                                        GdkEventButton*  event,
                                        gpointer         user_data)
{
  handle_tuning(widget,event,-lrg_step);
}


void
on_RightBtn_button_press_event         (GtkWidget*       widget,
                                        GdkEventButton*  event,
                                        gpointer         user_data)
{
  handle_tuning(widget,event,lrg_step);
}


void
on_RRightBtn_button_press_event        (GtkWidget*       widget,
                                        GdkEventButton*  event,
                                        gpointer         user_data)
{
  handle_tuning(widget,event,sml_step);
}

void
on_step010_activate                    (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  lrg_step = 0.10;
}


void
on_step050_activate                    (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  lrg_step = 0.50;
}


void
on_step1_activate                      (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  lrg_step = 1.00;
}


void
on_step2_activate                      (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  lrg_step = 2.00;
}


void
on_step3_activate                      (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  lrg_step = 3.00;
}


void
on_step5_activate                      (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  lrg_step = 5.00;
}


void
on_step9_activate                      (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  lrg_step = 9.00;
}


void
on_step10_activate                     (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  lrg_step = 10.00;
}


void
on_step100_activate                    (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  lrg_step = 100.00;
}


void
on_step1000_activate                   (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  lrg_step = 1000.00;
}


void
on_lstep001_activate                   (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  sml_step = 0.01;
}


void
on_lstep002_activate                   (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  sml_step = 0.02;
}


void
on_lstep003_activate                   (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  sml_step = 0.03;
}


void
on_lstep005_activate                   (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  sml_step = 0.05;
}


void
on_lstep010_activate                   (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  sml_step = 0.10;
}


void
on_lstep020_activate                   (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  sml_step = 0.20;
}


void
on_lstep030_activate                   (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  sml_step = 0.30;
}


void
on_lstep050_activate                   (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  sml_step = 0.50;
}


void
on_lstep1_activate                     (GtkMenuItem*     menuitem,
                                        gpointer         user_data)
{
  sml_step = 1.00;
}


void
on_LLeftBtn_enter_notify_event         (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_tuning_help(-sml_step);
}


void
on_LLeftBtn_leave_notify_event         (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_tuning_help(0.00);
}


void
on_LeftBtn_enter_notify_event          (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_tuning_help(-lrg_step);
}


void
on_LeftBtn_leave_notify_event          (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_tuning_help(0.00);
}



void
on_RightBtn_enter_notify_event         (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_tuning_help(lrg_step);
}


void
on_RightBtn_leave_notify_event         (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_tuning_help(0.00);
}


void
on_RRightBtn_enter_notify_event        (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_tuning_help(sml_step);
}


void
on_RRightBtn_leave_notify_event        (GtkWidget*        widget,
                                        GdkEventCrossing* event,
                                        gpointer          user_data)
{
  set_tuning_help(0.00);
}


gboolean
on_app1_key_press_event                (GtkWidget*       widget,
                                        GdkEventKey*     event,
                                        gpointer         user_data)
{
  gboolean	retval;
  
  retval = false;
  if ((event->keyval == GDK_KEY_Left) ||
      (event->keyval == GDK_KEY_KP_Left) ||
      (event->keyval == GDK_KEY_Right) ||
      (event->keyval == GDK_KEY_KP_Right)) {
    retval = true;
    process_keys(widget,event);
  }
  /*if (event->keyval == GDK_KEY_Escape) {
    retval = true;
    set_power();
  }
  if ((event->keyval == GDK_KEY_F9) && (event->state == GDK_MOD1_MASK)) {
    retval = true;
    on_open_database1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_X) && (event->state == GDK_MOD1_MASK)) {
    retval = true;
    on_exit1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_K) && (event->state == GDK_MOD1_MASK)) {
    retval = true;
    on_keypad1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_P) && (event->state == GDK_MOD1_MASK)) {
    retval = true;
    on_control_panel1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_H) && (event->state == GDK_MOD1_MASK)) {
    retval = true;
    on_history1_activate(NULL, NULL);
  }
  if (event->keyval == GDK_KEY_F2) {
    retval = true;
    on_band_plan1_activate(NULL, NULL);
  }
  if (event->keyval == GDK_KEY_F5) {
    retval = true;
    on_tuning_database1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_H) && (event->state == GDK_CONTROL_MASK)) {
    retval = true;
    on_show_list1_activate(NULL, NULL);
  }
  if (event->keyval == GDK_KEY_F4) {
    retval = true;
    on_show_log1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_U) && (event->state == GDK_MOD1_MASK)) {
    retval = true;
    on_update_display1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_B) && (event->state == GDK_CONTROL_MASK)) {
    retval = true;
    on_noise_blanker1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_B) && (event->state == (GDK_SHIFT_MASK | GDK_CONTROL_MASK))) {
    retval = true;
    on_noise_blanker2_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_O) && (event->state == GDK_CONTROL_MASK)) {
    retval = true;
    on_noise_reduction1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_A) && (event->state == GDK_CONTROL_MASK)) {
    retval = true;
    on_attenuator1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_R) && (event->state == GDK_CONTROL_MASK)) {
    retval = true;
    on_preamp1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_N) && (event->state == GDK_CONTROL_MASK)) {
    retval = true;
    on_notch1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_X) && (event->state == GDK_CONTROL_MASK)) {
    retval = true;
    on_auto_notch1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_E) && (event->state == GDK_CONTROL_MASK)) {
    retval = true;
    on_memory_mode1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_W) && (event->state == GDK_CONTROL_MASK)) {
    retval = true;
    on_capture1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_Y) && (event->state == GDK_CONTROL_MASK)) {
    retval = true;
    on_fill1_activate(NULL, NULL);
  }
  if ((event->keyval == GDK_KEY_P) && (event->state == GDK_CONTROL_MASK)) {
    retval = true;
    on_options1_activate(NULL, NULL);
  }*/
  return retval;
}


void
on_app1_key_press_event_after		(GtkWidget*	widget,
                                        GdkEventKey*	event,
                                        gpointer	user_data)
{
  /*if ((event->keyval == GDK_KEY_0) ||
      (event->keyval == GDK_KEY_1) ||
      (event->keyval == GDK_KEY_2) ||
      (event->keyval == GDK_KEY_3) ||
      (event->keyval == GDK_KEY_4) ||
      (event->keyval == GDK_KEY_5) ||
      (event->keyval == GDK_KEY_6) ||
      (event->keyval == GDK_KEY_7) ||
      (event->keyval == GDK_KEY_8) ||
      (event->keyval == GDK_KEY_9) ||
      (event->keyval == GDK_KEY_period) ||
      (event->keyval == GDK_KEY_Return) ||
      (event->keyval == GDK_KEY_KP_0) ||
      (event->keyval == GDK_KEY_KP_1) ||
      (event->keyval == GDK_KEY_KP_2) ||
      (event->keyval == GDK_KEY_KP_3) ||
      (event->keyval == GDK_KEY_KP_4) ||
      (event->keyval == GDK_KEY_KP_5) ||
      (event->keyval == GDK_KEY_KP_6) ||
      (event->keyval == GDK_KEY_KP_7) ||
      (event->keyval == GDK_KEY_KP_8) ||
      (event->keyval == GDK_KEY_KP_9) ||
      (event->keyval == GDK_KEY_KP_Decimal) ||
      (event->keyval == GDK_KEY_KP_Enter) ||
      (event->keyval == GDK_KEY_BackSpace)) {
    process_keys(widget,event);
  }*/
  if ((event->keyval == GDK_KEY_Left) ||
      (event->keyval == GDK_KEY_KP_Left)) {
    set_focus(app1,"LeftBtn");
  }
  if ((event->keyval == GDK_KEY_Right) ||
      (event->keyval == GDK_KEY_KP_Right)) {
    set_focus(app1,"RightBtn");
  }

}

//CONTROL PANEL CALLBACKS
extern void set_af(GtkAdjustment* adj,gpointer gptr);
extern void set_rf(GtkAdjustment* adj,gpointer gptr);
extern void set_if(GtkAdjustment* adj,gpointer gptr);
extern void set_pi(GtkAdjustment* adj,gpointer gptr);
extern void set_po(GtkAdjustment* adj,gpointer gptr);
extern void set_cp(GtkAdjustment* adj,gpointer gptr);
extern void set_sq(GtkAdjustment* adj,gpointer gptr);
extern void set_no(GtkAdjustment* adj,gpointer gptr);
extern void set_nr(GtkAdjustment* adj,gpointer gptr);
extern void set_ap(GtkAdjustment* adj,gpointer gptr);
/* LATER when tone and bandwidth controls are available in hamlib
extern void set_to(GtkAdjustment* adj,gpointer gptr);
extern void set_bwc(GtkAdjustment* adj,gpointer gptr);*/

//OPTIONS CALLBACKS
void
on_PortBoxEdit_changed                 (GtkEditable*	editable,
                                        gpointer	user_data)
{
  gchar* list_str = gtk_editable_get_chars(editable,0,-1);
  gboolean show_item = (strcmp(list_str,"Other...") == 0);
  set_enabled(opt1,"PortEdit",show_item);
}


void
on_OkBtn_clicked                       (GtkButton*       button,
                                        gpointer         user_data)
{ 
  gboolean	oldBCB;
   
  hamlib_riglist_set_defaults();
  oldBCB = g_settings_get_boolean(sgc_settings,"include-bcb");
  doIncludeBCB = gtk_toggle_button_get_active((GtkToggleButton*)lookup_widget(opt1,"doAMBroadcastBand"));
  g_settings_set_boolean(sgc_settings,"include-bcb",doIncludeBCB);
  auto_update = gtk_toggle_button_get_active((GtkToggleButton*)lookup_widget(opt1,"autoUpdateCheck"));
  g_settings_set_boolean(sgc_settings,"autoupdate",auto_update);
  gtk_window_close (GTK_WINDOW(opt1));
  if (oldBCB != doIncludeBCB)
    if (tun1)
      doRefresh();
 }


void
on_ApplyBtn_clicked                    (GtkButton*       button,
                                        gpointer         user_data)
{
  gboolean	oldBCB;

  hamlib_riglist_set_defaults();
  oldBCB = g_settings_get_boolean(sgc_settings,"include-bcb");
  doIncludeBCB = gtk_toggle_button_get_active((GtkToggleButton*)lookup_widget(opt1,"doAMBroadcastBand"));
  g_settings_set_boolean(sgc_settings,"include-bcb",doIncludeBCB);
  auto_update = gtk_toggle_button_get_active((GtkToggleButton*)lookup_widget(opt1,"autoUpdateCheck"));
  g_settings_set_boolean(sgc_settings,"autoupdate",auto_update);
  if (oldBCB != doIncludeBCB)
    if (tun1)
      doRefresh();
}


void
on_CancelBtn_clicked                   (GtkButton*       button,
                                        gpointer         user_data)
{
  gtk_window_close (GTK_WINDOW(opt1));
}


//MEMORY CALLBACKS
void
on_MemDownBtn_clicked			(GtkButton*	button,
                                         gpointer	user_data)
{
  mem_up_down(-1.0);
}


void
on_MemUpBtn_clicked			(GtkButton*	button,
                                         gpointer	user_data)
{
  mem_up_down(1.0);
}


void
on_new4_activate                       (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  get_mem_file(0);
}


void
on_MemNewBtn_clicked                   (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  get_mem_file(0);
}


void
on_open4_activate                      (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  get_mem_file(1);
}


void
on_MemOpenBtn_clicked                  (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  get_mem_file(1);
}


void
on_save3_activate                      (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  save_memories_to_file(NULL);
}


void
on_MemSaveBtn_clicked                   (GtkButton*	button,
                                         gpointer	user_data)
{
  save_memories_to_file(NULL);
}


void
on_save_as4_activate                   (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  get_mem_file(2);
}


void
on_MemSaveAsBtn_clicked                (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  get_mem_file(2);
}


void
on_capture2_activate                   (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  get_memory_list();
}


void
on_fill_radio1_activate	               (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  fill_all_memory_channels_from_list();
}


void
on_CloseBtn_clicked			(GtkButton*	button,
                                         gpointer	user_data)
{
  gtk_widget_set_visible(mem1, false);  
}


// TUNING DATABASE CALLBACKS 
void
on_open_database2_activate             (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  get_database_file();
}


void
on_OpenDBBtn_clicked                   (GtkButton*	button,
                                        gpointer	user_data)
{
  get_database_file();
}


void
on_update_database2_activate           (GtkMenuItem*	menuitem,
                                        gpointer	user_data)
{
  checkWebPage(false);
}


void
on_UpdateDBBtn_clicked                   (GtkButton*	button,
                                        gpointer	user_data)
{
  checkWebPage(false);
}


static gboolean
on_completion_match_selected		(GtkEntryCompletion*	completion,
                                         GtkTreeModel*		model,
                                         GtkTreeIter		iter,
                                         gpointer		uder_data)
{
  gtk_entry_completion_complete(completion);
  return true;
}


void
on_SearchEntry_activate			(GtkWidget*	sEntry,
                                         gpointer	data)
{
  doInputTest(sEntry);
}


void
on_unfiltered1_activate			(GtkMenuItem*	menuitem,
                                         gpointer	user_data)
{
  gtk_combo_box_set_active((GtkComboBox*)lookup_widget(tun1, "FilterData"), 0);      
}


void
on_languagefilt1_activate		(GtkMenuItem*	menuitem,
                                         gpointer	user_data)
{
  gtk_combo_box_set_active((GtkComboBox*)lookup_widget(tun1, "FilterData"), 1);      
}


void
on_countryfilt1_activate		(GtkMenuItem*	menuitem,
                                         gpointer	user_data)
{
  gtk_combo_box_set_active((GtkComboBox*)lookup_widget(tun1, "FilterData"), 2);      
}


void
on_regionfilt1_activate			(GtkMenuItem*	menuitem,
                                         gpointer	user_data)
{
  gtk_combo_box_set_active((GtkComboBox*)lookup_widget(tun1, "FilterData"), 3);      
}


void
on_targetfilt1_activate			(GtkMenuItem*	menuitem,
                                         gpointer	user_data)
{
  gtk_combo_box_set_active((GtkComboBox*)lookup_widget(tun1, "FilterData"), 4);      
}


void
on_FilterData_changed			(GtkWidget*	opt_menu,
                                         gpointer	data)
{
  GtkWidget*	gen_item;
  gint		opt_id;
  
  opt_id = gtk_combo_box_get_active((GtkComboBox*)opt_menu); 
  gen_item = lookup_widget(tun1,"FilterDataBox");
  if (gen_item)
    set_filter_data_box(gen_item, opt_id);
  g_signal_handlers_block_by_func(lookup_widget(tun1,"unfiltered1"), G_CALLBACK(on_unfiltered1_activate), NULL);
  g_signal_handlers_block_by_func(lookup_widget(tun1,"languagefilt1"), G_CALLBACK(on_languagefilt1_activate), NULL);
  g_signal_handlers_block_by_func(lookup_widget(tun1,"countryfilt1"), G_CALLBACK(on_countryfilt1_activate), NULL);
  g_signal_handlers_block_by_func(lookup_widget(tun1,"regionfilt1"), G_CALLBACK(on_regionfilt1_activate), NULL);
  g_signal_handlers_block_by_func(lookup_widget(tun1,"targetfilt1"), G_CALLBACK(on_targetfilt1_activate), NULL);
  set_filter_menu_items(opt_id);
  g_signal_handlers_unblock_by_func(lookup_widget(tun1,"unfiltered1"), G_CALLBACK(on_unfiltered1_activate), NULL);
  g_signal_handlers_unblock_by_func(lookup_widget(tun1,"languagefilt1"), G_CALLBACK(on_languagefilt1_activate), NULL);
  g_signal_handlers_unblock_by_func(lookup_widget(tun1,"countryfilt1"), G_CALLBACK(on_countryfilt1_activate), NULL);
  g_signal_handlers_unblock_by_func(lookup_widget(tun1,"regionfilt1"), G_CALLBACK(on_regionfilt1_activate), NULL);
  g_signal_handlers_unblock_by_func(lookup_widget(tun1,"targetfilt1"), G_CALLBACK(on_targetfilt1_activate), NULL);
}


void
on_FilterDataBox_changed		(GtkWidget*	opt_menu,
                                         gpointer	data)
{
  GtkWidget*	gen_item;
  gint		opt_id;
  
  
  gen_item = lookup_widget(tun1,"FilterData");
  if (gen_item) {
    opt_id = gtk_combo_box_get_active((GtkComboBox*)gen_item); 
    get_filter_data_box(opt_menu, opt_id);
  }
}


void
on_StationsListSelection_changed	(GtkWidget*	selection,
                                         gpointer	data)
{
  get_station_data(selection, true);
}


void
on_FreqListSelection_changed		(GtkWidget*	selection,
                                         gpointer	data)
{
  get_station_data(selection, false);
}


void
on_StatListSelection_changed		(GtkWidget*	selection,
                                         gpointer	data)
{
  get_frequency_data(selection);
}


void
on_refresh1_activate			(GtkMenuItem*	menuitem,
                                         gpointer	user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(tun1,"StationsList"), G_CALLBACK(on_StationsListSelection_changed), NULL);
  g_signal_handlers_block_by_func(lookup_widget(tun1,"StatList"), G_CALLBACK(on_StatListSelection_changed), NULL);
  g_signal_handlers_block_by_func(lookup_widget(tun1,"FreqList"), G_CALLBACK(on_FreqListSelection_changed), NULL);
  doRefresh();
  g_signal_handlers_unblock_by_func(lookup_widget(tun1,"StationsList"), G_CALLBACK(on_StationsListSelection_changed), NULL);
  g_signal_handlers_unblock_by_func(lookup_widget(tun1,"StatList"), G_CALLBACK(on_StatListSelection_changed), NULL);
  g_signal_handlers_unblock_by_func(lookup_widget(tun1,"FreqList"), G_CALLBACK(on_FreqListSelection_changed), NULL);  
}


void
on_RefreshBtn_clicked			(GtkButton*	button,
                                         gpointer	user_data)
{
  g_signal_handlers_block_by_func(lookup_widget(tun1,"StationsList"), G_CALLBACK(on_StationsListSelection_changed), NULL);
  g_signal_handlers_block_by_func(lookup_widget(tun1,"StatList"), G_CALLBACK(on_StatListSelection_changed), NULL);
  g_signal_handlers_block_by_func(lookup_widget(tun1,"FreqList"), G_CALLBACK(on_FreqListSelection_changed), NULL);
  doRefresh();
  g_signal_handlers_unblock_by_func(lookup_widget(tun1,"StationsList"), G_CALLBACK(on_StationsListSelection_changed), NULL);
  g_signal_handlers_unblock_by_func(lookup_widget(tun1,"StatList"), G_CALLBACK(on_StatListSelection_changed), NULL);
  g_signal_handlers_unblock_by_func(lookup_widget(tun1,"FreqList"), G_CALLBACK(on_FreqListSelection_changed), NULL);  
}


void
on_freqtimefilt1_toggled		(GtkMenuItem*	menuitem,
                                         gpointer	user_data)
{
  gboolean	freqFilter;

  freqFilter = get_checked(tun1, "freqtimefilt1");
  set_down(tun1,"FreqTimeFilterBtn", freqFilter);
}


void
on_stattimefilt1_toggled		(GtkMenuItem*	menuitem,
                                         gpointer	user_data)
{
  gboolean	statFilter;

  statFilter = get_checked(tun1, "stattimefilt1");
  set_down(tun1,"StatTimeFilterBtn", statFilter);
}


void
on_FreqTimeFilterBtn_toggled		(GtkButton*	button,
                                         gpointer	user_data)
{
  gboolean	freqFilter;
  
  g_signal_handlers_block_by_func(lookup_widget(tun1,"FreqList"), G_CALLBACK(on_FreqListSelection_changed), NULL);
  refreshList("FreqList");
  g_signal_handlers_unblock_by_func(lookup_widget(tun1,"FreqList"), G_CALLBACK(on_FreqListSelection_changed), NULL);
  
  g_signal_handlers_block_by_func(lookup_widget(tun1,"freqtimefilt1"), G_CALLBACK(on_freqtimefilt1_toggled), NULL);
  freqFilter = get_down(tun1,"FreqTimeFilterBtn");
  set_checked(tun1, "freqtimefilt1", freqFilter);
  g_signal_handlers_unblock_by_func(lookup_widget(tun1,"freqtimefilt1"), G_CALLBACK(on_freqtimefilt1_toggled), NULL);
}


void
on_StatTimeFilterBtn_toggled		(GtkButton*	button,
                                         gpointer	user_data)
{
  gboolean	statFilter;

  g_signal_handlers_block_by_func(lookup_widget(tun1,"StatList"), G_CALLBACK(on_StatListSelection_changed), NULL);
  refreshList("StatList");
  g_signal_handlers_unblock_by_func(lookup_widget(tun1,"StatList"), G_CALLBACK(on_StatListSelection_changed), NULL);

  g_signal_handlers_block_by_func(lookup_widget(tun1,"stattimefilt1"), G_CALLBACK(on_stattimefilt1_toggled), NULL);
  statFilter = get_down(tun1,"StatTimeFilterBtn");
  set_checked(tun1, "stattimefilt1", statFilter);  
  g_signal_handlers_unblock_by_func(lookup_widget(tun1,"stattimefilt1"), G_CALLBACK(on_stattimefilt1_toggled), NULL);
}


void
on_DBCloseBtn_clicked			(GtkButton*	button,
                                         gpointer	user_data)
{
  gtk_widget_set_visible(tun1, false); 
}


// LOG WINDOW CALLBACKS
void
on_LogListSelection_changed		(GtkWidget*	selection,
                                         gpointer	data)
{
  get_LogList_data(selection);
}


void
on_log_new_activate			(GtkMenuItem*	menuitem,
                                         gpointer	user_data)
{
  get_log_file(0);
}


void
on_LogNewBtn_clicked			(GtkButton*	button,
                                         gpointer	user_data)
{
  get_log_file(0);
}


void
on_log_open_activate			(GtkMenuItem*	menuitem,
                                         gpointer	user_data)
{
  get_log_file(1);
}


void
on_LogOpenBtn_clicked			(GtkButton*	button,
                                         gpointer	user_data)
{
  get_log_file(1);
}


void
on_log_save_activate			(GtkMenuItem*	menuitem,
                                         gpointer	user_data)
{
  get_log_file(2);
}


void
on_LogSaveBtn_clicked			(GtkButton*	button,
                                         gpointer	user_data)
{
  get_log_file(2);
}


void
on_LogClearBtn_clicked			(GtkButton*	button,
                                         gpointer	user_data)
{
  clear_log_entry();
}

void
on_log_add_entry_activate		(GtkMenuItem*	menuitem,
                                         gpointer	user_data)
{
  modify_log_entry(0);
}


void
on_LogAddBtn_clicked			(GtkButton*	button,
                                         gpointer	user_data)
{
  modify_log_entry(0);
}

void
on_log_modify_activate			(GtkMenuItem*	menuitem,
                                         gpointer	user_data)
{
  modify_log_entry(1);
}

void
on_LogUpdateBtn_clicked			(GtkButton*	button,
                                         gpointer	user_data)
{
  modify_log_entry(1);
}

void
on_log_delete_activate			(GtkMenuItem*	menuitem,
                                         gpointer	user_data)
{
  modify_log_entry(2);
}

void
on_LogDeleteBtn_clicked			(GtkButton*	button,
                                         gpointer	user_data)
{
  modify_log_entry(2);
}

void
on_LogCloseBtn_clicked			(GtkButton*	button,
                                         gpointer	user_data)
{
  gtk_widget_set_visible(log1, false);
}


//ENTRY INTERFACE
void
on_EntryOkBtn_clicked			(GtkButton*	button,
                                         gpointer	user_data)
{
  gtk_dialog_response((GtkDialog*)entry_dlg, GTK_RESPONSE_OK);
}


void
on_EntryCancelBtn_clicked		(GtkButton*	button,
                                         gpointer	user_data)
{
  gtk_dialog_response((GtkDialog*)entry_dlg, GTK_RESPONSE_CANCEL);
}


//UNIVERSAL HOOK TO PREVENT DIALOGS FROM CLOSING ON ESC
gboolean
on_widget_key_press_event		(GtkWidget*	widget,
                                         GdkEventKey*	event,
                                         gpointer	user_data)
{
  gboolean	retval;
  
  retval = false;
  if (event->keyval == GDK_KEY_Escape) {
    retval = true;
    set_power();
  }
  /*if (widget == mem1) {
    if ((event->keyval == GDK_KEY_W) && (event->state == GDK_CONTROL_MASK)) {
      retval = true;
      on_capture1_activate(NULL, NULL);
    }
    if ((event->keyval == GDK_KEY_Y) && (event->state == GDK_CONTROL_MASK)) {
      retval = true;
      on_fill1_activate(NULL, NULL);
    }
  }
  if (widget == log1) {
    if ((event->keyval == GDK_KEY_F4) && (event->state == GDK_CONTROL_MASK)) {
      retval = true;
      on_log_open_activate(NULL, NULL);
    }
    if ((event->keyval == GDK_KEY_L) && (event->state == GDK_CONTROL_MASK)) {
      retval = true;
      on_log_add_entry_activate(NULL, NULL);
    }
    if (event->keyval == GDK_KEY_KP_Insert) {
      retval = true;
      on_log_modify_activate(NULL, NULL);
    }
    if (event->keyval == GDK_KEY_KP_Delete) {
      retval = true;
      on_log_delete_activate(NULL, NULL);
    }
  }*/
  return retval;
}


//MAIN INTERFACE FOR BUILDER
GtkWidget*
build_user_interface			(gchar*	UI_name,
					 gchar*	UI_file)
{
  GtkWidget*	outWidget = nullptr;
  gchar*	gladefile;
  GError*	error = nullptr;
  
  if (!builder)
    builder = gtk_builder_new();
  gladefile = g_strconcat(PACKAGE_DATA_DIR"/",UI_file,NULL);
  
  if (gtk_builder_add_from_file (builder, gladefile, &error)) {
    outWidget = GTK_WIDGET(gtk_builder_get_object(builder, UI_name));
  }
  
  if (error != NULL) {
    g_print("%s",error->message);
    g_clear_error(&error);
  }
  
  return outWidget;
}


//MAIN APPLUCATION INTERFACE
GtkWidget*
create_app1 (void)
{
  GtkWidget*	mainapp1;
  
  mainapp1 = build_user_interface("app1","SGControl.glade");
  
  l_popup = lookup_widget(mainapp1,"LtlTuneMenu");
  gtk_menu_attach_to_widget(GTK_MENU(l_popup),lookup_widget(mainapp1,"LLeftBtn"), NULL);
  
  b_popup = lookup_widget(mainapp1,"BigTuneMenu");
  gtk_menu_attach_to_widget(GTK_MENU(b_popup),lookup_widget(mainapp1,"LeftBtn"), NULL);
    
  /*File Menu*/
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"open_database1")), "activate",
                      G_CALLBACK (on_open_database1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"update_database1")), "activate",
                      G_CALLBACK (on_update_database1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"exit1")), "activate",
                      G_CALLBACK (on_exit1_activate), NULL);
                      
  /*Edit Menu*/                    
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"mark1")), "activate",
                      G_CALLBACK (on_mark1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"delete1")), "activate",
                      G_CALLBACK (on_delete1_activate), NULL);
                      
  /*View Menu*/
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"keypad1")), "activate",
                      G_CALLBACK (on_keypad1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"control_panel1")), "activate",
                      G_CALLBACK (on_control_panel1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"history1")), "activate",
                      G_CALLBACK (on_history1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"band_plan1")), "activate",
                      G_CALLBACK (on_band_plan1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"tuning_database1")), "activate",
                      G_CALLBACK (on_tuning_database1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"update_display1")), "activate",
                      G_CALLBACK (on_update_display1_activate), NULL);
                                           
  /*Radio Menu*/                    
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"power1")), "activate",
                      G_CALLBACK (on_power1_activate), NULL);                      
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"noise_blanker1")), "activate",
                      G_CALLBACK (on_noise_blanker1_activate), NULL);                      
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"noise_blanker2")), "activate",
                      G_CALLBACK (on_noise_blanker2_activate), NULL);                      
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"noise_reduction1")), "activate",
                      G_CALLBACK (on_noise_reduction1_activate), NULL);                      
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"attenuator1")), "activate",
                      G_CALLBACK (on_attenuator1_activate), NULL);                      
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"preamp1")), "activate",
                      G_CALLBACK (on_preamp1_activate), NULL);                      
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"notch1")), "activate",
                      G_CALLBACK (on_notch1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"auto_notch1")), "activate",
                      G_CALLBACK (on_auto_notch1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"tape_vox1")), "activate",
                      G_CALLBACK (on_tape_vox1_activate), NULL);

  /*Tuning Menu*/
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"memory_mode1")), "activate",
                      G_CALLBACK (on_memory_mode1_activate), NULL);

  /*Memory Menu*/
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"new1")), "activate",
                      G_CALLBACK (on_new1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"open1")), "activate",
                      G_CALLBACK (on_open1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"save1")), "activate",
                      G_CALLBACK (on_save1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"save_as1")), "activate",
                      G_CALLBACK (on_save_as1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"capture1")), "activate",
                      G_CALLBACK (on_capture1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"show_list1")), "activate",
                      G_CALLBACK (on_show_list1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"fill_radio1")), "activate",
                      G_CALLBACK (on_fill_radio1_activate), NULL);

  /*Log Menu*/
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"show_log1")), "activate",
                      G_CALLBACK (on_show_log1_activate), NULL);

  /*Tools Menu*/                                          
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"options1")), "activate",
                      G_CALLBACK (on_options1_activate), NULL);                      
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"about1")), "activate",
                      G_CALLBACK (on_about1_activate), NULL);

  /*Radio Toolbar*/                    
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"HistBackBtn")), "clicked",
                      G_CALLBACK (on_HistBackBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"HistForwBtn")), "clicked",
                      G_CALLBACK (on_HistForwBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"NoiseBtn")), "clicked",
                      G_CALLBACK (on_togglebutton1_released), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"NoiseBtn2")), "clicked",
                      G_CALLBACK (on_NoiseBtn2_released), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"DNRBtn")), "clicked",
                      G_CALLBACK (on_DNRBtn_released), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"AttBtn")), "clicked",
                      G_CALLBACK (on_attbtn_released), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"PreampBtn")), "clicked",
                      G_CALLBACK (on_preampbtn_released), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"NotchBtn")), "clicked",
                      G_CALLBACK (on_togglebutton2_released), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"ANFBtn")), "clicked",
                      G_CALLBACK (on_ANFBtn_released), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"TapeBtn")), "clicked",
                      G_CALLBACK (on_togglebutton3_released), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"MemBtn")), "clicked",
                      G_CALLBACK (on_togglebutton4_released), NULL);                   
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"ShowMemBtn")), "clicked",
                      G_CALLBACK (on_ShowMemBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"ShowLogBtn")), "clicked",
                      G_CALLBACK (on_ShowLogBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"RefreshBtn2")), "clicked",
                      G_CALLBACK (on_RefreshBtn2_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"OptionsBtn")), "clicked",
                      G_CALLBACK (on_OptionsBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"ExitBtn")), "clicked",
                      G_CALLBACK (on_ExitBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"AboutBtn")), "clicked",
                      G_CALLBACK (on_AboutBtn_clicked), NULL);
                      
  /*Database Toolbar*/
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"OpenBtn")), "clicked",
                      G_CALLBACK (on_OpenBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"UpdateBtn")), "clicked",
                      G_CALLBACK (on_UpdateBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"DBViewBtn")), "clicked",
                      G_CALLBACK (on_DBViewBtn_clicked), NULL);

  /*Radio Controls*/
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"PwrBtn")), "clicked",
                      G_CALLBACK (on_PwrBtn_clicked), NULL);          
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"LLeftBtn")), "button_press_event",
                      G_CALLBACK (on_LLeftBtn_button_press_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"LeftBtn")), "button_press_event",
                      G_CALLBACK (on_LeftBtn_button_press_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"RightBtn")), "button_press_event",
                      G_CALLBACK (on_RightBtn_button_press_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"RRightBtn")), "button_press_event",
                      G_CALLBACK (on_RRightBtn_button_press_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"LLeftBtn")), "enter_notify_event",
                      G_CALLBACK (on_LLeftBtn_enter_notify_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"LeftBtn")), "enter_notify_event",
                      G_CALLBACK (on_LeftBtn_enter_notify_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"RightBtn")), "enter_notify_event",
                      G_CALLBACK (on_RightBtn_enter_notify_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"RRightBtn")), "enter_notify_event",
                      G_CALLBACK (on_RRightBtn_enter_notify_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"LLeftBtn")), "leave_notify_event",
                      G_CALLBACK (on_LLeftBtn_leave_notify_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"LeftBtn")), "leave_notify_event",
                      G_CALLBACK (on_LeftBtn_leave_notify_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"RightBtn")), "leave_notify_event",
                      G_CALLBACK (on_RightBtn_leave_notify_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"RRightBtn")), "leave_notify_event",
                      G_CALLBACK (on_RRightBtn_leave_notify_event), NULL);

  /*Big Tune Popup Menu*/
  g_signal_connect (G_OBJECT (lookup_widget(b_popup,"step010")), "activate",
                      G_CALLBACK (on_step010_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(b_popup,"step050")), "activate",
                      G_CALLBACK (on_step050_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(b_popup,"step1")), "activate",
                      G_CALLBACK (on_step1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(b_popup,"step2")), "activate",
                      G_CALLBACK (on_step2_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(b_popup,"step3")), "activate",
                      G_CALLBACK (on_step3_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(b_popup,"step5")), "activate",
                      G_CALLBACK (on_step5_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(b_popup,"step9")), "activate",
                      G_CALLBACK (on_step9_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(b_popup,"step10")), "activate",
                      G_CALLBACK (on_step10_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(b_popup,"step100")), "activate",
                      G_CALLBACK (on_step100_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(b_popup,"step1000")), "activate",
                      G_CALLBACK (on_step1000_activate), NULL);
  
  /*Small Tune Popup Menu*/
  g_signal_connect (G_OBJECT (lookup_widget(l_popup,"lstep001")), "activate",
                      G_CALLBACK (on_lstep001_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(l_popup,"lstep002")), "activate",
                      G_CALLBACK (on_lstep002_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(l_popup,"lstep003")), "activate",
                      G_CALLBACK (on_lstep003_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(l_popup,"lstep005")), "activate",
                      G_CALLBACK (on_lstep005_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(l_popup,"lstep010")), "activate",
                      G_CALLBACK (on_lstep010_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(l_popup,"lstep020")), "activate",
                      G_CALLBACK (on_lstep020_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(l_popup,"lstep030")), "activate",
                      G_CALLBACK (on_lstep030_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(l_popup,"lstep050")), "activate",
                      G_CALLBACK (on_lstep050_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(l_popup,"lstep1")), "activate",
                      G_CALLBACK (on_lstep1_activate), NULL);
  
  /*Keypad*/
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"kp0")), "clicked",
                      G_CALLBACK (on_kp0_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"kp1")), "clicked",
                      G_CALLBACK (on_kp1_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"kp2")), "clicked",
                      G_CALLBACK (on_kp2_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"kp3")), "clicked",
                      G_CALLBACK (on_kp3_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"kp4")), "clicked",
                      G_CALLBACK (on_kp4_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"kp5")), "clicked",
                      G_CALLBACK (on_kp5_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"kp6")), "clicked",
                      G_CALLBACK (on_kp6_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"kp7")), "clicked",
                      G_CALLBACK (on_kp7_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"kp8")), "clicked",
                      G_CALLBACK (on_kp8_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"kp9")), "clicked",
                      G_CALLBACK (on_kp9_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"kpBS")), "clicked",
                      G_CALLBACK (on_kpBS_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"kpDot")), "clicked",
                      G_CALLBACK (on_kpDot_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"kpEnter")), "clicked",
                      G_CALLBACK (on_kpEnter_clicked), NULL);
                                            
  g_signal_connect (G_OBJECT (mainapp1), "destroy",
                      G_CALLBACK (on_app1_destroy), NULL);

  /*Display*/
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"eventbox1")), "enter_notify_event",
                      G_CALLBACK (on_eventbox1_enter_notify_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"eventbox1")), "leave_notify_event",
                      G_CALLBACK (on_eventbox1_leave_notify_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"eventbox1")), "button_press_event",
                      G_CALLBACK (on_eventbox1_button_press_event), NULL);
                      
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"eventbox2")), "enter_notify_event",
                      G_CALLBACK (on_eventbox2_enter_notify_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"eventbox2")), "leave_notify_event",
                      G_CALLBACK (on_eventbox2_leave_notify_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"eventbox2")), "button_press_event",
                      G_CALLBACK (on_eventbox2_button_press_event), NULL);
                      
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"eventbox3")), "enter_notify_event",
                      G_CALLBACK (on_eventbox3_enter_notify_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"eventbox3")), "leave_notify_event",
                      G_CALLBACK (on_eventbox3_leave_notify_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"eventbox3")), "button_press_event",
                      G_CALLBACK (on_eventbox3_button_press_event), NULL);

  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"clist1")), "enter_notify_event",
                      G_CALLBACK (on_clist1_enter_notify_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"clist1")), "leave_notify_event",
                      G_CALLBACK (on_clist1_leave_notify_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"clist1")), "button_press_event",
                    G_CALLBACK (on_clist1_button_press_event), NULL);

  /*Aux Control Panel*/
  g_signal_connect(G_OBJECT (gtk_range_get_adjustment (GTK_RANGE (lookup_widget(app1,"AFScale")))),
                   "value-changed", G_CALLBACK (set_af), NULL);
  g_signal_connect(G_OBJECT (gtk_range_get_adjustment (GTK_RANGE (lookup_widget(app1,"RFScale")))),
                   "value-changed", G_CALLBACK (set_rf), NULL);
  g_signal_connect(G_OBJECT (gtk_range_get_adjustment (GTK_RANGE (lookup_widget(app1,"IFScale")))),
                   "value-changed", G_CALLBACK (set_if), NULL);
  g_signal_connect(G_OBJECT (gtk_range_get_adjustment (GTK_RANGE (lookup_widget(app1,"PIScale")))),
                   "value-changed", G_CALLBACK (set_pi), NULL);
  g_signal_connect(G_OBJECT (gtk_range_get_adjustment (GTK_RANGE (lookup_widget(app1,"POScale")))),
                   "value-changed", G_CALLBACK (set_po), NULL);
  g_signal_connect(G_OBJECT (gtk_range_get_adjustment (GTK_RANGE (lookup_widget(app1,"CPScale")))),
                   "value-changed", G_CALLBACK (set_cp), NULL);
  g_signal_connect(G_OBJECT (gtk_range_get_adjustment (GTK_RANGE (lookup_widget(app1,"SQScale")))),
                   "value-changed", G_CALLBACK (set_sq), NULL);
  g_signal_connect(G_OBJECT (gtk_range_get_adjustment (GTK_RANGE (lookup_widget(app1,"NOScale")))),
                   "value-changed", G_CALLBACK (set_no), NULL);
  g_signal_connect(G_OBJECT (gtk_range_get_adjustment (GTK_RANGE (lookup_widget(app1,"NRScale")))),
                   "value-changed", G_CALLBACK (set_nr), NULL);
  g_signal_connect(G_OBJECT (gtk_range_get_adjustment (GTK_RANGE (lookup_widget(app1,"APScale")))),
                   "value-changed", G_CALLBACK (set_ap), NULL);
/*g_signal_connect(G_OBJECT (gtk_range_get_adjustment (GTK_RANGE (lookup_widget(app1,"TOScale")))),
                   "value-changed", G_CALLBACK (set_to), NULL);
  g_signal_connect(G_OBJECT (gtk_range_get_adjustment (GTK_RANGE (lookup_widget(app1,"BWCScale")))),
                   "value-changed", G_CALLBACK (set_bwc), NULL);*/
 
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"button4")), "clicked",
                      G_CALLBACK (on_button4_clicked), NULL);

  /*History Panel*/
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"HistList")), "button_press_event",
                      G_CALLBACK (on_HistList_button_press_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"button5")), "clicked",
                      G_CALLBACK (on_button5_clicked), NULL);

  /*Band PLan Panel*/
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"BCList")), "button_press_event",
                      G_CALLBACK (on_BCBandList_button_press_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"UteList")), "button_press_event",
                      G_CALLBACK (on_UteBandList_button_press_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(mainapp1,"button6")), "clicked",
                      G_CALLBACK (on_button6_clicked), NULL);

  g_signal_connect (G_OBJECT (mainapp1), "key_press_event",
                      G_CALLBACK (on_app1_key_press_event), NULL);

  g_signal_connect_after (G_OBJECT (mainapp1), "key_release_event",
                      G_CALLBACK (on_app1_key_press_event_after), NULL);

/* LATER - UNIMPLEMENTED
  gtk_signal_connect (GTK_OBJECT (ScanBtn), "released",
                      GTK_SIGNAL_FUNC (on_togglebutton5_released),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (TimerBtn), "released",
                      GTK_SIGNAL_FUNC (on_togglebutton6_released),
                      NULL);*/

 return mainapp1;
}


GtkWidget*
create_OptionsDlg (void)
{
  GtkWidget*	OptionsDlg;
  GtkWidget*	RadioList;
  GSList*	rig_list;

  OptionsDlg = build_user_interface("OptionsDlg","OptionBox.glade");
  
  RadioList = lookup_widget(OptionsDlg,"RadioList");
  if (RadioList) {
    rig_list = get_hamlib_radio_list();
    hamlib_fill_radio_list(rig_list, GTK_TREE_VIEW(RadioList));
  }
  set_text(OptionsDlg,"PortBoxEdit",g_settings_get_string(sgc_settings,"rig-port"));
  set_text(OptionsDlg,"PortEdit",g_settings_get_string(sgc_settings,"port-text"));
  
  if (cur_rig_model != 0)
    hamlib_riglist_select_id(cur_rig_model);

  doIncludeBCB = g_settings_get_boolean(sgc_settings,"include-bcb");
  gtk_toggle_button_set_active((GtkToggleButton*)lookup_widget(OptionsDlg,"doAMBroadcastBand"), doIncludeBCB);
  auto_update = g_settings_get_boolean(sgc_settings,"autoupdate");
  gtk_toggle_button_set_active((GtkToggleButton*)lookup_widget(OptionsDlg,"autoUpdateCheck"), auto_update);

  g_signal_connect (G_OBJECT (lookup_widget(OptionsDlg,"PortBoxEdit")), "changed",
                      G_CALLBACK (on_PortBoxEdit_changed), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(OptionsDlg,"OkBtn")), "clicked",
                      G_CALLBACK (on_OkBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(OptionsDlg,"ApplyBtn")), "clicked",
                      G_CALLBACK (on_ApplyBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(OptionsDlg,"CancelBtn")), "clicked",
                      G_CALLBACK (on_CancelBtn_clicked), NULL);

  gtk_widget_grab_focus (lookup_widget(OptionsDlg,"OkBtn"));
  gtk_widget_grab_default (lookup_widget(OptionsDlg,"OkBtn"));

  return OptionsDlg;
}


GtkWidget*
create_AboutDlg (void)
{
  const gchar *authors[] = {
    "Authors:",
    "Mark J. Fine",
    "",
    "Contributors:",
    "Alexandru Csete - Provided code from Gnome-rig",
    "Michael Marcus, KG7M - Provided invaluable Drake R8 testing",
    NULL
  };
  GtkWidget*	AboutDlg;
  
  AboutDlg = gtk_about_dialog_new ();
  gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(AboutDlg), authors);
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(AboutDlg),"This program provides a graphical user interface to multiple communications receivers using the Hamlib library, and is based on Smart Control and Frequency Browser.");
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(AboutDlg), "Copyright (c) 2004, 2005, 2025  Mark J. Fine");
  gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(AboutDlg),
"Smart Gnome Control is free software: you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation, either version 2 of the License, or\n\
(at your option) any later version.\n\
\n\
Smart Gnome Control is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details.\n\
\n\
You should have received a copy of the GNU General Public License\n\
along with Smart Gnome Control.  If not, see http://www.gnu.org/licenses/.");
  gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(AboutDlg), NULL);
  gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(AboutDlg), NULL);
  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(AboutDlg),"Smart Gnome Control");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(AboutDlg), PACKAGE_VERSION);
  gtk_about_dialog_set_website (GTK_ABOUT_DIALOG(AboutDlg), PACKAGE_URL);
  
  gtk_widget_set_name (AboutDlg, "AboutDlg");
  g_object_set_data (G_OBJECT (AboutDlg), "AboutDlg", AboutDlg);
  gtk_window_set_modal (GTK_WINDOW (AboutDlg), TRUE);
  
  return AboutDlg;
}


GtkWidget*
create_MemWindow (void)
{
  GtkWidget* MemWindow;
  
  MemWindow = build_user_interface("MemWindow","MemWindow.glade");

  g_signal_connect (G_OBJECT (lookup_widget(MemWindow,"MemList")), "button_release_event",
                    G_CALLBACK (on_MemList_button_press_event), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(MemWindow,"new4")), "activate",
                    G_CALLBACK (on_new4_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(MemWindow,"open4")), "activate",
                    G_CALLBACK (on_open4_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(MemWindow,"save3")), "activate",
                    G_CALLBACK (on_save3_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(MemWindow,"save_as4")), "activate",
                    G_CALLBACK (on_save_as4_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(MemWindow,"capture2")), "activate",
                    G_CALLBACK (on_capture2_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(MemWindow,"fill1")), "activate",
                    G_CALLBACK (on_fill1_activate), NULL);
                  
  g_signal_connect (G_OBJECT (lookup_widget(MemWindow,"button18")), "clicked",
                    G_CALLBACK (on_MemNewBtn_clicked), NULL);  
  g_signal_connect (G_OBJECT (lookup_widget(MemWindow,"button19")), "clicked",
                    G_CALLBACK (on_MemOpenBtn_clicked), NULL);  
  g_signal_connect (G_OBJECT (lookup_widget(MemWindow,"button20")), "clicked",
                    G_CALLBACK (on_MemSaveBtn_clicked), NULL);  
  g_signal_connect (G_OBJECT (lookup_widget(MemWindow,"button21")), "clicked",
                    G_CALLBACK (on_MemSaveAsBtn_clicked), NULL);  
  g_signal_connect (G_OBJECT (lookup_widget(MemWindow,"button23")), "clicked",
                    G_CALLBACK (on_MemDownBtn_clicked), NULL);  
  g_signal_connect (G_OBJECT (lookup_widget(MemWindow,"button24")), "clicked",
                    G_CALLBACK (on_MemUpBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(MemWindow,"button3")), "clicked",
                    G_CALLBACK (on_CloseBtn_clicked), NULL);

  g_signal_connect (G_OBJECT (MemWindow), "key_press_event",
                    G_CALLBACK (on_widget_key_press_event), NULL);

/*  gtk_signal_connect (GTK_OBJECT (MemWindow), "show",
                      GTK_SIGNAL_FUNC (on_MemWindow_show),
                      NULL);*/

  return MemWindow;
}

GtkWidget*
create_TuningDlg (void)
{
  GtkWidget* TuningDlg;
  
  TuningDlg = build_user_interface("TuningDlg","TuningDlg.glade");
  
  gtk_tree_view_set_level_indentation((GtkTreeView*)lookup_widget(TuningDlg,"StationsList"),-18);

  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg,"entrycompletion1")), "match-selected",
                    G_CALLBACK (on_completion_match_selected), NULL);
                    
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg,"open_database2")), "activate",
                    G_CALLBACK (on_open_database2_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg,"update_database2")), "activate",
                    G_CALLBACK (on_update_database2_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg, "refresh1")), "activate",
                    G_CALLBACK (on_refresh1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg, "unfiltered1")), "activate",
                    G_CALLBACK (on_unfiltered1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg, "languagefilt1")), "activate",
                    G_CALLBACK (on_languagefilt1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg, "countryfilt1")), "activate",
                    G_CALLBACK (on_countryfilt1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg, "regionfilt1")), "activate",
                    G_CALLBACK (on_regionfilt1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg, "targetfilt1")), "activate",
                    G_CALLBACK (on_targetfilt1_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg, "freqtimefilt1")), "toggled",
                    G_CALLBACK (on_freqtimefilt1_toggled), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg, "stattimefilt1")), "toggled",
                    G_CALLBACK (on_stattimefilt1_toggled), NULL);
                    
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg,"OpenDBBtn")), "clicked",
                    G_CALLBACK (on_OpenDBBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg,"UpdateDBBtn")), "clicked",
                    G_CALLBACK (on_UpdateDBBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg, "RefreshBtn")), "clicked",
                    G_CALLBACK (on_RefreshBtn_clicked), NULL);
                    
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg,"SearchEntry")), "activate",
                    G_CALLBACK (on_SearchEntry_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg, "StationsListSelection")), "changed",
                    G_CALLBACK (on_StationsListSelection_changed), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg, "FreqListSelection")), "changed",
                    G_CALLBACK (on_FreqListSelection_changed), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg, "StatListSelection")), "changed",
                    G_CALLBACK (on_StatListSelection_changed), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg,"FilterData")), "changed",
                    G_CALLBACK (on_FilterData_changed), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg,"FilterDataBox")), "changed",
                    G_CALLBACK (on_FilterDataBox_changed), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg, "FreqTimeFilterBtn")), "toggled",
                    G_CALLBACK (on_FreqTimeFilterBtn_toggled), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg, "StatTimeFilterBtn")), "toggled",
                    G_CALLBACK (on_StatTimeFilterBtn_toggled), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(TuningDlg,"DBCloseBtn")), "clicked",
                    G_CALLBACK (on_DBCloseBtn_clicked), NULL);

  g_signal_connect (G_OBJECT (TuningDlg), "key_press_event",
                    G_CALLBACK (on_widget_key_press_event), NULL);

  return TuningDlg;
}


GtkWidget*
create_LogWindow (void)
{
  GtkWidget *LogWindow;
 
  LogWindow = build_user_interface("LogWindow","LogWindow.glade");

  g_signal_connect (G_OBJECT (lookup_widget(LogWindow,"LogListSelection")), "changed",
                      G_CALLBACK (on_LogListSelection_changed), NULL);
  /* File menu */
  g_signal_connect (G_OBJECT (lookup_widget(LogWindow,"log_new")), "activate",
                      G_CALLBACK (on_log_new_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(LogWindow,"LogNewBtn")), "clicked",
                      G_CALLBACK (on_LogNewBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(LogWindow,"log_open")), "activate",
                      G_CALLBACK (on_log_open_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(LogWindow,"LogOpenBtn")), "clicked",
                      G_CALLBACK (on_LogOpenBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(LogWindow,"log_save")), "activate",
                      G_CALLBACK (on_log_save_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(LogWindow,"LogSaveBtn")), "clicked",
                      G_CALLBACK (on_LogSaveBtn_clicked), NULL);
  /* Edit menu*/
  g_signal_connect (G_OBJECT (lookup_widget(LogWindow,"log_add_entry")), "activate",
                      G_CALLBACK (on_log_add_entry_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(LogWindow,"log_modify")), "activate",
                      G_CALLBACK (on_log_modify_activate), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(LogWindow,"log_delete")), "activate",
                      G_CALLBACK (on_log_delete_activate), NULL);
  /*Dialog buttons*/
  g_signal_connect (G_OBJECT (lookup_widget(LogWindow,"LogClearBtn")), "clicked",
                      G_CALLBACK (on_LogClearBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(LogWindow,"LogAddBtn")), "clicked",
                      G_CALLBACK (on_LogAddBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(LogWindow,"LogUpdateBtn")), "clicked",
                      G_CALLBACK (on_LogUpdateBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(LogWindow,"LogDeleteBtn")), "clicked",
                      G_CALLBACK (on_LogDeleteBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(LogWindow,"LogCloseBtn")), "clicked",
                      G_CALLBACK (on_LogCloseBtn_clicked), NULL);

  g_signal_connect (G_OBJECT (LogWindow), "key_press_event",
                    G_CALLBACK (on_widget_key_press_event), NULL);

//LATER - UNIMPLEMENTED
/*  gtk_signal_connect (GTK_OBJECT (LogExportBtn), "clicked",
                      GTK_SIGNAL_FUNC (on_LogExportBtn_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (LogPrintBtn), "clicked",
                      GTK_SIGNAL_FUNC (on_LogPrintBtn_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (LogDownBtn), "clicked",
                      GTK_SIGNAL_FUNC (on_LogDownBtn_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (LogUpBtn), "clicked",
                      GTK_SIGNAL_FUNC (on_LogUpBtn_clicked),
                      NULL);*/
  
  return LogWindow;
}


GtkWidget*
create_EntryDlg (void)
{
  GtkWidget	*EntryDlg;
  GdkRGBA	imgColor = {(double)192.0,(double)192.0,(double)192.0,(double) 1.0};

  EntryDlg = build_user_interface("EntryDlg","EntryDlg.glade");

  if (get_theme_style() == 1) {
    set_svg_color(EntryDlg, "OkImg", PACKAGE_PIXMAPS_DIR"/mark.svg", imgColor);
    set_svg_color(EntryDlg, "CancelImg", PACKAGE_PIXMAPS_DIR"/cancel.svg", imgColor);
  }

  g_signal_connect (G_OBJECT (lookup_widget(EntryDlg,"OkBtn")), "clicked",
                      G_CALLBACK (on_EntryOkBtn_clicked), NULL);
  g_signal_connect (G_OBJECT (lookup_widget(EntryDlg,"CancelBtn")), "clicked",
                      G_CALLBACK (on_EntryCancelBtn_clicked), NULL);

  return EntryDlg;
}

