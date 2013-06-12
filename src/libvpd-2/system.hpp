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
#ifndef LSVPDSYSTEM_HPP
#define LSVPDSYSTEM_HPP

#include <string>
#include <vector>
#include <iostream>

#include <libvpd-2/dataitem.hpp>
#include <libvpd-2/component.hpp>
#include <libvpd-2/lsvpd.hpp>

#include <sys/types.h>
#include <linux/types.h>

namespace lsvpd
{
	class System
	{
		friend class ProcCollector;
		friend class DeviceTreeCollector;
		friend class SysFSTreeCollector;
		friend class ICollector;
		friend class Gatherer;

		private:
			DataItem mIdNode;
			DataItem mArch;
			DataItem deviceTreeNode;
			DataItem mDescription;
			DataItem mBrand;
			DataItem mNodeName;
			DataItem mOS;
			DataItem mProcessorID;
			DataItem mMachineType;
			DataItem mMachineModel;
			DataItem mFeatureCode;
			DataItem mFlagField;
			DataItem mRecordType;
			DataItem mSerialNum1;
			DataItem mSerialNum2;
			DataItem mSUID;
			DataItem mKeywordVersion;
			DataItem mLocationCode;
			u32 mCPUCount;

			vector<string> mChildren;
			vector<DataItem*> mDeviceSpecific;
			vector<DataItem*> mUserData;
			vector<Component*> mLeaves;

			unsigned int getPackedSize( );

			// All of the mutator methods for this are private to prevent anyone outside
			// of our friend list from modifying a System Object.
			void addDeviceSpecific( const string& ac, const string& humanName,
									const string& val, int lvl );
			void addUserData( const string& ac, const string& humanName,
								const string& val, int prefLvl, bool clobber );
			bool removeDeviceSpecific( int index );
			bool removeDeviceSpecific( const string& spec );
			bool removeLeaf( int index );
			bool removeLeaf( const string& in );

			bool isChild(Component *child);
			const string getChild( int i);
			inline void addChild( const string& in) { mChildren.push_back( in ); }

		public:
			static const int INIT_BUF_SIZE = 107;

			//Delimiters for data set vectors.
			static const string CHILD_START;
			static const string CHILD_END;

			static const string DEVICE_START;
			static const string DEVICE_END;

			static const string USER_START;
			static const string USER_END;

			static const string ID;

			System();
			System( const void* packedData );
			~System( );

			// These methods contain the logic to pack and unpack this object
			// for use with the db.  pack will new the apporiate buffer and
			// store it in *buffer and it will expect the consumer to call an
			// appropriate delete.  unpack will not do any memory management,
			// the consumer will be expected to new the appropriate memory
			// and free it when unpack finishes.
			void unpack( const void* packed );

			unsigned int pack( void** buffer );

			inline const vector<string>& getChildren( ) const
			{
				return mChildren;
			}

			void removeChild( const string& id );

			inline const string& getID( ) const { return ID; }

			//friend ostream& operator<<( ostream& os, const System& in );

			/*
			 * These functions will be used by the VPD library interface only, they allow
			 * the consumer to hold a representation of the hardware as a tree.
			 */
			inline const vector<Component*>& getLeaves( ) const { return mLeaves; }
			inline void addLeaf( Component* in ) { mLeaves.push_back( in ); }

