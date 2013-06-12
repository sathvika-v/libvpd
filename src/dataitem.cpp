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

#ifdef HAVE_CONFIG_H
#include <libvpd-2/config.h>
#endif

#include <libvpd-2/dataitem.hpp>
#include <libvpd-2/debug.hpp>

#include <cstring> // for memcpy
#include <ctype.h> // for isspace

using namespace std;

namespace lsvpd
{
	DataItem::DataItem( const DataItem& copyMe ) : humanName( copyMe.humanName ),
		ac( copyMe.ac ), dataValue( copyMe.dataValue ),
		prefLevelUsed(copyMe.prefLevelUsed), packedLength( 0 )
	{}

	DataItem::DataItem( ) : packedLength( 0 ), dataValue( "" )
	{
		prefLevelUsed = 0;
	}

	DataItem::~DataItem( )
	{
		vector<Source*>::iterator i, end;
		for( i = sources.begin( ), end = sources.end( ); i != end; ++i )
		{
			Source* s = *i;
			delete s;
		}
	}

	int DataItem::getNumSources() const
	{
		return sources.size();
	}

	Source * DataItem::getSource(int i) const
	{
		if (i < getNumSources() || i == 0) {
			return sources[i];
		}
		else {
			return NULL;
		}
	}

	void DataItem::addSource(Source *in)
	{
		vector<Source*>::iterator i, end;
		for( i = sources.begin( ), end = sources.end( ); i != end; ++i )
		{
			if( (*i)->getPrefLvl( ) < in->getPrefLvl( ) )
			{
				sources.insert( i, in );
				return;
			}
		}

		sources.push_back( in );
	}

	void DataItem::removeSource(int i)
	{
		vector<Source*>::iterator cur, end;
		int j = 0;

		for( cur = sources.begin( ), end = sources.end( ); cur != end; ++cur, j++ )
		{
			if( j == i )
			{
				Source * s = *cur;
				sources.erase( cur );
				delete s;
				return;
			}
		}
	}

	const string& DataItem::getHumanName( ) const
	{
		return humanName;
	}

	void DataItem::setHumanName( const string& in )
	{
		humanName = in;
		packedLength = 0;
	}

	const string& DataItem::getAC( ) const
	{
		return ac;
	}

	void DataItem::setAC( const string& in )
	{
		ac = in;
		packedLength = 0;
	}

	const string& DataItem::getValue() const
	{
		return dataValue;
	}

	int DataItem::setValue( const string& in, int prefLevelUsed_t,
									 const char *file, int lineNum)
	{
		int i;

		if ((prefLevelUsed_t > prefLevelUsed) && (!in.empty()))
		{
			string val( in );
			// Get rid of any NULL bytes, new lines or carriage returns
			i = 0;
			int pos;
			while( ( pos = val.find( '\0' ) ) != string::npos ||
				( pos = val.find( '\n' ) ) != string::npos ||
				( pos = val.find( '\r' ) ) != string::npos )
			{
				val.erase( pos, 1 );
			}

			// Get rid of leading and trailing whitespace
			for( i = 0; i < val.length( ) && isspace( val.at( i ) ); i++ )
				val.erase( i, 1 );
			for( i = val.length( ) - 1; i >= 0 && isspace( val.at( i ) ); i-- )
				val.erase( i, 1 );

			/*
			if (ac == "AX") {
				coutd << "Attempting to set [" << ac << "]: " << endl
						<< "Request src: " << file<< ", line " << lineNum << endl
						<<" Old PrefLevel: " << prefLevelUsed << endl
						<<" New PrefLevel: " << prefLevelUsed_t << endl
						<< " Old dataVal: " << dataValue << endl
						<<" Attempting to set to dataVal to: '" << in << "'" << endl
						<< endl;
				coutd << "DataVal Set!  New Val: '" << val << "'" << endl;
			}
			*/
			/*
			if (humanName == "Device Bus") {
				coutd << "Attempting to set [" << ac << "]: " << endl
						<< "Request src: " << file<< ", line " << lineNum << endl
						<<" Old PrefLevel: " << prefLevelUsed << endl
						<<" New PrefLevel: " << prefLevelUsed_t << endl
						<< " Old dataVal: " << dataValue << endl
						<<" Attempting to set to dataVal to: '" << in << "'" << endl
						<< endl;
				coutd << "DataVal Set!  New Val: '" << val << "'" << endl;
			}
			*/
			dataValue = val;
			packedLength = 0;
			prefLevelUsed = prefLevelUsed_t;
			return 0;
		}
		else return -1;
	}

	int DataItem::getPrefLevel()
	{
		return prefLevelUsed;
	}

	/**
	 * Method reports the number of bytes that would be required to "serialize" (in the Java
	 * sense of the word) this object into a character buffer.  The result of the computation
	 * is stored to make multiple calls to this method move a little faster.  Each time an item
	 * is changed with a set method, the packedLength member is invalidated, forcing the method
	 * to sum the length again.
	 */
	int DataItem::getPackedLength( )
	{
		if( packedLength != 0 )
			return packedLength;

		int ret = 3; // 3 bytes is the smallest a packed DataItem can be.

		ret += ac.length( );
		ret += humanName.length( );
		ret += dataValue.length( );
		packedLength = ret;

		return ret;
	}

	/**
	 * Like the pack method from Component, this method takes the contents
	 * of this DataItem and packs them into the data buffer provided.  It will return
	 * the number of bytes packed into that buffer.  Each field is followed by a '\0'
	 * character to make unpacking easier.
	 */
	int DataItem::pack( void* buf )
	{
		char * buffer = (char*)buf;
		int tmp;
		int ret = getPackedLength( );

		memcpy( buffer, ac.c_str( ), ac.length( ) );
		buffer += ac.length( );

		*buffer = '\0';
		buffer++;

		memcpy( buffer, humanName.c_str( ), humanName.length( ) );
		buffer += humanName.length( );

		*buffer = '\0';
		buffer++;

		memcpy( buffer, dataValue.c_str( ), dataValue.length( ) );
		buffer += dataValue.length( );

		*buffer = '\0';
		buffer++;
		return ret;
	}

	/**
	 * Also like the like-named method within Component, this method loads this
	 * DataItem object from the provided data buffer.
	 */
	void DataItem::unpack( const void * data )
	{
		char * buf = (char*) data;
		ac = buf;
		buf += ( ac.length( ) + 1 );
		humanName = buf;
		buf += ( humanName.length( ) + 1 );
		dataValue = buf;
		int pos;
	}

	/*
	 * Prints this DataItem to the ostream in a meaningful way.
	 */
	/*
	ostream& operator<<( ostream& os, const DataItem& in )
	{
		os << " Human Name: " <<  in.getHumanName() << endl;
		os << "    Acronym: " << in.getAC() << endl;
		os << "      Value: " <<  in.getValue() << endl;

		return os;
	}
	*/
}
