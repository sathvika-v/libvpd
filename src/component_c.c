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

#include "libvpd-2/component.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

struct component* new_component( int init )
{
	struct component *ret = NULL;
	
	ret = malloc( sizeof( struct component ) );
	if( !ret )
		return NULL;
	
	memset( ret, 0, sizeof( struct component ) );
	
	if( init )
	{
		ret->id = new_dataitem( );
		if( !ret->id )
			goto outerr;
		ret->id->humanName = strdup( "Main Device Node, equals sysFsNode or deviceTreeNode" );
		
		ret->deviceTreeNode = new_dataitem( );
		if( !ret->deviceTreeNode )
			goto outerr;
		ret->deviceTreeNode->humanName = strdup( "/proc/device-tree Device Node" );
		
		ret->sysFsNode = new_dataitem( );
		if( !ret->sysFsNode )
			goto outerr;
		ret->sysFsNode->humanName = strdup( "/sys Device Node" );
		
		ret->sysFsLinkTarget = new_dataitem( );
		if( !ret->sysFsLinkTarget )
			goto outerr;
		ret->sysFsLinkTarget->humanName = strdup( "/sys/bus Device Node" );
		
		ret->devClass = new_dataitem( );
		if( !ret->devClass )
			goto outerr;
		ret->devClass->humanName = strdup( "/sys/bus/XXX - Device Class" );
		
		ret->fru = new_dataitem( );
		if( !ret->fru )
			goto outerr;
		ret->fru->ac = strdup( "FN" );
		ret->fru->humanName = strdup( "Field Replaceable Unit Number" );
		
		ret->scsiDetail = new_dataitem( );
		if( !ret->scsiDetail )
			goto outerr;
		ret->scsiDetail->ac = strdup( "ZZ" );
		ret->scsiDetail->humanName = strdup( "Device Details" );
		
		ret->plantMfg = new_dataitem( );
		if( !ret->plantMfg )
			goto outerr;
		ret->plantMfg->ac = strdup( "SE" );
		ret->plantMfg->humanName = strdup( "Plant of manufacture" );
		
		ret->netAddr = new_dataitem( );
		if( !ret->netAddr )
			goto outerr;
		ret->netAddr->ac = strdup( "NA" );
		ret->netAddr->humanName = strdup( "Network Address" );
		
		ret->description = new_dataitem( );
		if( !ret->description )
			goto outerr;
		ret->description->ac = strdup( "DS" );
		ret->description->humanName = strdup( "Displayable Message" );
		
		ret->cdField = new_dataitem( );
		if( !ret->cdField )
			goto outerr;
		ret->cdField->ac = strdup( "CD" );
		ret->cdField->humanName = strdup( "Card ID" );
		
		ret->serialNumber = new_dataitem( );
		if( !ret->serialNumber )
			goto outerr;
		ret->serialNumber->ac = strdup( "SN" );
		ret->serialNumber->humanName = strdup( "Serial Number" );
		
		ret->partNumber = new_dataitem( );
		if( !ret->partNumber )
			goto outerr;
		ret->partNumber->ac = strdup( "PN" );
		ret->partNumber->humanName = strdup( "Part Number of Assembly" );
		
		ret->firmwareLevel = new_dataitem( );
		if( !ret->firmwareLevel )
			goto outerr;
		ret->firmwareLevel->ac = strdup( "RL" );
		ret->firmwareLevel->humanName = strdup( "Non-alterable ROM level" );
		
		ret->firmwareVersion = new_dataitem( );
		if( !ret->firmwareVersion )
			goto outerr;
		ret->firmwareVersion->ac = strdup( "RM" );
		ret->firmwareVersion->humanName = strdup( "Alterable ROM level" );
		
		ret->manufacturer = new_dataitem( );
		if( !ret->manufacturer )
			goto outerr;
		ret->manufacturer->ac = strdup( "MF" );
		ret->manufacturer->humanName = strdup( "Manufacturer Name" );
		
		ret->model = new_dataitem( );
		if( !ret->model )
			goto outerr;
		ret->model->ac = strdup( "TM" );
		ret->model->humanName = strdup( "Machine Type-Model" );
		
		ret->manufacturerID = new_dataitem( );
		if( !ret->manufacturerID )
			goto outerr;
		ret->manufacturerID->ac = strdup( "MN" );
		ret->manufacturerID->humanName = strdup( "Manufacturer ID" );
		
		ret->engChangeLevel = new_dataitem( );
		if( !ret->engChangeLevel )
			goto outerr;
		ret->engChangeLevel->ac = strdup( "EC" );
		ret->engChangeLevel->humanName = strdup( "Engineering Change Level" );
		
		ret->featureCode = new_dataitem( );
		if( !ret->featureCode )
			goto outerr;
		ret->featureCode->ac = strdup( "FC" );
		ret->featureCode->humanName = strdup( "Feature Code or Request for Price Quotation (RPQ) number" );
		
		ret->devDriver = new_dataitem( );
		if( !ret->devDriver )
			goto outerr;
		ret->devDriver->ac = strdup( "DD" );
		ret->devDriver->humanName = strdup( "Device Driver Level" );
		
		ret->recordType = new_dataitem( );
		if( !ret->recordType )
			goto outerr;
		ret->recordType->ac = strdup( "RT" );
		ret->recordType->humanName = strdup( "Record Type" );
		
		ret->keywordVersion = new_dataitem( );
		if( !ret->keywordVersion )
			goto outerr;
		ret->keywordVersion->ac = strdup( "VK" );
		ret->keywordVersion->humanName = strdup( "Keyword Version" );
		
		ret->microCodeImage = new_dataitem( );
		if( !ret->microCodeImage )
			goto outerr;
		ret->microCodeImage->ac = strdup( "MI" );
		ret->microCodeImage->humanName = strdup( "Keyword Version" );
		
		ret->physicalLocation = new_dataitem( );
		if( !ret->physicalLocation )
			goto outerr;
		ret->physicalLocation->ac = strdup( "YL" );
		ret->physicalLocation->humanName = strdup( "Location Code" );
		
		ret->secondLocation = new_dataitem( );
		if( !ret->secondLocation )
			goto outerr;
		ret->secondLocation->ac = strdup( "YL" );
		ret->secondLocation->humanName = strdup( "Location Code" );
		
		ret->devBus = new_dataitem( );
		if( !ret->devBus )
			goto outerr;
		ret->devBus->humanName = strdup( "Device Bus" );
	}
	
