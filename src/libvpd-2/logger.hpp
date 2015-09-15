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

/**
 * log_msg is a macro used to log message and priority to syslog
 */
#define log_msg(_PLEVEL_, _FORMAT_,...)					\
	L_DEFAULT_HANDLER(_PLEVEL_, "%s:%d: " _FORMAT_, __FILE__,	\
			  __LINE__, ##__VA_ARGS__)

/**
 * log_<X> calls log_msg with priority.
 * can be called as:
 *   log_info("Some Msg");
 *   log_info("Some Error Code %d\n", ret);
 *   or
 *   log_info("Some Error Code %d context:%p\n", ret, ptr);
 *
 * Sample o/p:
 *   lsvpd[11738]: src/vpddbenv.cpp:134: log log_message
 */
#define log_emerg(_FORMAT_, ...)  log_msg(LOG_EMERG, _FORMAT_, ##__VA_ARGS__)
#define log_alert(_FORMAT_, ...)  log_msg(LOG_ALERT, _FORMAT_, ##__VA_ARGS__)
#define log_crit(_FORMAT_, ...)   log_msg(LOG_CRIT, _FORMAT_, ##__VA_ARGS__)
#define log_err(_FORMAT_, ...)    log_msg(LOG_ERR, _FORMAT_, ##__VA_ARGS__)
#define log_warn(_FORMAT_, ...)   log_msg(LOG_WARNING, _FORMAT_, ##__VA_ARGS__)
#define log_notice(_FORMAT_, ...) log_msg(LOG_NOTICE, _FORMAT_, ##__VA_ARGS__)
#define log_info(_FORMAT_, ...)   log_msg(LOG_INFO, _FORMAT_, ##__VA_ARGS__)
#define log_debug(_FORMAT_, ...)  log_msg(LOG_DEBUG, _FORMAT_, ##__VA_ARGS__)

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

