/***************************************************************************
 *   Copyright (C) 2006, IBM                                               *
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

#define TRACE_ON

#include <arpa/inet.h>
#include <netinet/in.h>

#include <libvpd-2/system.hpp>
#include <libvpd-2/lsvpd.hpp>
#include <libvpd-2/vpdexception.hpp>
#include <libvpd-2/debug.hpp>
#include <libvpd-2/logger.hpp>

#include <cstring>

namespace lsvpd
{
	string const System::CHILD_START   ( "::childrenStart::" );
	string const System::CHILD_END     ( "::childrenEnd::" );
	string const System::DEVICE_START  ( "::deviceSpecificStart::" );
	string const System::DEVICE_END    ( "::deviceSpecificEnd::" );
	string const System::USER_START    ( "::userStart::" );
	string const System::USER_END      ( "::userEnd::" );

	string const System::ID ( "/sys/bus" );

	System::System()
	{
		mIdNode.setValue( ID, 100, __FILE__, __LINE__ );

		mDescription.ac = "DS";
		mDescription.humanName = "Description";
		mBrand.ac = "BR";
		mBrand.humanName = "Brand Keyword";
		mOS.ac = "OS";
		mOS.humanName = "Operating System";
		mProcessorID.ac = "PI";
		mProcessorID.humanName = "Processor ID or unique ID";
		mMachineType.ac = "TM";
		mMachineType.humanName = "Machine Type";
		mMachineModel.ac = "TM";
		mMachineModel.humanName = "Machine Model";
		mFeatureCode.ac = "FC";
		mFeatureCode.humanName = "Feature Code";
		mFlagField.ac = "FG";
		mFlagField.humanName = "Flag Field";
		mRecordType.ac = "RT";
		mRecordType.humanName = "Record Type";
		mSerialNum1.ac = "SE";
		mSerialNum1.humanName = "Machine or Cabinet Serial Number";
		mSerialNum2.ac = "SE";
		mSerialNum2.humanName = "Machine or Cabinet Serial Number";
		mSUID.ac = "SU";
		mSUID.humanName = "System Unique ID";
		mKeywordVersion.ac = "VK";
		mKeywordVersion.humanName = "Version of Keywords";
		mLocationCode.ac = "YL";
		mLocationCode.humanName = "Location Code";
		mArch = "";
		mCPUCount = 1;

		mDeviceSpecific = vector<DataItem*>( );
		mUserData = vector<DataItem*>( );
		mChildren = vector<string>( );
		mLeaves = vector<Component*>( );
	}

	System::System( const void* packedData )
	{
		unpack( packedData );
		mIdNode.setValue( ID, 100, __FILE__, __LINE__ );
		mLeaves = vector<Component*>( );
	}

	System::~System( )
	{
		vector<Component*>::iterator i, end = mLeaves.end( );
		for( i = mLeaves.begin( ); i != end; ++i )
		{
			delete (*i);
		}

		vector<DataItem*>::iterator j, dEnd;
		for( j = mDeviceSpecific.begin( ), dEnd = mDeviceSpecific.end( ); j != dEnd; ++j )
		{
			delete (*j);
		}
	}

	unsigned int System::getPackedSize( )
	{
		unsigned int ret = INIT_BUF_SIZE;

		ret += ( sizeof( u32 ) * 2 );
		ret += mIdNode.getPackedLength( );
		ret += mArch.getPackedLength( );
		ret += deviceTreeNode.getPackedLength( );
		ret += mDescription.getPackedLength( );
		ret += mBrand.getPackedLength( );
		ret += mNodeName.getPackedLength( );
		ret += mOS.getPackedLength( );
		ret += mProcessorID.getPackedLength( );
		ret += mMachineType.getPackedLength( );
		ret += mMachineModel.getPackedLength( );
		ret += mFeatureCode.getPackedLength( );
		ret += mFlagField.getPackedLength( );
		ret += mRecordType.getPackedLength( );
		ret += mSerialNum1.getPackedLength( );
		ret += mSerialNum2.getPackedLength( );
		ret += mSUID.getPackedLength( );
		ret += mKeywordVersion.getPackedLength( );
		ret += mLocationCode.getPackedLength( );

		ret += mDeviceSpecific.size( );
		ret += mUserData.size( );
		ret += mChildren.size( );
		vector<string>::iterator child, cEnd;
		vector<DataItem*>::iterator item, dEnd;
		for( child = mChildren.begin( ), cEnd = mChildren.end( ); child != cEnd; ++child )
		{
			ret += (*child).length( );
		}

		for( item = mDeviceSpecific.begin( ), dEnd = mDeviceSpecific.end( ); item != dEnd;
				   ++item )
		{
			ret += (*item)->getPackedLength( );
		}

		for( item = mUserData.begin( ), dEnd = mUserData.end( ); item != dEnd;
				   ++item )
		{
			ret += (*item)->getPackedLength( );
		}

		return ret;
	}

	/**
	 * pack serializes this object into the provided buffer (pack will allocate the
	 * buffer) storing only the data fields (at the moment) and ignoring all the meta-
	 * data within the DataItem object.  The format for the packed object will be
	 * direct strings seperated by '\0' charaters.  If a given string field is empty
	 * then there will be consectutive '\0' characters.  Each of the vector fields will
	 * be preceeded by ::listNameStart:: and followed by ::listNameEnd:: (e.g. the
	 * children vector will be ::childrenStart::\0data\0data\0::childrenEnd::\0).
	 */
	unsigned int System::pack( void** buffer )
	{
		u32 ret = getPackedSize( ), length;
		char* buf;

		buf = new char[ ret ];

		if( buf == NULL )
		{
			string message( "Component.pack( ): new call failed, out of memory." );
			Logger l;
			l.log( message, LOG_ERR );
			VpdException ve( message );
			throw ve;
		}

		memset( buf, '\0', ret );

		*buffer = (void*)buf;

		/* -----------------------------------------------------*/
		/* ------------ Load up the buffer with data -----------*/
		/* -----------------------------------------------------*/

		// The first entry in our buffer is our length in network byte order
		u32 netOrder = htonl( ret );
		memcpy( buf, &netOrder, sizeof( u32 ) );
		buf += sizeof( u32 );
		// Next is our system CPU count.
		netOrder = htonl( mCPUCount );
		memcpy( buf, &netOrder, sizeof( u32 ) );
		buf += sizeof( u32 );
		// Pack the individual data items.
		length = mIdNode.pack( buf );
		buf += length;
		length = mArch.pack( buf );
		buf += length;
		length = deviceTreeNode.pack( buf );
		buf += length;
		length = mDescription.pack( buf );
		buf += length;
		length = mBrand.pack( buf );
		buf += length;
		length = mNodeName.pack( buf );
		buf += length;
		length = mOS.pack( buf );
		buf += length;
		length = mProcessorID.pack( buf );
		buf += length;
		length = mMachineType.pack( buf );
		buf += length;
		length = mMachineModel.pack( buf );
		buf += length;
		length = mFeatureCode.pack( buf );
		buf += length;
		length = mFlagField.pack( buf );
		buf += length;
		length = mRecordType.pack( buf );
		buf += length;
		length = mSerialNum1.pack( buf );
		buf += length;
		length = mSerialNum2.pack( buf );
		buf += length;
		length = mSUID.pack( buf );
		buf += length;
		length = mKeywordVersion.pack( buf );
		buf += length;
		length = mLocationCode.pack( buf );
		buf += length;

		// Pack the child vector.
		memcpy( buf, CHILD_START.c_str( ), CHILD_START.length( ) );
		buf += CHILD_START.length( );
		*buf = '\0';
		buf++;

		vector<string>::iterator child, cEnd;
		vector<DataItem*>::iterator item, dEnd;
		for( child = mChildren.begin( ), cEnd = mChildren.end( ); child != cEnd; ++child )
		{
			const char* str = (*child).c_str( );
			int length = (*child).length( );
			memcpy( buf, str, length );
			buf+= length;
			*buf = '\0';
			buf++;
		}
		memcpy( buf, CHILD_END.c_str( ), CHILD_END.length( ) );
		buf += CHILD_END.length( );
		*buf = '\0';
		buf++;

		// Pack the Device Specific vector
		memcpy( buf, DEVICE_START.c_str( ), DEVICE_START.length( ) );
		buf += DEVICE_START.length( );
		*buf = '\0';
		buf++;
		for( item = mDeviceSpecific.begin( ), dEnd = mDeviceSpecific.end( ); item != dEnd;
				   ++item )
		{
			length = (*item)->pack( buf );
			buf+= length;
		}
		memcpy( buf, DEVICE_END.c_str( ), DEVICE_END.length( ) );
		buf += DEVICE_END.length( );
		*buf = '\0';
		buf++;

		// Pack the User Data vector
		memcpy( buf, USER_START.c_str( ), USER_START.length( ) );
		buf += USER_START.length( );
		*buf = '\0';
		buf++;
		for( item = mUserData.begin( ), dEnd = mUserData.end( ); item != dEnd;
				   ++item )
		{
			length = (*item)->pack( buf );
			buf+= length;
		}
		memcpy( buf, USER_END.c_str( ), USER_END.length( ) );
		buf += USER_END.length( );
		*buf = '\0';
		buf++;

		return ret;
	}

	/**
	 * unpack does exactly the opposite from pack, it will take a data buffer
	 * and load this object with the contents.  If we need more data than the
	 * size claims that we have, then we have a corrupt buffer and we will
	 * throw an exception.
	 */
	void System::unpack( const void* payload )
	{
		u32 size = 0, netOrder;
		char* packed = (char*) payload;
		char* next = packed;;
		string item;

		if( payload == NULL )
		{
			// Rather than throwing an exception, this will just return, if NULL is handed in
			// this far there is nothing that we can do to stop program instability other
			// than refuse to mess with it.
			return;
		}

		// Load the size of the payload. (It is packed in network order)
		memcpy( &netOrder, next, sizeof( u32 ) );
		size = ntohl( netOrder );
		next += sizeof( u32 );

		memcpy( &netOrder, next, sizeof( u32 ) );
		mCPUCount = ntohl( netOrder );
		next += sizeof( u32 );

		mChildren = vector<string>( );
		mDeviceSpecific = vector<DataItem*>( );

		mIdNode = DataItem( next );
		next += mIdNode.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mArch = DataItem( next );
		next += mArch.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		deviceTreeNode = DataItem( next );
		next += deviceTreeNode.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mDescription = DataItem( next );
		next += mDescription.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mBrand = DataItem( next );
		next += mBrand.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mNodeName = DataItem( next );
		next += mNodeName.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mOS = DataItem( next );
		next += mOS.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mProcessorID = DataItem( next );
		next += mProcessorID.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mMachineType = DataItem( next );
		next += mMachineType.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mMachineModel = DataItem( next );
		next += mMachineModel.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mFeatureCode = DataItem( next );
		next += mFeatureCode.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mFlagField = DataItem( next );
		next += mFlagField.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mRecordType = DataItem( next );
		next += mRecordType.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mSerialNum1 = DataItem( next );
		next += mSerialNum1.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mSerialNum2 = DataItem( next );
		next += mSerialNum2.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mSUID = DataItem( next );
		next += mSUID.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mKeywordVersion = DataItem( next );
		next += mKeywordVersion.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}
		mLocationCode = DataItem( next );
		next += mLocationCode.getPackedLength( );
		if( next > packed + size )
		{
			goto lderror;
		}

		// Now onto the vectors, these should go much the same way that the packing
		// did.
		if( next == CHILD_START )
		{
			next += CHILD_START.length( ) + 1;
			if( next > packed + size )
			{
				goto lderror;
			}
			item = string( (char*)next );
			while( CHILD_END != item )
			{
				if( item != string( "" ) )
				{
					string child = string( (char*)next );
					mChildren.push_back( child );
					next += item.length( );
				}
				next++;
				item = string( (char*)next );
			}
			next += CHILD_END.length( ) + 1;
		}

		if( DEVICE_START == next )
		{
			next += DEVICE_START.length( ) + 1;
			if( next > packed + size )
			{
				goto lderror;
			}
			item = string( (char*) next );
			while( DEVICE_END != item )
			{
				DataItem* d = new DataItem( next );
				next += d->getPackedLength( );
				if( next > packed + size )
				{
					goto lderror;
				}
				mDeviceSpecific.push_back( d );
				item = string( (char*)next );
			}
			next += DEVICE_END.length( ) + 1;
		}

		if( USER_START == next )
		{
			next += USER_START.length( ) + 1;
			if( next > packed + size )
			{
				goto lderror;
			}
			item = string( (char*) next );
			while( USER_END != item )
			{
				DataItem* d = new DataItem( next );
				next += d->getPackedLength( );
				if( next > packed + size )
				{
					goto lderror;
				}
				mDeviceSpecific.push_back( d );
				item = string( (char*)next );

			}
		}
		return;

