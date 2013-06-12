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

#include <libvpd-2/system.h>
#include <stdlib.h>
#include <string.h>

struct system * new_system( int init )
{
	struct system *ret = NULL;
	ret = malloc( sizeof( struct system ) );
	if( !ret )
		return NULL;
	
	memset( ret, 0 , sizeof( struct system ) );
	
	if( init )
	{
		ret->id = new_dataitem( );
		if( !ret->id )
			goto newsyserr;
		ret->id->dataValue = strdup( SYS_ID );
		
		ret->description = new_dataitem( );
		if( !ret->description )
			goto newsyserr;
		ret->description->ac = strdup( "DS" );
		ret->description->humanName = strdup( "Description" );
		
		ret->description = new_dataitem( );
		if( !ret->description )
			goto newsyserr;
		ret->description->ac = strdup( "BR" );
		ret->description->humanName = strdup( "Brand Keyword" );
		
		ret->description = new_dataitem( );
		if( !ret->description )
			goto newsyserr;
		ret->description->ac = strdup( "OS" );
		ret->description->humanName = strdup( "Operation System" );
		
		ret->description = new_dataitem( );
		if( !ret->description )
			goto newsyserr;
		ret->description->ac = strdup( "PI" );
		ret->description->humanName = strdup( "Processor ID or Unique ID" );
		
		ret->description = new_dataitem( );
		if( !ret->description )
			goto newsyserr;
		ret->description->ac = strdup( "TM" );
		ret->description->humanName = strdup( "Machine Type" );
		
		ret->description = new_dataitem( );
		if( !ret->description )
			goto newsyserr;
		ret->description->ac = strdup( "TM" );
		ret->description->humanName = strdup( "Machine Model" );
		
		ret->description = new_dataitem( );
		if( !ret->description )
			goto newsyserr;
		ret->description->ac = strdup( "FC" );
		ret->description->humanName = strdup( "Feature Code" );
		
		ret->description = new_dataitem( );
		if( !ret->description )
			goto newsyserr;
		ret->description->ac = strdup( "FG" );
		ret->description->humanName = strdup( "Flag Field" );
		
		ret->description = new_dataitem( );
		if( !ret->description )
			goto newsyserr;
		ret->description->ac = strdup( "RT" );
		ret->description->humanName = strdup( "Record Type" );
		
		ret->description = new_dataitem( );
		if( !ret->description )
			goto newsyserr;
		ret->description->ac = strdup( "SE" );
		ret->description->humanName = strdup( "Machine or Cabinet Serial Number" );
		
		ret->description = new_dataitem( );
		if( !ret->description )
			goto newsyserr;
		ret->description->ac = strdup( "SE" );
		ret->description->humanName = strdup( "Machine or Cabinet Serial Number" );
		
		ret->description = new_dataitem( );
		if( !ret->description )
			goto newsyserr;
		ret->description->ac = strdup( "SU" );
		ret->description->humanName = strdup( "System Unique ID" );
		
		ret->description = new_dataitem( );
		if( !ret->description )
			goto newsyserr;
		ret->description->ac = strdup( "VK" );
		ret->description->humanName = strdup( "Version of Keywords" );
		
		ret->description = new_dataitem( );
		if( !ret->description )
			goto newsyserr;
		ret->description->ac = strdup( "YL" );
		ret->description->humanName = strdup( "Physical Location" );
		
		ret->cpuCount = 1;
	}
	
	return ret;
newsyserr:
	free_system( ret );
	return NULL;
}

struct system * unpack_system( void * buffer )
{
	struct system *ret = NULL;
	u32 size = 0, netOrder;
	char *packed = (char*)buffer;
	char * next;
	struct list *item;
	struct dataitem *data;
	
	if( !buffer )
		return ret;

	ret = new_system( 0 );
	if( !ret )
		return ret;
	
	memcpy( &netOrder, packed, sizeof( u32 ) );
	size = ntohl( netOrder );
	next = packed + sizeof( u32 );
	
	memcpy( &netOrder, next, sizeof( u32 ) );
	ret->cpuCount = ntohl( netOrder );
	next = next + sizeof( u32 );
	
