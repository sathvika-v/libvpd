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

#include <libvpd-2/common.h>

#include <stdlib.h>
#include <string.h>

struct list* new_list( )
{
	struct list *ret = NULL;
	ret = malloc( sizeof( struct list ) );
	if( !ret )
		return ret;
	
	memset( ret, 0, sizeof( struct list ) );
	return ret;
}

void free_list( struct list *head )
{
	struct list *next;

	while( head )
	{
		next = head->next;
		if( head->data )
			free( head->data );
		free( head );
		head = next;
	}
}

struct list* concat_list( struct list *head, const struct list *addme )
{
	struct list *ret = head;
	if( !head || !addme )
		return NULL;
	
	while( head->next )
		head = head->next;
	head->next = addme;
	
	return ret;
}
