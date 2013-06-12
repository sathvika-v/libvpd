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

#include <libvpd-2/vpddbenv.hpp>
#include <libvpd-2/vpdretriever.hpp>
#include <libvpd-2/logger.hpp>

#include <vector>
#include <string>

using namespace std;

namespace lsvpd
{
	const string VpdRetriever::DEFAULT_DIR  ( "/var/lib/lsvpd/" );
	const string VpdRetriever::DEFAULT_FILE ( "vpd.db" );

	VpdRetriever::VpdRetriever( string envDir,
		string dbFileName ) throw( VpdException& )
	{
		db = new VpdDbEnv( envDir, dbFileName, true );

		if( db == NULL )
		{
			Logger logger;
			logger.log( "Out of memory, failed to build VpdEnv.", LOG_ERR );
			VpdException ve( "Out of memory, failed to build VpdEnv." );
			throw ve;
		}
	}
	
	VpdRetriever::VpdRetriever( ) throw( VpdException& )
	{
		db = new VpdDbEnv( VpdRetriever::DEFAULT_DIR,
					VpdRetriever::DEFAULT_FILE, true );

		if( db == NULL )
		{
			Logger logger;
			logger.log( "Out of memory, failed to build VpdEnv.", LOG_ERR );
			VpdException ve( "Out of memory, failed to build VpdEnv." );
			throw ve;
		}
	}

	VpdRetriever::~VpdRetriever( )
	{
		if( db != NULL )
		{
			delete db;
		}
	}

	System* VpdRetriever::getComponentTree( )
	{
		System *root = db->fetch( );
		if (root)
			buildSubTree( root );
		else
		{
			Logger logger;
			logger.log( "Failed to fetch VPD DB, it may be corrupt.", LOG_ERR );
			VpdException ve( "Failed to fetch VPD DB, it may be corrupt." );
			throw ve;
		}

		return root;
	}

	void VpdRetriever::buildSubTree( System* root )
	{
		Component* leaf;
		const vector<string> children = root->getChildren( );
		vector<string>::const_iterator i, end;

		end = children.end( );

		for( i = children.begin( ); i != end; ++i )
		{
			const string next = *i;
			leaf = db->fetch( next );
			if( leaf == NULL )
			{
				Logger logger;
				logger.log( "Failed to fetch requested item.", LOG_ERR );
				VpdException ve( "Failed to fetch requested item." );
				throw ve;
			}
			buildSubTree( leaf );
			root->addLeaf( leaf );
		}
	}

	void VpdRetriever::buildSubTree( Component* root )
	{
		Component* leaf;
		const vector<string> children = root->getChildren( );
		vector<string>::const_iterator i, end;

		end = children.end( );

		for( i = children.begin( ); i != end; ++i )
		{
			const string next = *i;
			leaf = db->fetch( next );
			if( leaf == NULL )
			{
				Logger logger;
				logger.log( "Failed to fetch requested item.", LOG_ERR );
				VpdException ve( "Failed to fetch requested item." );
				throw ve;
			}
			buildSubTree( leaf );
			root->addLeaf( leaf );
		}
	}
}

