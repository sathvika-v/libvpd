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
#ifndef DATAITEM_HPP
#define DATAITEM_HPP

#include <string>
#include <vector>
#include <iostream>

#include <libvpd-2/Source.hpp>

using namespace std;

namespace lsvpd
{
	/**
	 * Holds a single item of data inside a Component as well as some
	 * meta-data about this information.  It contains labels for this data
	 * and a vector of sources for this data.
	 *
	 * @class DataItem
	 *
	 * @ingroup lsvpd
	 *
	 * @brief
	 *   Holds a single item of data for a Component
	 *
	 * @author Eric Munson <munsone@us.ibm.com>,
	 *   Brad Peters <bpeters@us.ibm.com
	 */
	class DataItem
	{
		/**
		 * Any mutator for this class is marked private because programs
		 * outside of this package should not be allowed to modify the VPD
		 * stored.  So each class that is allowed to modify the data
		 * (the Collector Classes, the Data storage classes, and the Gatherer)
		 * will be marked as friend classes.
		 */
		friend class DeviceTreeCollector;
		friend class SysFSTreeCollector;
		friend class ProcCollector;
		friend class ICollector;
		friend class Gatherer;
		friend class Component;
		friend class System;

		private:
			string humanName; ///< Human readable name for field
			string ac; ///< Acronym for field - refer to lsvpd-acronyms
			string dataValue;  /**< Actual value of this data item,
								as obtained from one of the sub-systems*/
			int packedLength;
			vector<Source*> sources;	/**< A collection of all the
										alternative sources to fill this data
										 item */
			int prefLevelUsed;	/**< PreferenceLevel of source used to fill
									this dataitem.  Zero if not filled*/

			int setValue( const string& in, int prefLevelUsed_t,
								const char *file, int lineNum);
			void setHumanName( const string& in );
			void setAC(const string& in);
			int getPrefLevel();

		public:

			DataItem( );
			DataItem(string& in) : dataValue(in), packedLength( 0 ),
				 prefLevelUsed(0)
				 {};

			/**
			 * Unpackes a buffer using the unpack method to fill *this.
			 *
			 * @brief
			 *   Unpacks data into *this.
			 *
			 * @param data
			 *   The buffer holding the DataItem
			 */
			DataItem( const void * data ) : packedLength( 0 ), prefLevelUsed(0)
			{ unpack( data ); }
			DataItem( const DataItem& copyMe );

			~DataItem( );

			const string& getHumanName( ) const;
			const string& getAC( ) const;
			const string& getValue() const;

			/**
			 * @brief
			 *   Returns the number of bytes required to hold *this.
			 *
			 * @return
			 *   The buffer size needed to hold *this.
			 */
			int getPackedLength( );

			/**
			 * Pack will not do any memory management, it assumes that
			 * buffer is a valid pointer and that there is enough space
			 * after buffer to hold *this.  This is valid because this
			 * method should only be called from Component::unpack.
			 *
			 * @brief
			 *   Stores *this into the buffer.
			 *
			 * @param buffer
			 *   The buffer to use to store *this.
			 *
			 * @return
			 *   The number of bytes added to the buffer.
			 */
			int pack( void* buffer );

			/**
			 * Like DataItem::pack, unpack does not do any memory management.
			 * It assumes that data is a valid pointer and that the information
			 * following *data will fill this DataItem.  This assumption is
			 * used only because this method should only be called from
			 * Component::unpack.
			 *
			 * @brief
			 *   Unpacks buffer into *this.
			 *
			 * @param data
			 *   The buffer to be unpacked.
			 */
			void unpack( const void * data );

			int getNumSources() const;
			Source * getSource(int i) const;
			void addSource(Source *in);
			void addSource(char **buf);
			void removeSource(int i);

			inline const int getPrefLevelUsed()
			{ return prefLevelUsed; }

			inline const vector<Source*>& getSources( ) const
			{ return sources; }

			inline const Source* getFirstSource( ) const
			{
				if( !sources.empty( ) )
					return sources.at( 0 );
				else
					return NULL;
			}

			//friend ostream& operator<<( ostream& os, const DataItem& in );
	};

}

#endif /* DATAITEM_H */
