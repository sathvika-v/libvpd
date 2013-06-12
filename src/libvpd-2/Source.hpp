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

#ifndef LSVPDSOURCE_HPP
#define LSVPDSOURCE_HPP

/* Collector Source data types */
enum {
	BINARY = 0,
	ASCII,
	UNUSED
};

/* Various data sources/collectors */
enum {
	SRC_DEVICETREE = 0,
	SRC_SYSFS
};

#include <string>
#include <iostream>

using namespace std;

namespace lsvpd
{
	class Source
	{
		private:

			/*
			 * Identifier needed to access this source.  May be a file to open,
			 * mem pointer, unique ID, etc
			 */
			string srcRef;

			// Optional: Used for various purposes as needed by this source
			string data;

			/*
			 * Optional: reference to where this data field resides - ie
			 * PCIIDS, SYSFS, etc
			 */
			int id;

			// binary file? ascii?
			int type;

			/*
			 * How many lines need to be read from this file to collect all
			 * needed data
			 */
			int lines;

			/*
			 * If prefLevel is set higher than other sources, values obtained
			 * from this source will be used to overwrite values from other
			 * sources
			 */
			int preferenceLevel;

		public:

			Source(string srcRef_t, string data_t, int id_t,
					int type_t, int lines_t, int preferenceLevel_t)
						:
						srcRef(srcRef_t),
						data(data_t),
						id(id_t),
						type(type_t),
						lines(lines_t),
						preferenceLevel(preferenceLevel_t)
						{}
			Source() : srcRef( "" )
			{}
			~Source() {}

			inline int getPrefLvl( ) const { return preferenceLevel; }
			inline const string& getSrcRef( ) const { return srcRef; }
			inline const string& getData( ) const { return data; }
			inline int getID( ) const { return id; }
			inline int getType( ) const { return type; }
			inline int getLines( ) const { return lines; }

			void unpack(char **buf);
			int getPackedLength();
			void pack(char **buf);

			friend ostream& operator<<( ostream& os, const Source& in );

	};
}
#endif /*LSVPDSOURCE_H_*/