	return ret;
outerr:
	free_component( ret );
	return NULL;
}

void free_component( struct component *freeme )
{
	struct component *this, *next;
	struct list *ids;
	
	/* Free dataitems */
	free_dataitem( freeme->id );
	free_dataitem( freeme->deviceTreeNode );
	free_dataitem( freeme->sysFsNode );
	free_dataitem( freeme->sysFsLinkTarget );
	free_dataitem( freeme->halUDI );
	free_dataitem( freeme->netAddr );
	free_dataitem( freeme->devClass );
	free_dataitem( freeme->description );
	free_dataitem( freeme->cdField );
	free_dataitem( freeme->serialNumber );
	free_dataitem( freeme->partNumber );
	free_dataitem( freeme->firmwareLevel );
	free_dataitem( freeme->firmwareVersion );
	free_dataitem( freeme->fru );
	free_dataitem( freeme->manufacturer );
	free_dataitem( freeme->model );
	free_dataitem( freeme->manufacturerID );
	free_dataitem( freeme->engChangeLevel );
	free_dataitem( freeme->parent );
	free_dataitem( freeme->devSubSystem );
	free_dataitem( freeme->devDriver );
	free_dataitem( freeme->devKernel );
	free_dataitem( freeme->devKernelNumber );
	free_dataitem( freeme->devSysName );
	free_dataitem( freeme->devDevTreeName );
	free_dataitem( freeme->devBus );
	free_dataitem( freeme->devType );
	free_dataitem( freeme->devBusAddr );
	free_dataitem( freeme->microCodeImage );
	free_dataitem( freeme->recordType );
	free_dataitem( freeme->scsiDetail );
	free_dataitem( freeme->plantMfg );
	free_dataitem( freeme->featureCode );
	free_dataitem( freeme->keywordVersion );
	free_dataitem( freeme->microCodeVersion );
	free_dataitem( freeme->physicalLocation );
	free_dataitem( freeme->secondLocation );
	free_dataitem( freeme->deviceSpecific );
	free_dataitem( freeme->userData );
	free_dataitem( freeme->aixNames );
	
	/* Free Children */
	this = freeme->children;
	while( this )
	{
		next = this->next;
		free_component( this );
		this = next;
	}
	
	if( freeme->childrenIDs )
	{
		ids = freeme->childrenIDs;
		while( ids )
		{
			free_list( ids );
			ids = ids->next;
		}
	}
	
	free( freeme );
}

