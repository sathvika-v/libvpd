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

#ifndef LSVPDLOGGER_HPP
#define LSVPDLOGGER_HPP

#include <string>

#include <syslog.h>
#define L_DEFAULT_HANDLER syslog // replaces fprintf(3) by syslog(3)
#define L_DEFAULT_PARAMS LOG_USER // default priority for syslog info

using namespace std;

namespace lsvpd
{

	class Logger
	{
		public:
			/*
			 * Log level will use the constants defined in syslog.h see
			 * syslog(3) for their meanings.
			 */

			Logger( const string& ident = "lsvpd" );

			~Logger();

			inline const string& getIdent( ) const { return mIdent; }

			/**
			 * Sends the specified message to syslogd with the specified
			 * severity level.  The severity level defaults to LOG_NOTICE if
			 * nothing is specified.
			 */
			void log( const string& message, int level = LOG_NOTICE ) const;

		private:
			string mIdent;
	};

}

#endif //LSVPDLOGGER_H

