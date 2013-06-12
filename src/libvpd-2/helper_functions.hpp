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

#include <string>
#include <vector>

#include <libvpd-2/dataitem.hpp>

#ifndef HELPERFUNCTIONS_HPP
#define HELPERFUNCTIONS_HPP

/* Collection of definitions needed globally */
using namespace std;

namespace lsvpd {
	class HelperFunctions {
		private:
			HelperFunctions();
			~HelperFunctions();

		public:
			static string readMatchFromFile(const string&, const string&);
			static string parseString(const string& line, int str_pos, string& out);
			static string findAIXFSEntry(vector <DataItem*>, const string&);
			static string parsePathr(const string& path, int count);
			static string parsePath(const string& path, int count);
			static string getSymLinkTarget(const string&);
			static int dropDir(char * filePath);
			static int dropDir(string& filePath_t);
			static string getAbsolutePath(char * relPath, char * curDir);
			static bool matches(const string& s1, const string& s2);
			/* Remove trailing '/', fix anything else mucked up with path */
			static char *fs_fixPath(char *path_t);
			static void fs_fixPath(string& str);
			
			static bool file_exists(const string& file);

			static void str2chr(char **str1, const string& str2);

			/* Counts and returns the occurences of c in str. */
			static int countChar( const string& str, char c );

			static bool contains( const vector<DataItem*>& vec,
				const string& val );
			static int execCmd( const char *cmd, string& output );
	};
}

#endif /*HELPERFUNCTIONS_H_*/
