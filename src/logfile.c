/*
 * Smart Gnome Control - log functions file
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
#include <stdbool.h>

#include "logfile.h"
#include "extra.h"
#include "interface.h"

extern GtkWidget*	app1;
extern GtkWidget*	log1;
extern GtkWidget*	entry_dlg;
extern gboolean		ispower;
extern double		frequency;
extern GSettings*	sgc_settings;
extern gchar*		basepath;
extern gchar*		basedbpath;
extern gchar*		baselogpath;
extern gchar*		basemempath;
extern gchar*		log_name;

const gchar* basestr = ".sgcontrol";

/* routine to create user data directory string */
gchar*
get_user_base()
{
  gchar*	pathstr;
  const gchar*	homestr;
	
  homestr = g_get_home_dir();
  pathstr = g_strdup_printf("%s/%s",homestr,basestr);
  return pathstr;
}


void
copy_file_to_base			(gchar*	fromFile)
{
  GFile*	fromPath;
  GFile*	toPath;
  GError*	err = nullptr;
  gchar*	tmpStr;
  gchar*	tmpStr2;
  gboolean	result;
  
  tmpStr = g_strdup_printf("%s/%s",PACKAGE_DATA_DIR,fromFile);
  fromPath = g_file_new_for_path(tmpStr);
  tmpStr2 = g_strdup_printf("%s/%s",basepath,fromFile);
  toPath = g_file_new_for_path(tmpStr2);
  if ((fromPath) && (toPath)) {
    result = g_file_copy(fromPath, toPath, G_FILE_COPY_NONE, NULL, NULL, NULL, &err);
    if ((result != true) || (err)) {
      g_print("error copying file %s to %s - %s\n", fromFile, basepath, err->message);
      g_error_free(err);
    }
  }
  g_free(tmpStr);
  g_free(tmpStr2);
  g_object_unref(fromPath);
  g_object_unref(toPath);
}

/* routine to see if user directory exists, if not create it and copy dbs into it */
void
prep_user_dir()
{
  GFile*	toPath;
  GError*	err = nullptr;
  gboolean	result;
	
  basepath = get_user_base();
  basedbpath = g_strdup_printf("%s/%s",basepath,"eibi-freq*.txt");
  baselogpath = g_strdup_printf("%s/%s",basepath,"*.log");
  basemempath = g_strdup_printf("%s/%s",basepath,"*.mem");
  if (!g_file_test(basepath, G_FILE_TEST_EXISTS)) {    
    toPath = g_file_new_for_path(basepath);
    result = g_file_make_directory (toPath, NULL, &err);
    if ((result != true) || (err)) {
      if (err) {
        g_print("error creating profile directory %s - %s\n", basepath, err->message);
        g_error_free(err);
      }
    } else {
      //copy all starting data files in PACKAGE_DATA_DIR to basepath
      copy_file_to_base("eibi-freq-a25-02-Jun.txt");
      copy_file_to_base("eibi-readme.txt");
      copy_file_to_base("sgcontrol.log");
      copy_file_to_base("statsTypeData.txt");
    }
  }
}


gboolean
file_exists                            (gchar*	in_file)
{
  gboolean	result;

  result = (g_file_test(in_file, G_FILE_TEST_EXISTS));
  return result;
}


// FIXME replace with void inline return of temp
gchar*
strchrins                              (gchar*	in_ins,
                                        gchar*	in_str,
                                        gint	in_pos)
{
  gchar*	temp;
  gchar*	sect1;
  gchar*	sect2;
	
  temp = in_str;
  sect1 = g_strndup(temp,in_pos);
  sect2 = g_strdup(temp+in_pos);
  g_free(temp);
  temp = g_strdup_printf("%s%s%s",sect1,in_ins,sect2);
  g_free(sect1);
  g_free(sect2);
  return temp;
}

