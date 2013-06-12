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

#include "libvpd-2/dataitem.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct dataitem* new_dataitem( )
{
	struct dataitem *ret = NULL;
	
	ret = malloc( sizeof( struct dataitem ) );
	if( !ret )
		return ret;
	
	memset( ret, 0, sizeof( struct dataitem ) );
	
	return ret;
}

void free_dataitem( struct dataitem *freeme )
{
	struct dataitem *next = NULL;
	struct dataitem *this = NULL;
	
	if( !freeme )
		return;
	
	if( freeme->humanName )
		free( freeme->humanName );
	if( freeme->ac )
		free( freeme->ac );
	if( freeme->dataValue )
		free( freeme->dataValue );
	
	this = freeme->next;
	while( this )
	{
		next = this->next;
		free_dataitem( this );
		this = next;
	}
	
	free( freeme );
}

int calc_packed_length_dataitem( struct dataitem *packme )
{
	int ret = 3;
	if( !packme )
		return 0;
		
	if( packme->ac )
		ret += strlen( packme->ac );
	if( packme->dataValue )
		ret += strlen( packme->dataValue );
	if( packme->humanName )
		ret += strlen( packme->humanName );
	
	return ret;
}

struct dataitem * unpack_dataitem( void *buffer )
{
	char *buf = (char*)buffer;
	struct dataitem *ret = NULL;
	
	ret = new_dataitem( );
	if( !ret )
		return ret;
	
	ret->ac = strdup( buf );
	buf += strlen( ret->ac ) + 1;
	ret->humanName = strdup( buf );
	buf += strlen( ret->humanName ) + 1;
	ret->dataValue = strdup( buf );
	return ret;
}

void add_dataitem( struct dataitem *head, const struct dataitem *addme )
{
	if( !head || !addme )
		return;
	
	while( head->next )
		head = head->next;
	head->next = addme;
}
