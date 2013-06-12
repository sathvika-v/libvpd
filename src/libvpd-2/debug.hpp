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

#ifndef DEBUG_HPP
#define DEBUG_HPP

//#define GREEN "\e[0;32m"
//#define RED "\e[1;31m"
//#define BLUE "\e[1;34m"
//#define NC "\e[0m"
#define GREEN ""
#define RED ""
#define BLUE ""
#define NC ""

#define STRIT(x) #x
#define TOSTRING(x) STRIT(x)

#ifdef TRACE_ON
#define coutd cout << RED << __FILE__ << ": " << GREEN << __func__ << "()" << BLUE << " [" << __LINE__ << "] " << NC
#else
#define coutd if(0) cout
#endif

#endif /*DEBUG_H_*/
