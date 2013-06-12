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

#include <libvpd-2/vpdexception.hpp>

namespace lsvpd
{

	VpdException::VpdException( )
	{
		mMessage = string( );
	}

	VpdException::VpdException( const exception& e )
	{
		mMessage = string( e.what( ) );
	}

	VpdException::VpdException( const string& in ) : mMessage( in )
	{
	}

	VpdException::~VpdException( ) throw( )
	{
	}

	const char* VpdException::what( ) const throw( )
	{
		return mMessage.c_str( );
	}

	void VpdException::setMessage( const string& in )
	{
		mMessage = in;
	}

}