			//Get Value methods
			inline const string& getMachineType( ) const
			{ return mMachineType.getValue( ); }
			inline const string& getSerial1( ) const
			{ return mSerialNum1.getValue( ); }
			inline const string& getSerial2( ) const
			{ return mSerialNum2.getValue( ); }
			inline const string& getProcessorID( ) const
			{ return mProcessorID.getValue( ); }
			inline const string& getOS( ) const
			{ return mOS.getValue( ); }
			inline const string& getFeatureCode( ) const
			{ return mFeatureCode.getValue( ); }
			inline const string& getDescription( ) const
			{ return mDescription.getValue( ); }
			inline const string& getRecordType( ) const
			{ return mRecordType.getValue( ); }
			inline const string& getLocation( ) const
			{ return mLocationCode.getValue( ); }
			inline const string& getMachineModel( ) const
			{ return mMachineModel.getValue( ); }
			inline const string& getSUID( ) const
			{ return mSUID.getValue( ); }
			inline const string& getKeywordVer( ) const
			{ return mKeywordVersion.getValue( ); }
			inline const string& getFlagField( ) const
			{ return mFlagField.getValue( ); }
			inline const string& getBrand( ) const
			{ return mBrand.getValue( ); }
			inline const vector<DataItem*>& getDeviceSpecific( ) const
			{ return mDeviceSpecific; }
			inline const string& getArch( ) const
			{ return mArch.getValue( ); }
			inline int getCPUCount( ) const
			{ return (int)mCPUCount; }
			inline const string& getDevTreeNode( ) const
			{ return deviceTreeNode.getValue( ); }

			//Get AC Methods
			inline const string& getMachineTypeAC( ) const
			{ return mMachineType.getAC( ); }
			inline const string& getSerial1AC( ) const
			{ return mSerialNum1.getAC( ); }
			inline const string& getSerial2AC( ) const
			{ return mSerialNum2.getAC( ); }
			inline const string& getProcessorIDAC( ) const
			{ return mProcessorID.getAC( ); }
			inline const string& getOSAC( ) const
			{ return mOS.getAC( ); }
			inline const string& getFeatureCodeAC( ) const
			{ return mFeatureCode.getAC( ); }
			inline const string& getDescriptionAC( ) const
			{ return mDescription.getAC( ); }
			inline const string& getRecordTypeAC( ) const
			{ return mRecordType.getAC( ); }
			inline const string& getLocationAC( ) const
			{ return mLocationCode.getAC( ); }
			inline const string& getMachineModelAC( ) const
			{ return mMachineModel.getAC( ); }
			inline const string& getSUIDAC( ) const
			{ return mSUID.getAC( ); }
			inline const string& getKeywordVerAC( ) const
			{ return mKeywordVersion.getAC( ); }
			inline const string& getFlagFieldAC( ) const
			{ return mFlagField.getAC( ); }
			inline const string& getBrandAC( ) const
			{ return mBrand.getAC( ); }
			inline const string& getDevTreeNodeAC( ) const
			{ return deviceTreeNode.getAC( ); }

			//Get Human Name methods
			inline const string& getMachineTypeHN( ) const
			{ return mMachineType.getHumanName( ); }
			inline const string& getSerial1HN( ) const
			{ return mSerialNum1.getHumanName( ); }
			inline const string& getSerial2HN( ) const
			{ return mSerialNum2.getHumanName( ); }
			inline const string& getProcessorIDHN( ) const
			{ return mProcessorID.getHumanName( ); }
			inline const string& getOSHN( ) const
			{ return mOS.getHumanName( ); }
			inline const string& getFeatureCodeHN( ) const
			{ return mFeatureCode.getHumanName( ); }
			inline const string& getDescriptionHN( ) const
			{ return mDescription.getHumanName( ); }
			inline const string& getRecordTypeHN( ) const
			{ return mRecordType.getHumanName( ); }
			inline const string& getLocationHN( ) const
			{ return mLocationCode.getHumanName( ); }
			inline const string& getMachineModelHN( ) const
			{ return mMachineModel.getHumanName( ); }
			inline const string& getSUIDHN( ) const
			{ return mSUID.getHumanName( ); }
			inline const string& getKeywordVerHN( ) const
			{ return mKeywordVersion.getHumanName( ); }
			inline const string& getFlagFieldHN( ) const
			{ return mFlagField.getHumanName( ); }
			inline const string& getBrandHN( ) const
			{ return mBrand.getHumanName( ); }
			inline const string& getDevTreeNodeHN( ) const
			{ return deviceTreeNode.getHumanName( ); }
	};
}

#endif
