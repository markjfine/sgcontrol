/*
 * Smart Gnome Control - log functions definition file
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

#ifndef HAVE_LOGFILE_H
#define HAVE_LOGFILE_H 1
#pragma once

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef __MINGW64__
void
replace_backslashes(gchar* inStr);
#endif

gchar*
get_user_base(void);

void
prep_user_dir(void);

gboolean
file_exists                             (gchar*		in_file);

void
get_LogList_data			(GtkWidget*	selection);


void
set_sort_menu_items			(gint		option_id);

void
get_log_file				(gint		file_mode);

void
open_logfile(void);

void
clear_log_entry(void);

void
fill_log_display_from_faceplate		(GtkWidget*	selection);

void
modify_log_entry			(gint		log_mode);

#endif

