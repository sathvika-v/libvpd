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
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <sstream>
#include <sys/wait.h>
#include <linux/limits.h>


#define BUF_SIZE PATH_MAX

using namespace std;
using namespace lsvpd;

static int process_child(char *argv[], int pipefd[])
{
	int     nullfd;

	close(pipefd[0]);

	/* stderr to /dev/null redirection */
	nullfd = open("/dev/null", O_WRONLY);
	if (nullfd == -1) {
		fprintf(stderr, "%s : %d - failed to open "
				"\'/dev/null\' for redirection : %s\n",
				__func__, __LINE__, strerror(errno));
		close(pipefd[1]);
		return -1;
	}

	/* redirect stdout to write-end of the pipe */
	if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
		fprintf(stderr, "%s : %d - failed to redirect "
				"pipe write fd to stdout : %s\n",
				__func__, __LINE__, strerror(errno));
		goto err_out;
	}

	if (dup2(nullfd, STDERR_FILENO) == -1) {
		fprintf(stderr, "%s : %d - failed to redirect "
				"\'/dev/null\' to stderr : %s\n",
				__func__, __LINE__, strerror(errno));
		goto err_out;
	}

	execve(argv[0], argv, NULL);
	/* some failure in exec */

err_out:
	close(pipefd[1]);
	close(nullfd);
	return -1;
}

/*
 * This function mimics popen(3).
 *
 * Returns:
 *   NULL, if fork(2), pipe(2) and dup2(3) calls fail
 *
 * Note:
 *   fclose(3) function shouldn't be used to close the stream
 *   returned here, since it doesn't wait for the child to exit.
 */
FILE *spopen(char *argv[], pid_t *ppid)
{
	FILE    *fp = NULL;
	int     pipefd[2];
	pid_t   cpid;

	if (argv == NULL)
		return fp;

	if (access(argv[0], F_OK|X_OK) != 0) {
		fprintf(stderr, "%s : The command \"%s\" is not executable.\n",
			__func__, argv[0]);
		return fp;
	}

	if (pipe(pipefd) == -1) {
		fprintf(stderr, "%s : %d - failed in pipe(), error : %s\n",
			__func__, __LINE__, strerror(errno));
		return NULL;
	}

	cpid = fork();
	switch (cpid) {
	case -1:
		/* Still in parent; Failure in fork() */
		fprintf(stderr, "%s : %d - fork() failed, error : %s\n",
			__func__, __LINE__, strerror(errno));
		close(pipefd[0]);
		close(pipefd[1]);
		return NULL;
	case  0: /* Code executed by child */
		if (process_child(argv, pipefd) == -1) {
			fprintf(stderr, "%s : %d - Error occured while "
					"processing write end of the pipe "
					"(in child).", __func__, __LINE__);
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	default: /* Code executed by parent */
		/* store the child pid for pclose() */
		*ppid = cpid;

		close(pipefd[1]);

		fp = fdopen(pipefd[0], "r");
		if (fp == NULL) {
			fprintf(stderr, "%s : %d - fdopen() error : %s\n",
				__func__, __LINE__, strerror(errno));
			close(pipefd[0]);
			return NULL;
		}
		break;
	}

	return fp;
}

/*
 * This function closely mimics pclose(3).
 * Returns :
 *   On success :  exit status of the command as returned by waitpid(2),
 *   On failure : -1 if waitpid(2) returns an error.
 *   If it cannot obtain the child status, errno is set to ECHILD.
 */
int spclose(FILE *stream, pid_t cpid)
{
	int     status;
	pid_t   pid;

	/*
	 * Close the stream, fclose() takes care of closing
	 * the underlying fd.
	 */
	if (fclose(stream) == EOF) {
		fprintf(stderr, "%s : %d - Failed in fclose() : %s\n",
			__func__, __LINE__, strerror(errno));
		return -1;
	}

	/* Wait for the child to exit */
	do {
		pid = waitpid(cpid, &status, 0);
	} while (pid == -1 && errno == EINTR);

	/* Couldn't obtain child status */
	if (status == -1)
		errno = SIGCHLD;

	return status;
}

	/**
	 * findAIXFSEntry
	 * @brief Finds AIX name which exists as a file or link in the dir
	 *	'rootPath'
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
	unsigned int pos = 0;
	int beg, len, count = 0;

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
		int beg = 0, end = 0, i = count;

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
	 * @brief Returns the absolute path of regular file pointed at by a sym link
	 * @param full path of sym link
	 * @return String abs target
	 */
	string HelperFunctions::getSymLinkTarget(const string& symLinkPath)
	{
		char linkTarget[PATH_MAX];

		if ( realpath( symLinkPath.c_str(), linkTarget ) == NULL )
			return string();

		return string( linkTarget );
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
		string tmp;

        if (len <= 0 || i <= 0)
		return string();

		while (i > 0 && curDir[i] != '/')
			curDir[i--] = '\0';

		i = 0;
		while (strncmp(&relPath[i * 3], "../", 3) == 0) {
			dropDir(curDir);
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
		unsigned int beg = 0, end = s1.length();
		unsigned int z;
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
			unsigned int i;
			*str1 = new char[ str2.length( ) * sizeof( char ) + 1 ];
			if ( *str1 != NULL ) {
				i=0;
				while (i < str2.length())
				{
					(*str1)[i] = str2[i];
					i++;
				}

				(*str1)[i] = '\0';
			}
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
		char *chr = NULL;

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
string HelperFunctions::readMatchFromFile(const string& file, const string& str)
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
				fin.close();
				return line;
			}
		}
		fin.close();
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
		char *system_args[32] = {NULL,};
		pid_t cpid;
		int i = 0;
		int rc = -1;
		istringstream ss(cmd);
		string s;
		FILE *fp;

		while ( ss >> s ) {
			if (i < 32) {
				system_args[i] = strdup(s.c_str());
				if (system_args[i] == NULL)
					goto free_mem;
				i++;
			} else {
				goto free_mem;
			}
		}

		fp = spopen(system_args, &cpid);
		if (fp == NULL)
			goto free_mem;

		while ( !feof(fp) )
			if (fgets(buf, BUF_SIZE - 1, fp) != NULL)
				output += buf;

		spclose(fp, cpid);
		rc = 0;

free_mem:
		for (i -= 1; i >= 0; i--)
			free(system_args[i]);

		return rc;
	}
