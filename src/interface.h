/*
 * Smart Gnome Control - GUI interface definition file
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

#ifndef HAVE_INTERFACE_H
#define HAVE_INTERFACE_H 1
#pragma once

GtkWidget* create_app1 (void);
GtkWidget* create_OptionsDlg (void);
GtkWidget* create_AboutDlg (void);
GtkWidget* create_MemWindow (void);
GtkWidget* create_TuningDlg (void);
GtkWidget* create_LogWindow (void);
GtkWidget* create_EntryDlg (void);

#endif
