/***************************************************************************
 *   Copyright (C) 2007, IBM                                               *
 *                                                                         *
 *   Maintained By:                                                        *
 *   Eric Munson and Brad Peters                                           *
 *   munsone@us.ibm.com, bpeters@us.ibm.com                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the Lesser GNU General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or at your option) any later version.                        *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the Lesser GNU General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef VPDDBENV_H_
#define VPDDBENV_H_

#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include <libvpd-2/component.h>
#include <libvpd-2/system.h>

#define DEFAULT_ENV      "/var/lib/lsvpd"
#define DEFAULT_DB       "vpd.db"
#define TABLE_NAME       "components"
#define ID               "comp_id"
#define DATA             "comp_data"
#define MAX_NAME_LENGTH  256
#define QUERY_BUF_LENGTH 4096

#if HAVE_SQLITE3_PREPARE_V2
#define SQLITE3_PREPARE sqlite3_prepare_v2
#else
#define SQLITE3_PREPARE sqlite3_prepare
#endif

struct vpddbenv
{
	char envDir[ MAX_NAME_LENGTH + 1 ];
	char dbFileName[ MAX_NAME_LENGTH + 1 ];
	char fullPath[ MAX_NAME_LENGTH * 2 + 2 ];
	sqlite3 *db;
};

struct vpddbenv * new_vpddbenv( const char *dir, const char *file );
void free_vpddbenv( struct vpddbenv *freeme );
struct component* fetch_component( struct vpddbenv *db, const char *deviceID );
struct system* fetch_system( struct vpddbenv *db );

#endif /*VPDDBENV_H_*/
