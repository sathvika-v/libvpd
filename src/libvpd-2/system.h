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

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <libvpd-2/common.h>
#include <libvpd-2/dataitem.h>
#include <libvpd-2/component.h>

#define SYS_INIT_BUF_SIZE 107
#define SYS_ID "/sys/bus"

struct system
{
	struct dataitem *id;
	struct dataitem *arch;
	struct dataitem *deviceTreeNode;
	struct dataitem *description;
	struct dataitem *brand;
	struct dataitem *nodeName;
	struct dataitem *os;
	struct dataitem *processorID;
	struct dataitem *machineType;
	struct dataitem *machineModel;
	struct dataitem *featureCode;
	struct dataitem *flagField;
	struct dataitem *recordType;
	struct dataitem *serialNum1;
	struct dataitem *serialNum2;
	struct dataitem *suid;
	struct dataitem *keywordVersion;
	struct dataitem *locationCode;
	u32 cpuCount;
	
	/*
	 * The list struct uses a void * for its payload.  All the data
	 * pointers in this list will be char*'s.
	 */
	struct list *childrenIDs;
	struct dataitem *deviceSpecific;
	struct dataitem *userData;
	struct component *children;
};

struct system * new_system( int init );
struct system * unpack_system( void * buffer );
void free_system( struct system *freeme );

#endif /*SYSTEM_H_*/
