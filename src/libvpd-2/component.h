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

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <libvpd-2/common.h>
#include <libvpd-2/dataitem.h>

#define CHILD_START  "::childrenStart::"
#define CHILD_END    "::childrenEnd::"
#define DEVICE_START "::deviceSpecificStart::"
#define DEVICE_END   "::deviceSpecificEnd::"
#define USER_START   "::userStart::"
#define USER_END     "::userEnd::"
#define AX_START     "::axStart::"
#define AX_END       "::axEnd::"
#define COMP_INIT_BUF_SIZE 129

/* All lists should be NULL terminated */
struct component
{
	struct dataitem *id;
	struct dataitem *deviceTreeNode;
	struct dataitem *sysFsNode;
	struct dataitem *sysFsLinkTarget;
	struct dataitem *halUDI;
	struct dataitem *netAddr;
	struct dataitem *devClass;
	struct dataitem *description;
	struct dataitem *cdField;
	struct dataitem *serialNumber;
	struct dataitem *partNumber;
	struct dataitem *firmwareLevel;
	struct dataitem *firmwareVersion;
	struct dataitem *fru;
	struct dataitem *manufacturer;
	struct dataitem *model;
	struct dataitem *manufacturerID;
	struct dataitem *engChangeLevel;
	struct dataitem *parent;
	struct dataitem *devSubSystem;
	struct dataitem *devDriver;
	struct dataitem *devKernel;
	struct dataitem *devKernelNumber;
	struct dataitem *devSysName;
	struct dataitem *devDevTreeName;
	struct dataitem *devBus;
	struct dataitem *devType;
	struct dataitem *devBusAddr;
	struct dataitem *microCodeImage;
	struct dataitem *recordType;
	struct dataitem *scsiDetail;
	struct dataitem *plantMfg;
	struct dataitem *featureCode;
	struct dataitem *keywordVersion;
	struct dataitem *microCodeVersion;
	struct dataitem *physicalLocation;
	struct dataitem *secondLocation;
	/* 
	 * The next three data items are singlely linked lists that will
	 * be NULL terminated.
	 */
	struct dataitem *deviceSpecific;
	struct dataitem *userData;
	struct dataitem *aixNames;
	
	struct list *childrenIDs;
	struct component *children;
	struct component *next;
};

struct component* new_component( int init );
void free_component( struct component *freeme );
struct component * unpack_component( void *buffer );
void add_component( struct component *head, const struct component *addme );

#endif /*COMPONENT_H_*/
