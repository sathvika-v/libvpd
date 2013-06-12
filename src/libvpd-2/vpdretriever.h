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

#ifndef VPDRETRIEVER_H_
#define VPDRETRIEVER_H_

#include <libvpd-2/component.h>
#include <libvpd-2/system.h>
#include <libvpd-2/vpddbenv.h>

/*
 * struct vpdretriever is the outside world's access point for the VPD database
 * It can be used to request individual hardware records or the entire VPD tree
 * as the user needs.
 */

struct vpdretriever
{
	struct vpddbenv *dbenv;
};

/*
 * Creates a new vpdretiever, takes a directory where the VPD db lives, a
 * filename for the db to query, and an unsigned 32 bit integer that holds the
 * flags to be passed to onto the BerekelyDB create commands.  This function
 * will open the default location for a VPD db (dir='/var/lib/lsvpd' file='db')
 * and no flags specified to the BerkeleyDB library.  The pointer returned is
 * malloc'd and should be free'd using the free_vpdretriever function.  On
 * error NULL is returned. 
 */
struct vpdretriever * new_vpdretriever( const char* dir, const char *file );

/*
 * Releases all resources used by this vpdretriever.  Closes connections to the
 * VPD db and free's all malloc'd memory.
 */
void free_vpdretriever( struct vpdretriever *freeme );

/*
 * Retrieves the entire tree of device VPD.  The pointer returned is malloc'd
 * and should be free'd using the free_system function from system.h.  On error
 * NULL is returned.
 */
struct system * get_component_tree( struct vpdretriever *dbenv );

/*
 * Retrieves the specified component.  The pointer returned is malloc'd and
 * should be free'd using free_component function from component.h.  On error
 * NULL is returned.
 */
struct component * get_component( struct vpdretriever *dbenv, const char *id );

/*
 * Retrieves the system level VPD.  The pointer returned is malloc'd and
 * should be free'd using free_system function from system.h.  On error
 * NULL is returned.
 */
struct system * get_system( struct vpdretriever *dbenv );

#endif /*VPDRETRIEVER_H_*/
