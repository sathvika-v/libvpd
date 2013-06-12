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

#ifndef LSVPD_ERROR_CODES_HPP
#define LSVPD_ERROR_CODES_HPP

#include <string>
using namespace std;

namespace lsvpd {
	/* Thrown error codes should be looked for in errno.h first.
	 */
	enum {
		FILE_NOT_FOUND = 151,
		DIRECTORY_NOT_FOUND,
		SYSTEM_COMMAND_PROCESSOR_NOT_FOUND,
		CLASS_NODE_UNDEFINED,
		ERROR_ACCESSING_DEVICE,
		BUFFER_EMPTY,
		SGUTILS_READ_ERROR,
		UNABLE_TO_OPEN_FILE,
		UNABLE_TO_MKNOD_FILE,
		SG_DATA_INVALID,
		SGUTILS_IOCTL_FAILED,

		RTAS_CALL_NOT_AVAILABLE,
		RTAS_PARAMETER_ERROR,
		RTAS_HARDWARD_ERROR,
		RTAS_ERROR,

		SCSI_FILL_TEMPLATE_LOADING
		};

	string interp_err_code(int errCode);
}
#endif /*LSVPD_ERROR_CODES_H_*/
