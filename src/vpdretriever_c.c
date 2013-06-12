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

#include <stdlib.h>
#include <string.h>

#include "libvpd-2/vpdretriever.h"

static int build_component_sub( struct vpdretriever *dbenv, struct component *root )
{
	struct list *child;
	struct component *addme;
	char *id;
	
	child = root->childrenIDs;
	while( child )
	{
		addme = get_component( dbenv, (char*)child->data );
		if( !addme )
			return 1;
		
		/*
		 * Put this new component at the front of the child list because order
		 * does not matter
		 */
		addme->next = root->children;
		root->children = addme;
		if( build_component_sub( dbenv, addme ) )
			return 1;
		
		child = child->next;
	}
		
	return 0;
}

static int build_system_sub( struct vpdretriever *dbenv, struct system *root )
{
	struct list *child;
	struct component *addme;
	char *id;
	
	child = root->childrenIDs;
	while( child )
	{
		addme = get_component( dbenv, (char*)child->data );
		if( !addme )
			return 1;
		
		/*
		 * Put this new component at the front of the child list because order
		 * does not matter
		 */
		addme->next = root->children;
		root->children = addme;
		if( build_component_sub( dbenv, addme ) )
			return 1;
		
		child = child->next;
	}
	
return 0;
}

struct vpdretriever * new_vpdretriever( const char* dir, const char *file )
{
	struct vpdretriever *ret = NULL;
	
	ret = malloc( sizeof( struct vpdretriever ) );
	if( !ret )
		return NULL;
	
	ret->dbenv = new_vpddbenv( dir, file );
	if( !ret->dbenv )
	{
		free( ret );
		return NULL;
	}
	
	return ret;
}

void free_vpdretriever( struct vpdretriever *freeme )
{
	if( !freeme )
		return;
	
	free_vpddbenv( freeme->dbenv );
	free( freeme );
}

struct system * get_component_tree( struct vpdretriever * dbenv )
{
	struct system *root;
	
	if( !dbenv )
		return NULL;
	
	root = get_system( dbenv );
	
	if( !root )
		return NULL;
	
	if( build_system_sub( dbenv, root ) )
		goto geterr;
	
	return root;
	
geterr:
	free_system( root );
	return NULL;
}

struct component * get_component( struct vpdretriever * dbenv, const char *id )
{
	if( !dbenv )
		return NULL;
	
	return fetch_component( dbenv->dbenv, id );
}

struct system * get_system( struct vpdretriever * dbenv )
{
	if( !dbenv )
		return NULL;
	return fetch_system( dbenv->dbenv );
}