// FIXME replace with void inline return of temp
gchar*
strchrdel                              (gchar* in_str,
                                        int    in_pos,
                                        int    in_len)
{
  gchar*	temp;
  gchar*	sect1 = NULL;
  gchar*	sect2;
	
  temp = in_str;
  if (in_pos != 0)
    sect1 = g_strndup(temp,in_pos);
  sect2 = g_strdup(temp+in_pos+in_len);
  g_free(temp);
  if (in_pos != 0)
    temp = g_strdup_printf("%s%s",sect1,sect2);
  else
    temp = g_strdup(sect2);
  if (in_pos != 0)
    g_free(sect1);
  g_free(sect2);
  return temp;
}


gint
get_log_text_length			(GtkWidget*	gen_item)
{
  GtkTextBuffer*	tBuf;
  GtkTextIter		startIter;
  GtkTextIter		endIter;
  gchar*		text;
  gint			len;

  tBuf = gtk_text_view_get_buffer((GtkTextView*)gen_item);
  gtk_text_buffer_get_bounds(tBuf, &startIter, &endIter);
  text = gtk_text_buffer_get_text(tBuf, &startIter, &endIter, true);
  len = strlen(text);
  
  return len;
}


gchar*
get_log_text				(GtkWidget*	gen_item)
{
  GtkTextBuffer*	tBuf;
  GtkTextIter		startIter;
  GtkTextIter		endIter;
  gchar*		text;

  tBuf = gtk_text_view_get_buffer((GtkTextView*)gen_item);
  gtk_text_buffer_get_bounds(tBuf, &startIter, &endIter);
  text = gtk_text_buffer_get_text(tBuf, &startIter, &endIter, true);
  
  return text;
}


void
set_log_text				(GtkWidget*	gen_item,
                                         gchar*		inText)
{
  GtkTextBuffer*	tBuf;

  tBuf = gtk_text_view_get_buffer((GtkTextView*)gen_item);
  gtk_text_buffer_set_text(tBuf, inText, strlen(inText));
}


void
clear_log_text				(GtkWidget*	gen_item)
{
  GtkTextBuffer*	tBuf;
  GtkTextIter		startIter;
  GtkTextIter		endIter;

  tBuf = gtk_text_view_get_buffer((GtkTextView*)gen_item);
  gtk_text_buffer_get_bounds(tBuf, &startIter, &endIter);
  gtk_text_buffer_delete(tBuf, &startIter, &endIter);
}


void
set_log_data                           (gchar*	in_text)
{
  GtkWidget*	gen_item;
  gint		len;

  gen_item = lookup_widget(log1,"LogText");
  len = get_log_text_length(gen_item);
  if (len > 0)
    clear_log_text(gen_item);
  if (in_text) {
    len = strlen(in_text);
    if (len > 0)
      set_log_text(gen_item, in_text);
  }
}


void
show_log_list_value			(GtkTreeModel*	model,
                                         GtkTreeIter*	iter,
                                         gint		col_id)
{
  GValue	value = G_VALUE_INIT;
  gchar*	textStr;
  gint		pos;
  

  gtk_tree_model_get_value(model, iter, col_id, &value);
  textStr = strdup(g_value_get_string(&value));
  if (textStr) {
    switch (col_id) {
      case 0 :
        set_text(log1, "LogDateEntry", textStr);
        break;
      case 1 :
        set_text(log1, "LogTimeEntry", textStr);
        break;
      case 2 :
        pos = strlen(textStr) - 3;
        if (strcmp(textStr+pos, "kHz") == 0)
          set_down(log1, "LogkHzBtn", true);
        else
          set_down(log1, "LogMHzBtn", true);
        (textStr[pos-1]) = '\0';
        set_text(log1, "LogFreqEntry", textStr);
        (textStr[pos-1]) = ' ';
        break;
      case 3 :
        set_text(log1, "LogCallEntry", textStr);
        break;
      case 4 :
        set_text(log1, "LogStatEntry", textStr);
        break;
      case 5 :
        set_text(log1, "LogCounEntry", textStr);
        break;
      case 6 :
        set_text(log1, "LogModeEntry", textStr);
        break;
      case 7 :
        set_text(log1, "LogSINPOEntry", textStr);
        break;
      case 8 :
        set_log_data(textStr);
        break;
      default :
        break;
    }
    g_free(textStr);
  }
  g_value_unset(&value);
}


