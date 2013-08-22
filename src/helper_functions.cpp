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

#include <libvpd-2/helper_functions.hpp>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <fstream>


#define BUF_SIZE 512

using namespace std;
using namespace lsvpd;

	/**
	 * findAIXFSEntry
	 * @brief Finds AIX name which exists as a file or link in the dir 
	 * 	'rootPath'
	 */
	string HelperFunctions::findAIXFSEntry(vector <DataItem*> aixNames,
											const string& rootPath)
	{
		string fin;
		int fd;
		vector<DataItem*>::const_iterator i, end;
		i = aixNames.begin();
		end = aixNames.end();
		
		while (i != end) {
			fin = rootPath + (*i)->getValue();
			fd = open( fin.c_str( ), O_RDONLY | O_NONBLOCK );
			if( fd < 0 )
				i++;
			else {
				close(fd);
				return fin;
			}
		}
		return string("");
	}

/**
 * parseString
 * @brief Parses out strings based on a count of position [0...x],
 *  with beginning and end of each string given by a '"'.
 * Ex: '"string 0" ... "string 1" ... "string 2" ...
 */
string HelperFunctions::parseString(const string& line, int str_pos, string& out)
{
	int count = 0, pos = 0;
	int beg, len;

	do {
		beg = line.find('"', pos) + 1;
		pos = line.find('"', beg) + 1;
		len = pos - beg;
		count++;
		} while  ((pos != string::npos) && (count < str_pos));

	out = line.substr(beg, len - 1);
	return out;
}
	
	/**
	 * parsePathr
	 * @brief Reverse parse of a path, returning the node specified by 'count'
	 * @param count 0 is the tail node, 1 is 1 in, etc. 
	 * @example path = /sys/devices/x/y/z, 
	 * @ex count = 0, returns z
	 *  count = 1, returns y
	 *  count = 2, returns x
	 */
	string HelperFunctions::parsePathr(const string& path, int count)
	{
		int beg, end;
		int i = count;
		
		if( path == "" )
			return path;
		
		beg = end = path.length();
		
		if (path[end] == '/')
			beg--;
		
		cout << "String: " << path << endl;
		
		while (i >= 0) {
			end = beg - 1;
			cout << "RFind Bef: beg: " << beg << ", end: " << end << ", i: " << i << endl;
			beg = path.rfind("/", end - 1) + 1;
			cout << "RFind : beg: " << beg << ", end: " << end << ", i: " << i << endl;
			i--;
		}
		
		return path.substr(beg, end - beg);
	}
	
	/**
	 * parsePath
	 * @brief Parse of a path, returning the node specified by 'count'
	 * @param count 0 is the first node, 1 is 1 in, etc. 
	 * @example path = /sys/devices/x/y/z, 
	 * @ex count = 0, returns sys
	 *  count = 1, returns devices
	 *  count = 2, returns x
	 */
	string HelperFunctions::parsePath(const string& path, int count)
	{
		int beg = 0, end, i = count;
		
		if( path == "" )
			return path;
		
		if (path[end] == '/')
			end--;
		
		while (i >= 0) {
			beg = path.find("/", beg) + 1;
			i--;
		}
		
		end = path.find("/", beg);
		
		return path.substr(beg, end - beg);
	}
	
	/**
	 * getSymLinkTarget
	 * @brief Returns the absolute path pointed at by a sym link
	 * @param full path of sym link
	 * @return String abs target
	 */
	string HelperFunctions::getSymLinkTarget(const string& symLinkPath)
	{
		char linkTarget[BUF_SIZE], *buf;
		
		buf = strdup(symLinkPath.c_str());
		int len = readlink(buf, linkTarget, sizeof(
			linkTarget));
		linkTarget[len] = '\0';
		return string(getAbsolutePath(linkTarget, buf));
	}

