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
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <cstdlib>
#include <errno.h>

using namespace std;

namespace lsvpd
{
	const string VpdRetriever::DEFAULT_DIR  ( "/var/lib/lsvpd/" );
	const string VpdRetriever::DEFAULT_FILE ( "vpd.db" );
	const string VpdRetriever::UDEV_NOTIFY_FILE ( "run.vpdupdate" );

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
		struct stat vpd_stat,udev_stat;
		const string vpddb = VpdRetriever::DEFAULT_DIR + VpdRetriever::DEFAULT_FILE;
		const string udev_file = VpdRetriever::DEFAULT_DIR + VpdRetriever::UDEV_NOTIFY_FILE;
		Logger logger;
		int flag = 1;

		/* Check if stat is successful for UDEV_NOTIFY_FILE. */
		if ( stat ( udev_file.c_str(), &udev_stat ) != 0 )
		{
			logger.log( "libvpd: Unable to stat udev rule file, run.vpdupdate.", LOG_INFO );
		}
		else
		{
			/* Find the modification time for default DB. */
			if ( stat ( vpddb.c_str(), &vpd_stat ) != 0 )
			{
				logger.log( "libvpd: Unable to stat vpd.db file.", LOG_INFO );
				if ( errno == ENOENT )
					vpd_stat.st_mtime = 0;
				else
					flag = 0;
			}

			if ( flag && ( udev_stat.st_mtime > vpd_stat.st_mtime ) )
			{
				/*
				 * This implies there were changes to devices on the system
				 * after the VPD db was created/modified. So
				 * run vpdupdate.
				 */
				logger.log( "libvpd: Running vpdupdate to update the default db.", LOG_INFO );
				system( "vpdupdate >/dev/null 2>&1" );
			}
		}
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