void
get_LogList_data			(GtkWidget*	selection)
{
  GtkTreeModel*	model;
  GtkTreeIter	iter;

  if (selection) {
    if (gtk_tree_selection_get_selected((GtkTreeSelection*)selection, &model, &iter)) {
      show_log_list_value(model, &iter, 0);
      show_log_list_value(model, &iter, 1);
      show_log_list_value(model, &iter, 2);
      show_log_list_value(model, &iter, 3);
      show_log_list_value(model, &iter, 4);
      show_log_list_value(model, &iter, 5);
      show_log_list_value(model, &iter, 6);
      show_log_list_value(model, &iter, 7);
      show_log_list_value(model, &iter, 8);
    }
  }
}


void
set_sort_menu_items			(gint		option_id)
{
  switch (option_id) {
    case 0 :
      set_checked(log1, "log_date", true);
      break;
    case 1 :
      set_checked(log1, "log_time", true);
      break;
    case 2 :
      set_checked(log1, "log_frequency", true);
      break;
    case 3 :
      set_checked(log1, "log_call", true);
      break;
    case 4 :
      set_checked(log1, "log_station", true);
      break;
    case 5 :
      set_checked(log1, "log_country", true);
      break;
    case 6 :
      set_checked(log1, "log_mode", true);
      break;
    case 7 :
      set_checked(log1, "log_sinpo", true);
      break;
    case 8 :
      set_checked(log1, "log_original_order", true);
      break;
  }
}


