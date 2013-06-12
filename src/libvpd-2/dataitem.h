/***************************************************************************
 *   Copyright (C) 2007, IBM                                               *
 *                                                                         *
 *   Maintained By:                                                        *
 *   Eric Munson and Brad Peters                                           *
 *   ebmunson@us.ibm.com, bpeters@us.ibm.com                               *
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

#ifndef DATAITEM_H_
#define DATAITEM_H_

struct dataitem
{
	char *humanName;
	char *ac;
	char *dataValue;
	struct dataitem *next;
};

struct dataitem* new_dataitem( );
void free_dataitem( struct dataitem *freeme );
int calc_packed_length_dataitem( struct dataitem *packme );
struct dataitem * unpack_dataitem( void *buffer );
void add_dataitem( struct dataitem *head, const struct dataitem *addme );

#endif /*DATAITEM_H_*/