lderror:
		string message( "Component.unpack( ): Attempting to unpack corrupt buffer." );
		Logger l;
		l.log( message, LOG_ERR );
		VpdException ve( message );
		throw ve;
	}

	void System::addDeviceSpecific( const string& ac, const string& humanName,
										const string& val, int lvl = 0 )
	{
		DataItem *d = new DataItem( );
		if( d == NULL )
		{
			VpdException ve( "Out of memory." );
			throw ve;
		}
		d->ac = ac;
		d->humanName = humanName;
		d->setValue( val, lvl, __FILE__, __LINE__ );
		mDeviceSpecific.push_back( d );
	}

	void System::addUserData( const string& ac, const string& humanName,
								const string& val, int prefLvl = 1,
								bool clobber = false )
	{
		vector<DataItem*>::iterator i, end;
		for( i = mUserData.begin( ), end = mUserData.end( ); i != end; ++i )
		{
			if( (*i)->ac == ac )
			{
				if( clobber )
				{
					(*i)->dataValue = val;
				}
				return;
			}
		}
		DataItem *d = new DataItem( );
		if( d == NULL )
		{
			VpdException ve( "Out of memory." );
			throw ve;
		}

		d->ac = ac;
		d->humanName = humanName;
		d->setValue( val, prefLvl, __FILE__, __LINE__ );
		mUserData.push_back( d );
	}

	/**
	 * Adds this Component to the output stream in a meaningful way.
	 */
	/*
	ostream& operator<<( ostream& os, const System& in )
	{
		os << "ID:" << endl << in.mIdNode;
		os << "Description:" << endl << in.mDescription;
		os << "Brand:" << endl << in.mBrand;
		os << "Node Name:" << endl << in.mNodeName;
		os << "OS:" << endl << in.mOS;
		os << "Processor ID:" << endl << in.mProcessorID;
		os << "Machine Type:" << endl << in.mMachineType;
		os << "Machine Model#:" << endl << in.mMachineModel;
		os << "Feature Code:" << endl << in.mFeatureCode;
		os << "Flag Field:" << endl << in.mFlagField;
		os << "Record Type:" << endl << in.mRecordType;
		os << "Serial Number 1:" << endl << in.mSerialNum1;
		os << "Serial Number 2:" << endl << in.mSerialNum2;
		os << "Unique ID:" << endl << in.mSUID;
		os << "Keyword Version:" << endl << in.mKeywordVersion;
		os << "Location Code:" << endl << in.mLocationCode;

		vector<DataItem*>::const_iterator i, end;
		vector<string>::const_iterator j, back;

		os << System::DEVICE_START << endl;

		for( i = in.mDeviceSpecific.begin( ), end = in.mDeviceSpecific.end( ); i != end; ++i )
		{
			os << "Device Specific:" << endl << **i;
		}

		os << System::DEVICE_END << endl
			<< System::CHILD_START << endl;

		for( j = in.mChildren.begin( ), back = in.mChildren.end( ); j != back; ++j )
		{
			os << *j << endl;
		}
		os << System::CHILD_END << endl;

		return os;
	}
	*/

	/**
	 * In-memory tree representation methods
	 */
	bool System::removeLeaf( int index )
	{
		vector<Component*>::iterator i = mLeaves.begin( );
		if( index > mLeaves.size( ) )
		{
			return false;
		}

		for( int j = 0; j < index; j++ )
		{
			i++;
		}
		mLeaves.erase( i );
		return true;
	}

	bool System::removeLeaf( const string& in )
	{
		vector<Component*>::iterator i = mLeaves.begin( );

		for( ; i != mLeaves.end( ); ++i )
		{
			if( (*i)->getID( ) == in )
			{
				mLeaves.erase( i );
				return true;
			}
		}
		return false;
	}

	void System::removeChild( const string& id )
	{
		vector<string>::iterator i, end;

		for( i = mChildren.begin( ), end = mChildren.end( ); i != end; ++i )
		{
			if( id == *i )
			{
				mChildren.erase(i);
				return;
			}
		}
	}
}