	ret->id = unpack_dataitem( next );
	if( !ret->id )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->id );
	
	ret->arch = unpack_dataitem( next );
	if( !ret->arch )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->arch );
	
	ret->deviceTreeNode = unpack_dataitem( next );
	if( !ret->deviceTreeNode )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->deviceTreeNode );
	
	ret->description = unpack_dataitem( next );
	if( !ret->description )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->description );
	
	ret->brand = unpack_dataitem( next );
	if( !ret->brand )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->brand );
	
	ret->nodeName = unpack_dataitem( next );
	if( !ret->nodeName )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->nodeName );
	
	ret->os = unpack_dataitem( next );
	if( !ret->os )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->os );
	
	ret->processorID = unpack_dataitem( next );
	if( !ret->processorID )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->processorID );
	
	ret->machineType = unpack_dataitem( next );
	if( !ret->machineType )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->machineType );

	ret->machineModel = unpack_dataitem( next );
	if( !ret->machineModel )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->machineModel );
	
	ret->featureCode = unpack_dataitem( next );
	if( !ret->featureCode )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->featureCode );
	
	ret->flagField = unpack_dataitem( next );
	if( !ret->flagField )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->flagField );
	
	ret->recordType = unpack_dataitem( next );
	if( !ret->recordType )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->recordType );
	
	ret->serialNum1 = unpack_dataitem( next );
	if( !ret->serialNum1 )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->serialNum1 );
	
	ret->serialNum2 = unpack_dataitem( next );
	if( !ret->serialNum2 )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->serialNum2 );
	
	ret->suid = unpack_dataitem( next );
	if( !ret->suid )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->suid );
	
	ret->keywordVersion = unpack_dataitem( next );
	if( !ret->keywordVersion )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->keywordVersion );
	
	ret->locationCode = unpack_dataitem( next );
	if( !ret->locationCode )
		goto unpackerr;
	next = next + calc_packed_length_dataitem( ret->locationCode );

	while( 0 != strncmp( next, CHILD_START, strlen( CHILD_START ) ) )
	{
		next++;
		if( next > packed + size )
			goto unpackerr;
	}
	
	if( strncmp( next, CHILD_START, strlen( CHILD_START ) ) == 0 )
	{
		next = next + strlen( CHILD_START ) + 1;
		if( next > packed + size )
			goto unpackerr;

		while( strncmp( next, CHILD_END, strlen( CHILD_END ) ) != 0 )
		{
			item = new_list( );
			if( !next )
				goto unpackerr;
			item->data = strdup( next );
			next = next + strlen( (char*)item->data ) + 1;
			if( !ret->childrenIDs )
				ret->childrenIDs = item;
			else
				concat_list( ret->childrenIDs, item );
			
			if( next > packed + size )
				goto unpackerr;
		}
		next = next + strlen( CHILD_END ) + 1;
	}
	
	if( strncmp( next, DEVICE_START, strlen( DEVICE_START ) ) == 0 )
	{
		next = next + strlen( DEVICE_START ) + 1;
		if( next > packed + size )
			goto unpackerr;
		
		while( strncmp( next, DEVICE_END, strlen( DEVICE_END ) ) != 0 )
		{
			data = unpack_dataitem( next );
			if( !data )
				goto unpackerr;
			
			next = next + calc_packed_length_dataitem( data );
			if( !ret->deviceSpecific )
				ret->deviceSpecific = data;
			else
				add_dataitem( ret->deviceSpecific, data );
			
			if( next > packed + size )
				goto unpackerr;
		}
		next = next + strlen( DEVICE_END ) + 1;
	}
	
	if( strncmp( next, USER_START, strlen( USER_START ) ) == 0 )
	{
		next = next + strlen( USER_START ) + 1;
		if( next > packed + size )
			goto unpackerr;
		
		while( strncmp( next, USER_END, strlen( USER_END ) ) != 0 )
		{
			data = unpack_dataitem( next );
			if( !data )
				goto unpackerr;
			
			next = next + calc_packed_length_dataitem( data );
			if( !ret->userData )
				ret->userData = data;
			else
				add_dataitem( ret->userData, data );
			
			if( next > packed + size )
				goto unpackerr;
		}
	}
	
	return ret;
	
unpackerr:
	free_system( ret );
	return NULL;
}

void free_system( struct system *freeme )
{
	if( !freeme )
		return;
	
	free_dataitem( freeme->id );
	free_dataitem( freeme->description );
	free_dataitem( freeme->arch );
	free_dataitem( freeme->deviceTreeNode );
	free_dataitem( freeme->brand );
	free_dataitem( freeme->nodeName );
	free_dataitem( freeme->os );
	free_dataitem( freeme->processorID );
	free_dataitem( freeme->machineType );
	free_dataitem( freeme->machineModel );
	free_dataitem( freeme->featureCode );
	free_dataitem( freeme->flagField );
	free_dataitem( freeme->recordType );
	free_dataitem( freeme->serialNum1 );
	free_dataitem( freeme->serialNum2 );
	free_dataitem( freeme->suid );
	free_dataitem( freeme->keywordVersion );
	free_dataitem( freeme->locationCode );
	
	free_dataitem( freeme->deviceSpecific );
	free_dataitem( freeme->userData );
	free_list( freeme->childrenIDs );
	free( freeme );
}