/* Drops one dir entry on the end of a path
	 * @return Returns number of dirs successfully dropped*/
	int HelperFunctions::dropDir(char * filePath)
	{
		int len = strlen(filePath);
		int i, ret = 0;

		if (len <= 0)
			return 0;

		/* erase filenames */
		i = len;
		while (i > 0 && filePath[i] != '/')
			filePath[i--] = '\0';

		filePath[len-1] = '\0'; //erase trailing '/'

		i = len;
		while (i > 0 && filePath[i] != '/') {
			filePath[i--] = '\0';
			ret = 1;
		}

		return ret;
	}

	/* Drops one dir entry on the end of a path
	 * @return Returns number of dirs successfully dropped*/
	int HelperFunctions::dropDir(string& filePath_t)
	{
		int len = filePath_t.length();
		int i, ret = 0;
		char *filePath;

		filePath = strdup(filePath_t.c_str());

		if (len <= 0) {
			free(filePath);
			return 0;
		}

		/* erase filenames */
		i = len;
		while (i > 0 && filePath[i] != '/')
			filePath[i--] = '\0';

		filePath[len-1] = '\0'; //erase trailing '/'

		i = len;
		while (i > 0 && filePath[i] != '/') {
			filePath[i--] = '\0';
			ret = 1;
		}
		filePath[i--] = '\0';

		filePath_t = string(filePath);
		free(filePath);
		return ret;
	}
	
	/* @brief Drops dirs from curDir, 1 for each '../', then add remainder of filePath
	 * to curDir */
	string HelperFunctions::getAbsolutePath(char * relPath, char * curDir)
	{
		int len = strlen(relPath);
		int i = strlen(curDir);
		int lastHit;
		string tmp;

        if (len <= 0 || i <= 0)
        	return string();

		while (i > 0 && curDir[i] != '/')
			curDir[i--] = '\0';

		i = 0;
		while (strncmp(&relPath[i * 3], "../", 3) == 0) {
        	dropDir(curDir);
        	lastHit = i;
        	i++;
        }

        tmp = string(curDir) + string(&relPath[i * 3]);

        return tmp;
	}
	
	/**
	 * Takes two strings and finds if the two are equivalent.
	 * s1 can contain '*', meaning zero or more of anything can be
	 * counted as a match
	 */
	bool HelperFunctions::matches(const string& s1, const string& s2)
	{
		int beg = 0, end = s1.length();
		bool star = false;
		int z;
//		coutd << " s1 = " << s1 << ", s2 = " << s2 << endl;

		//strings have matched to end - base case
		if ((s1 == s2) || (s1.length() == 0) || (s2.length() == 0)) {
			return (s1 == s2);
		}

		//s1 has matched all the way up to a * - base case
		if (s1 == "*") {
			return true;
		}

		if (s1[0] == '*') {
			/*
			 * Need to grab a substring of s2, up to size of s1 from
			 * star to any other star
			 */
			beg = 1;
			end = s1.length();

			if ( string::npos == (end = s1.find("*", beg))) {
				//No more stars - base case
				if (string::npos != (z = s2.find(s1.substr(beg, end), 0))) {
					return true;
				}
			}
			else {
				if (string::npos != s2.find(s1.substr(beg, end), 0))
					matches(s1.substr(end, s1.length() - end),
							s2.substr(end, s2.length() - end));
			}
		}
		else
			end = s1.find("*", 0);

		if (s1.substr(beg, end) == s2.substr(beg, end)) {
			return matches(s1.substr(end, s1.length() - end),
						s2.substr(end, s2.length() - end));
		}
		else {
			return false;
		}
	}

	/* Caller responsible for deleting new'd memory */
	void HelperFunctions::str2chr(char **str1, const string& str2)
		{
			int i;
			*str1 = new char[ str2.length( ) * sizeof( char ) ];

			i=0;
			while (i < str2.length())
			{
				(*str1)[i] = str2[i];
				i++;
			}

			(*str1)[i] = '\0';
		}

	/* @brief Ensure a passed in fs path arg is in reasonable format */
	void HelperFunctions::fs_fixPath(string& str)
	{
		int i = str.length();

		while (str[i] == '\0')
			i--;
		if (str[i] == '/')
			str[i] = '\0';
	}

	/* @brief Ensure a passed in fs path arg is in reasonable format */
	char *HelperFunctions::fs_fixPath(char *path_t)
	{
		char *path;
		char *chr;
		int i;

		path = strdup(path_t);
		/* Basic wierdness checking*/
			chr = &path[strlen(path)];
			while (chr != '\0')
				chr--;
			if (*chr == '/')
				*chr = '\0';

		return path;
	}
	
	bool HelperFunctions::file_exists(const string& file)
	{
		FILE *f = fopen(file.c_str(), "r");
		if (!f) return false;
		fclose(f);
		return true;
	}

/** readMatchFromFile
 *  @brief Returns first matching line from specified file, 
 *    in which was found a match to the specified string
*/ 
string HelperFunctions::readMatchFromFile(const string& file,
											const string& str)
{
	char buf[BUF_SIZE];
	string line;
	
	if (file_exists(file)) {
		ifstream fin(file.c_str());
		
		while(!fin.eof()) {
			fin.getline(buf,BUF_SIZE - 1);
			line = string(buf);
			int loc = line.find(str);
			if (loc == 0) {
				return line;
			}
		}
		return string("");
	}
	return string("");
}


	int HelperFunctions::countChar( const string& str, char c )
	{
		string::const_iterator i, end;
		int ret = 0;
		for( i = str.begin( ), end = str.end( ); i != end; ++i )
		{
			if( (*i) == c )
			{
				ret++;
			}
		}
		return ret;
	}

	bool HelperFunctions::contains( const vector<DataItem*>& vec,
		const string& val )
	{
		vector<DataItem*>::const_iterator i, end;
		for( i = vec.begin( ), end = vec.end( ); i != end; ++i )
		{
			if( (*i)->getValue( ) == val )
				return true;
		}

		return false;
	}

	int HelperFunctions::execCmd( const char *cmd, string& output )
	{
		char buf[BUF_SIZE];
		FILE *fp = popen(cmd, "r");

		if (fp == NULL)
			return 1;

		while ( !feof(fp) )
			if (fgets(buf, BUF_SIZE - 1, fp) != NULL)
				output += buf;
		pclose(fp);
		return 0;
	}
