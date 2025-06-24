/*
 * Smart Gnome Control - database functions definition file
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
 
#ifndef HAVE_DATABASE_H
#define HAVE_DATABASE_H 1
#pragma once

typedef union {
  unsigned char		c[4];
  unsigned short	s[2];
  unsigned long		l;
  float			f;
} ShortsFloat;

typedef union {
  unsigned char		s[2];
  unsigned short	i;
} ShortsInt;

typedef struct {
  union {
    guint16		pOn;   // : SmallInt; 2
    guchar		pOf1[2];
  };
  union {
    guint16		pOff;  // : SmallInt; 2
    guchar		pOf2[2];
  };
  guchar		pDays; // : Byte;     1
  guint32		pSInx; // : Integer;  2
  guint32		pCInx; // : Integer;  2
  guint32		pLInx; // : Integer;  2 
  gfloat		pFreq; // : Real;     6
  gchar			pTar[2];  // : Str2;  2
  gchar			pHear; // : Char;     1
} db_record_t;

typedef struct {
  gchar			pStrg[25];
} st_record_t;

void
get_database_file(void);

void
doInputTest				(GtkWidget*	sEntry);

gboolean
checkWebPage				(gboolean	isBackground);

void
open_database(void);

void
read_statsType(void);

void
set_filter_menu_items			(gint		option_id);

void
set_filter_data_box			(GtkWidget*	filterBox,
                                         gint		option_id);

void
get_filter_data_box			(GtkWidget*	filterBox,
                                         gint		option_id);

void
get_stats_records                      (GtkWidget*	cList,
                                        gchar*		inLanguage,
                                        gchar*		inCountry,
                                        gchar*		inRegion,
                                        gchar*		inTarget,
                                        gboolean	doExpand);

void
get_freq_records                       (GtkWidget*	clist,
                                        gdouble		inFreq);

void
get_stat_records			(GtkWidget*	cList,
                                         gchar*		inStation,
                                         gchar*		inLanguage,
                                         gchar*		inTarget);

void
get_station_data			(GtkWidget*	selection,
                                         gboolean	fromStations);

void
get_frequency_data			(GtkWidget*	selection);

void
doRefresh(void);

void
refreshList				(gchar*		inList);

#endif