void
get_log_file				(gint	file_mode)
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
  gint			result;
  
  enum {
    SGC_LOG_NEW = 0,
    SGC_LOG_OPEN,
    SGC_LOG_SAVE,
    SGC_NUM_LOG_FILE_CHOICES
  };
  
  switch (file_mode) {
    case SGC_LOG_NEW :
      title = "New Log File";
      temp = "create a new";
      break;
    case SGC_LOG_SAVE :
      title = "Save Log File As";
      temp = "save the current";
      break;
    default :
      title = "";
      temp = "";
      break;
  }
  
  prompt = g_strdup_printf("%s %s %s\n\n%s\n%s\n%s\n", "Enter a filename to", temp, "log to in the box below.",
                                                           "Please use a simple filename with no path or extension.",
                                                           "The filename can't be blank nor contain spaces or special characters.",
                                                           "And, the file can't already exist.");
                                                             
  if (file_mode == SGC_LOG_OPEN) { 
    file_selector = gtk_file_chooser_dialog_new("Open Log File",
                                                GTK_WINDOW(app1),
                                                GTK_FILE_CHOOSER_ACTION_OPEN,
                                                "_Cancel",
                                                GTK_RESPONSE_CANCEL,
                                                "_Open",
                                                GTK_RESPONSE_ACCEPT,
                                                NULL);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_selector), basepath);
    file_filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(file_filter, "*.log");
    gtk_file_filter_set_name(file_filter, "Log files (*.log)");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_selector), file_filter);
    gtk_window_set_position(GTK_WINDOW(file_selector), GTK_WIN_POS_CENTER);
    gtk_window_set_modal(GTK_WINDOW(file_selector), TRUE);
    result = gtk_dialog_run(GTK_DIALOG(file_selector));
    if (result == GTK_RESPONSE_ACCEPT) {
      new_file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_selector));
      if (log_name)
        g_free(log_name);
      log_name = g_strdup(new_file);
      open_logfile();
      g_free(new_file);
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
        show_message("Enter Log File", GTK_MESSAGE_WARNING, temp2);
        g_free(temp2);
      } else {
        new_file = g_strdup_printf("%s/%s.log", basepath, temp);
        if (file_exists(new_file)) {
          temp2 = g_strdup_printf("File should not already exist.\n%s already exists.\n",temp);
          show_message("Enter Log File", GTK_MESSAGE_WARNING, temp2);
          g_free(temp2);
        } else {
          if (file_mode == SGC_LOG_NEW) {
            //create blank new_file
            if (new_file) {     
              fp1 = fopen(new_file,"w");
              if (!fp1) {
                temp2 = g_strdup_printf("Can't create new log file.\n");
                show_message("New Log File", GTK_MESSAGE_WARNING, temp2);
                g_free(temp2);
              } else
                fclose(fp1);
            }          
          } else {
            if (file_mode == SGC_LOG_SAVE) {
              //copy log_name to new_file
              if ((log_name) && (new_file)) {
                fromPath = g_file_new_for_path(log_name);
                toPath = g_file_new_for_path(new_file);
                result = g_file_copy(fromPath, toPath, G_FILE_COPY_NONE, NULL, NULL, NULL, &err);
                if ((result != true) || (err)) {
                  g_print("error saving file %s - %s\n", new_file, err->message);
                  g_error_free(err);
                }
                g_object_unref(fromPath);
                g_object_unref(toPath);
              }
            }
          }
          //now open whatever we just created
          if (log_name)
            g_free(log_name);
          log_name = g_strdup(new_file);
          open_logfile();
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
open_logfile()
{	
  GtkWidget*	gen_item;
  GtkListStore*	store;
  GtkTreeIter	iter;
  gint		i;
  gint		j;
  gint		len;
  gint		pos;
  gint		tab_no;
  gint		tab5;
  gint		tab6;
  FILE*		fp;
  char		buf[10000];
  gchar*	tab;
  gchar*	tabtab;
  gchar*	temp;
  gchar*	temp2;
  gchar*	nul_pos;
  gchar*	log_line;
  gchar*	log_title_str;
  gchar*	log_file_name = NULL;
  gchar**	text;
	
  enum {
  DATE = 0,
  TIME,
  FREQ,
  CALL,
  STAT,
  COUN,
  MODE,
  SINPO,
  NOTES,
  NUM_COLS
  };	
	
  fp = fopen(log_name, "r");
  if (!fp) {
    temp = g_strdup_printf("%s not found.", log_name);
    show_message("Open Log File",GTK_MESSAGE_WARNING, temp);
    g_free(temp);
  } else {
    log_file_name = get_filename_from_full_path(log_name);
    log_title_str = g_strdup_printf("Log Display - [%s]", log_file_name);
    gtk_window_set_title(GTK_WINDOW(log1), log_title_str);
    g_free(log_file_name);
    g_free(log_title_str);

    gen_item = lookup_widget(log1,"LogText");
    clear_log_text(gen_item);
    
    gen_item = lookup_widget(log1,"LogList");
			
    j = 0;
    store = gtk_list_store_new( NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                                          G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, -1);
    while (!feof(fp)) {
      if (fgets(buf, 10000, fp) != NULL) {
        log_line = g_strdup(buf);
			
        // strip off crs and lfs		
        if (strlen(log_line) != 0) {
          nul_pos = strchr(log_line, 13);
          if (nul_pos) {
            temp = g_strndup(log_line, (nul_pos - (gchar*)log_line));
            g_free(log_line);
            log_line = g_strdup(temp);
            g_free(temp);
          }
          nul_pos = strchr(log_line, 10);
          if (nul_pos) {
            temp = g_strndup(log_line, (nul_pos - (gchar*)log_line));
            g_free(log_line);
            log_line = g_strdup(temp);
            g_free(temp);
          }
        }
			
        if (strlen(log_line) != 0) {
          nul_pos = strchr(log_line, 255);
          if (nul_pos)
            temp = g_strndup(log_line, (nul_pos - (gchar*)log_line));
          else
            temp = g_strdup(log_line);
					
          // count how many tabs 
          tab_no = 0;
          len = 0;
          tab5 = 0;
          tab6 = 0;
          temp2 = g_strdup(temp);
          nul_pos = strchr(temp2, 0x09);
          do {
            if (nul_pos) {
              pos = nul_pos - temp2 + 1;
              temp2 = strchrdel(temp2, 0, pos);
              len = len + pos;
              tab_no++;
              if (tab_no == 5)
                tab5 = len;
              if (tab_no == 6)
                tab6 = len;
              nul_pos = strchr(temp2, 0x09);
            }
          } while (nul_pos != NULL); 
          g_free(temp2);
          tab = g_strdup_printf("%c", 0x09);
          if (tab_no == 6) {
            temp = strchrins(tab, temp, tab5);
            temp = strchrins(tab, temp, tab6+1);
          }
					
          // kill first tab in log between freq and kHz
          for (i = 0; i < strlen(temp); i++) {
            if (temp[i] == 0x09) {
              temp[i] = 0x20;
              break;
            }
          }				
				
          // kill null segments by adding a space 
          tabtab = g_strdup_printf("%c%c", 0x09, 0x09);
          nul_pos = strstr(temp, tabtab);
          do {
            if (nul_pos) {
              pos = nul_pos - temp + 1;
              temp = strchrins(" ", temp, pos);
              nul_pos = strstr(temp, tabtab);
            }
          } while (nul_pos != NULL);
          g_free(tabtab);
							
          text = g_strsplit(temp, tab, 8);
          g_free(temp);
          
          // now add details to the log_details list
          nul_pos = strchr(log_line, 255);
          if (nul_pos != NULL)
            temp = g_strdup(nul_pos + 3);
          else
            temp = g_strdup(" ");

          // kill breaking segments by adding a space 
          if (temp) {
            tabtab = g_strdup_printf("%c%c%c",0xff,0x09,0x09);
            nul_pos = strstr(temp, tabtab);
            do {
              if (nul_pos) {
                pos = nul_pos - temp;
                temp = strchrdel(temp, pos, 3);
                temp = strchrins(" ", temp, pos);
                nul_pos = strstr(temp, tabtab);
              }
            } while (nul_pos != NULL);
            g_free(tabtab);
          }

	  gtk_list_store_append(store, &iter);
	  gtk_list_store_set(store, &iter,
	                     DATE, text[2],
	                     TIME, text[1],
	                     FREQ, text[0],
	                     CALL, text[6],
	                     STAT, text[7],
	                     COUN, text[4],
	                     MODE, text[3],
	                     SINPO, text[5],
	                     NOTES, temp,
                             -1);	                     
          j++;
          g_strfreev(text);
          g_free(tab);
        }
        g_free(log_line);
        for (i = 0; i < 10000; i++)
          buf[i] = 0x00;
      }
    }
    fclose(fp);

    gtk_tree_view_set_model ((GtkTreeView*)gen_item, (GtkTreeModel*)store);
    g_object_unref(store);

    j--;
    if (j > -1) //{
      set_list_selected_row(log1,"LogList",j);
    //} else {
    //  temp = g_strdup_printf("%s has no records!", log_name);
    //  show_message("Open Log File", GTK_MESSAGE_INFO, temp);
    //  g_free(temp);
    //}
    g_settings_set_string(sgc_settings,"last-log", log_name);
  }
}


void
clear_log_entry()
{
  GtkWidget*	gen_item;
  
  gen_item = lookup_widget(log1,"LogText");
  set_text(log1, "LogDateEntry", "");
  set_text(log1, "LogTimeEntry", "");
  set_text(log1, "LogFreqEntry", "");
  set_down(log1, "LogkHzBtn", true);
  set_text(log1, "LogStatEntry", "");
  set_text(log1, "LogCounEntry", "");
  set_text(log1, "LogModeEntry", "");
  set_text(log1, "LogSINPOEntry", "");
  clear_log_text(gen_item);
}


void
fill_log_display_from_faceplate				(GtkWidget*	selection)
{
  GtkTreeModel*	model;
  GtkTreeIter	iter;
  GValue	freqval = G_VALUE_INIT;
  GValue	langval = G_VALUE_INIT;
  GValue	targval = G_VALUE_INIT;
  GValue	counval = G_VALUE_INIT;
  GValue	statval = G_VALUE_INIT;
  gchar*	dateStr;
  gchar*	timeTemp;
  gchar*	timeStr;
  gchar*	modeStr;
  gchar*	freqStr;
  gchar*	counStr;
  gchar*	statStr;
  gchar*	textStr;
  double	freqTemp;
  gboolean	setMHz = false;

  clear_log_entry();    
  if (selection) {
    select_from_x_y(lookup_widget(app1,"clist1"), selection);
    if (gtk_tree_selection_get_selected((GtkTreeSelection*)selection, &model, &iter)) {
      dateStr = get_current_date();
      get_text(app1, "UTC2Lbl", &timeTemp);
      timeStr = g_strdup_printf("%s%c",timeTemp,get_current_dow_letter());
      if (ispower) {
        freqTemp = frequency;
        get_text(app1, "ModeBox", &modeStr);
      } else {
        gtk_tree_model_get_value(model, &iter, 0, &freqval);
        freqTemp = g_value_get_double(&freqval);
        modeStr = "";
      }  
      if (freqTemp >= 30000.0) {
        freqTemp = freqTemp / 1000.0;
        setMHz = true;
      }
      freqStr = g_strdup_printf("%8.2f", freqTemp);
      gtk_tree_model_get_value(model, &iter, 4, &langval);
      gtk_tree_model_get_value(model, &iter, 5, &targval);
      gtk_tree_model_get_value(model, &iter, 6, &counval);
      counStr = strdup(g_value_get_string(&counval));
      gtk_tree_model_get_value(model, &iter, 7, &statval);
      statStr = strdup(g_value_get_string(&statval));
      textStr = g_strdup_printf("In %s to %s.", g_value_get_string(&langval), g_value_get_string(&targval));
      
      set_text(log1, "LogDateEntry", dateStr);
      set_text(log1, "LogTimeEntry", timeStr);
      set_text(log1, "LogFreqEntry", freqStr);
      if (setMHz)
        set_down(log1, "LogMHzBtn", true);
      set_text(log1, "LogStatEntry", statStr);
      set_text(log1, "LogCounEntry", counStr);
      set_text(log1, "LogModeEntry", modeStr);
      set_log_data(textStr);
      
      g_value_unset(&freqval);
      g_value_unset(&langval);
      g_value_unset(&targval);
      g_value_unset(&counval);
      g_value_unset(&statval);
      g_free(dateStr);
      g_free(timeStr);
      g_free(freqStr);
      g_free(statStr);
      g_free(modeStr);
      g_free(textStr);
      
      if (!get_visible(log1,"LogWindow"))
        gtk_widget_set_visible(log1, true);
    }
  }    
}


void
modify_log_entry			(gint	log_mode)
{
  GtkWidget*	gen_item;
  gint		i;
  gint		j;
  gint		log_num;
  gint		max_row;
  char        	buf[10000];
  FILE*		fp1;
  FILE*		fp2;
  gchar*        tab;
  gchar*        crlf;
  gchar*	tabtab;
  gchar*	temp = NULL;
  gchar*	temp2;
  gchar*	text[9];
  gchar*	file_path;
  gchar*	new_file;
  gchar*	prompt_title;
  double	r_freq;

  enum {
    SGC_LOG_ADD = 0,
    SGC_LOG_MODIFY,
    SGC_LOG_DELETE,
    SGC_NUM_LOG_OPTIONS
  };

  log_num = get_list_selected_row(log1,"LogList");

  switch (log_mode) {
    case SGC_LOG_ADD :
      prompt_title = "Add Log Entry";
      log_num = get_list_num_rows(log1,"LogList"); //all the rows
      break;
    case SGC_LOG_MODIFY :
      prompt_title = "Modify Log Entry";
      break;
    case SGC_LOG_DELETE :
      prompt_title = "Delete Log Entry";
      break;
    default :
      prompt_title = "";
      break;
  }

  file_path = get_pathname_from_full_path(log_name);
  new_file = g_strdup_printf("%s%s",file_path,"temp.log");
  g_free(file_path);
	
  fp1 = fopen(log_name,"r");
  if (log_mode == SGC_LOG_ADD) {
    if (!fp1) {
      temp = g_strdup_printf("Creating new log file:\n%s",log_name);
      show_message(prompt_title, GTK_MESSAGE_INFO, temp);
      g_free(temp);
    }
  }  
  if ((!fp1) && (log_mode != SGC_LOG_ADD)) {
    temp = g_strdup_printf("Can't open %s.\nAborting...",log_name);
    show_message(prompt_title, GTK_MESSAGE_WARNING, temp);
    g_free(temp);
  } else {
    fp2 = fopen(new_file,"w");
    if (!fp2) {
      temp = g_strdup_printf("Can't create temporary file.\nAborting...");
      show_message(prompt_title, GTK_MESSAGE_WARNING, temp);
      g_free(temp);
    } else {
      // read and write all logs until the one to modify
      // if adding, log_num points to the end of the file
      if (fp1) { 
        for (i = 0; i < log_num; i++) {
          for (j = 0; j < 10000; j++)
            buf[j] = 0x00;
          fgets(buf,10000,fp1);
          fputs(buf,fp2);
        }
      }
			
      // read, but write new log line
      // don't read ofr add, since you're already at eof()
      if ((fp1) && (log_mode != SGC_LOG_ADD))
        fgets(buf,10000,fp1);
      
      //don't write new line for delete
      if (log_mode != SGC_LOG_DELETE) {
        crlf = g_strdup_printf("%c%c",0x0d,0x0a);
        tab = g_strdup_printf("%c",0x09);
        tabtab = g_strdup_printf("%c%c%c",0xff,0x09,0x09);
        get_text(log1,"LogFreqEntry",&text[0]);
        g_strstrip(text[0]);
        r_freq = g_strtod(text[0],NULL);
        if (get_down(log1,"LogkHzBtn"))
          temp = g_strdup_printf("%8.2f%skHz",r_freq,tab);
        else
          temp = g_strdup_printf("%8.5f%sMHz",r_freq,tab);
        get_text(log1,"LogTimeEntry",&text[1]);
        g_strstrip(text[1]);			
        get_text(log1,"LogDateEntry",&text[2]);
        g_strstrip(text[2]);
        get_text(log1,"LogModeEntry",&text[3]);
        g_strstrip(text[3]);
        get_text(log1,"LogSINPOEntry",&text[4]);
        g_strstrip(text[4]);
        get_text(log1,"LogCallEntry",&text[5]);
        g_strstrip(text[5]);
        get_text(log1,"LogCounEntry",&text[6]);
        g_strstrip(text[6]);
        get_text(log1,"LogStatEntry",&text[7]);
        g_strstrip(text[7]);
        gen_item = lookup_widget(log1,"LogText");
        text[8] = get_log_text(gen_item);
        text[8] = g_strdelimit(text[8],crlf,0x20);    
        g_strstrip(text[8]);
        temp2 = g_strdup_printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n", temp, tab,
                                                                        text[1], tab, text[2], tab, text[3], tab, text[6], tab,
                                                                        text[4], tab, text[5], tab, text[7], tabtab, text[8]);
        fputs(temp2,fp2);
        g_free(crlf);
        g_free(tab);
        g_free(tabtab);
        g_free(temp);
        g_free(temp2);
      }
      
      // read and write the rest of the logs if not already adding
      if ((fp1) && (log_mode != SGC_LOG_ADD)) {
        while (!feof(fp1)) {
          for (j = 0; j < 10000; j++)
            buf[j] = 0x00;
          fgets(buf,10000,fp1);
          fputs(buf,fp2);
        }
      }
      fclose(fp2);
      if (fp1)
        fclose(fp1);
			
      // now delete old one and rename new one to old name, then reload.
      if (remove(log_name) == 0) {
        if (rename(new_file, log_name) == 0) {
          // reload the log file
          open_logfile();
          if (log_mode == SGC_LOG_DELETE) {
            max_row = get_list_num_rows(log1,"LogList")-1;
            if (log_num > max_row)
              log_num = max_row;
          }
          if (log_mode != SGC_LOG_ADD)
            set_list_selected_row(log1,"LogList", log_num);
        }
      }
    }
  }
}