/**
 * Code adapted from component.cpp, dataitems are unpacked in the same
 * order as the Component::unpack( void* ) method to maintain consistency.
 */
struct component * unpack_component( void *buffer )
{
	struct component *ret = NULL;
	u32 size = 0, netOrder;
	char *packed = (char*)buffer;
	char * next;
	struct list *item;
	struct dataitem *data;
	
	if( !buffer )
		return ret;
	
	ret = new_component( 0 );
	if( !ret )
		return ret;
	
	memcpy( &netOrder, packed, sizeof( u32 ) );
	size = ntohl( netOrder );
	next = packed + sizeof( u32 );
	
	ret->id = unpack_dataitem( next );
	if( !ret->id )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->id );
	
	ret->deviceTreeNode = unpack_dataitem( next );
	if( !ret->deviceTreeNode )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->deviceTreeNode );
	
	ret->sysFsNode = unpack_dataitem( next );
	if( !ret->sysFsNode )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->sysFsNode );
	
	ret->sysFsLinkTarget = unpack_dataitem( next );
	if( !ret->sysFsLinkTarget )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->sysFsLinkTarget );
	
	ret->halUDI = unpack_dataitem( next );
	if( !ret->halUDI )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->halUDI );
	
	ret->netAddr = unpack_dataitem( next );
	if( !ret->netAddr )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->netAddr );
	
	ret->devClass = unpack_dataitem( next );
	if( !ret->devClass )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->devClass );
	
	ret->description = unpack_dataitem( next );
	if( !ret->description )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->description );
	
	ret->cdField = unpack_dataitem( next );
	if( !ret->cdField )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->cdField );
	
	ret->serialNumber = unpack_dataitem( next );
	if( !ret->serialNumber )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->serialNumber );
	
	ret->partNumber = unpack_dataitem( next );
	if( !ret->partNumber )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->partNumber );
	
	ret->firmwareLevel = unpack_dataitem( next );
	if( !ret->firmwareLevel )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->firmwareLevel );
	
	ret->firmwareVersion = unpack_dataitem( next );
	if( !ret->firmwareVersion )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->firmwareVersion );
	
	ret->fru = unpack_dataitem( next );
	if( !ret->fru )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->fru );
	
	ret->manufacturer = unpack_dataitem( next );
	if( !ret->manufacturer )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->manufacturer );
	
	ret->model = unpack_dataitem( next );
	if( !ret->model )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->model );
	
	ret->manufacturerID = unpack_dataitem( next );
	if( !ret->manufacturerID )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->manufacturerID );
	
	ret->engChangeLevel = unpack_dataitem( next );
	if( !ret->engChangeLevel )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->engChangeLevel );
	
	ret->parent = unpack_dataitem( next );
	if( !ret->parent )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->parent );
	
	ret->devSubSystem = unpack_dataitem( next );
	if( !ret->devSubSystem )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->devSubSystem );
	
	ret->devDriver = unpack_dataitem( next );
	if( !ret->devDriver )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->devDriver );
	
	ret->devKernel = unpack_dataitem( next );
	if( !ret->devKernel )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->devKernel );
	
	ret->devKernelNumber = unpack_dataitem( next );
	if( !ret->devKernelNumber )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->devKernelNumber );
	
	ret->devSysName = unpack_dataitem( next );
	if( !ret->devSysName )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->devSysName );
	
	ret->devDevTreeName = unpack_dataitem( next );
	if( !ret->devDevTreeName )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->devDevTreeName );
	
	ret->devBus = unpack_dataitem( next );
	if( !ret->devBus )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->devBus );
	
	ret->devBusAddr = unpack_dataitem( next );
	if( !ret->devBusAddr )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->devBusAddr );
	
	ret->recordType = unpack_dataitem( next );
	if( !ret->recordType )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->recordType );
	
	ret->scsiDetail = unpack_dataitem( next );
	if( !ret->scsiDetail )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->scsiDetail );
	
	ret->plantMfg = unpack_dataitem( next );
	if( !ret->plantMfg )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->plantMfg );
	
	ret->featureCode = unpack_dataitem( next );
	if( !ret->featureCode )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->featureCode );
	
	ret->keywordVersion = unpack_dataitem( next );
	if( !ret->keywordVersion )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->keywordVersion );
	
	ret->microCodeImage = unpack_dataitem( next );
	if( !ret->microCodeImage )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->microCodeImage );
	
	ret->secondLocation = unpack_dataitem( next );
	if( !ret->secondLocation )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->secondLocation );
	
	ret->physicalLocation = unpack_dataitem( next );
	if( !ret->physicalLocation )
		goto unpackerr;
	next += calc_packed_length_dataitem( ret->physicalLocation );
	
	while( 0 == strncmp( next, CHILD_START, strlen( CHILD_START ) ) )
	{
		next++;
		if( next > packed + size )
			goto unpackerr;
	}
	
	if( strncmp( next, CHILD_START, strlen( CHILD_START ) ) == 0 )
	{
		next += strlen( CHILD_START ) + 1;
		if( next > packed + size )
			goto unpackerr;

		while( strncmp( next, CHILD_END, strlen( CHILD_END ) ) != 0 )
		{
			item = new_list( );
			if( !next )
				goto unpackerr;
			item->data = strdup( next );
			next += strlen( (char*)item->data ) + 1;
			if( !ret->childrenIDs )
				ret->childrenIDs = item;
			else
				concat_list( ret->childrenIDs, item );
			
			if( next > packed + size )
				goto unpackerr;
		}
		
		next += strlen( CHILD_END ) + 1;
	}
	
	if( strncmp( next, DEVICE_START, strlen( DEVICE_START ) ) == 0 )
	{
		next += strlen( DEVICE_START ) + 1;
		if( next > packed + size )
			goto unpackerr;
		
		while( strncmp( next, DEVICE_END, strlen( DEVICE_END ) ) != 0 )
		{
			data = unpack_dataitem( next );
			if( !data )
				goto unpackerr;
			
			next += calc_packed_length_dataitem( data );
			if( !ret->deviceSpecific )
				ret->deviceSpecific = data;
			else
				add_dataitem( ret->deviceSpecific, data );
			
			if( next > packed + size )
				goto unpackerr;
		}
		
		next += strlen( DEVICE_END ) + 1;
	}
	
	if( strncmp( next, USER_START, strlen( USER_START ) ) == 0 )
	{
		next += strlen( USER_START ) + 1;
		if( next > packed + size )
			goto unpackerr;
		
		while( strncmp( next, USER_END, strlen( USER_END ) ) != 0 )
		{
			data = unpack_dataitem( next );
			if( !data )
				goto unpackerr;
			
			next += calc_packed_length_dataitem( data );
			if( !ret->userData )
				ret->userData = data;
			else
				add_dataitem( ret->userData, data );
			
			if( next > packed + size )
				goto unpackerr;
		}
		
		next += strlen( USER_END ) + 1;
	}
	
	if( strncmp( next, AX_START, strlen( AX_START ) ) == 0 )
	{
		next += strlen( AX_START ) + 1;
		if( next > packed + size )
			goto unpackerr;
		
		while( strncmp( next, AX_END, strlen( AX_END ) ) != 0 )
		{
			data = unpack_dataitem( next );
			if( !data )
				goto unpackerr;
			
			next += calc_packed_length_dataitem( data );
			if( !ret->aixNames )
				ret->aixNames = data;
			else
				add_dataitem( ret->aixNames, data );
			
			if( next > packed + size )
				goto unpackerr;
		}
	}
	
	return ret;
	
unpackerr:
	free_component( ret );
	return NULL;
}

void add_component( struct component *head, const struct component *addme )
{
	if( !head || !addme )
		return;
	
	while( head->next )
		head = head->next;
	head->next = addme;
}
