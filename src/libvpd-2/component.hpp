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
#ifndef LSVPDCOMPONENT_HPP
#define LSVPDCOMPONENT_HPP

#include <string>
#include <vector>
#include <iostream>

#include <libvpd-2/dataitem.hpp>

/**
 * @defgroup lsvpd lsvpd
 *
 * @brief
 *   Group lsvpd holds the vpd access programs and library
 */

namespace lsvpd
{
	static const int COMPONENT_STATE_LIVE = 0x00000001;
	static const int COMPONENT_STATE_DEAD = 0x00000002;
	/**
	 * This class provides a single storage point for all the VPD connected
	 * to a single entity on a system.  An entity may be a physical piece of
	 * hardware (e.g. an ethernet card or a PCI bus), a virtualized piece of
	 * hardware (e.g. a virtual SCSI drive), or a specific group of
	 * functionality provided by a piece of hardware (e.g. Some laptops have
	 * a sound card that also has an onboard modem).  This object has only
	 * private mutator methods because we do not want programs using libvpd
	 * to be able to modify information stored here.
	 *
	 * @class Component
	 *
	 * @ingroup lsvpd
	 *
	 * @brief
	 *   A Compontent holds VPD for a single entity on a system.
	 *
	 * @author Eric Munson <munsone@us.ibm.com>,
	 *   Brad Peters <bpeters@us.ibm.com
	 */
	class Component
	{
		friend class ProcCollector;
		friend class DeviceTreeCollector;
		friend class SysFSTreeCollector;
		friend class ICollector;
		friend class Gatherer;

		private:
			/**
			 * This holds the unique ID for this Component.  This may be
			 * its entry in sysfs, or the device-tree, or an ID constructed
			 * if neither of these is appropriate.
			 *
			 * @brief
			 *   The ID for this Component
			 */
			DataItem idNode;

			/**
			 * This holds the full path to the directory for this Component
			 * under /proc/device-tree.
			 *
			 * @brief
			 *   The directory in the device-tree for this Component
			 */
			DataItem deviceTreeNode;
			/**
			 * Full path to the directory for this Component under /sys/bus.
			 *
			 * @brief
			 *   The directory in /sys for this component.
			 */
			DataItem sysFsNode;

			/**
			 * @brief
			 *   If the sysFsNode is a sym link, this is the resolved path
			 * 		Should point to /sys/devices/*
			 */
			DataItem sysFsLinkTarget;

			/**
			 * @brief
			 *   Stores the HAL UDI, if HAL is available.
			 */
			DataItem halUDI;

			/**
			 * This will be empty if this is not a net device.
			 *
			 * @brief
			 *   Holds the MAC or Physical Address if this is a net device
			 */
			DataItem mNetAddr;

			/**
			 * @brief
			 *   Holds the full path to this Component under /sys/class
			 */
			DataItem mDevClass;

			/**
			 * @brief
			 *   The description of this Component
			 */
			DataItem mDescription;

			/**
			 * @brief
			 *   The CD field (in vpd-ese) for this Component.
			 */
			DataItem mCDField;

			/**
			 * @brief
			 *   The serial number for this Component.
			 */
			DataItem mSerialNumber;

			/**
			 * @brief
			 *   The part number for this Component.
			 */
			DataItem mPartNumber;

			/**
			 * This field can be used to store non-alterable ROM version
			 * numbers.  Though some devices place alterable ROM numbers here.
			 *
			 * @brief
			 *   The firmware level for this Component.
			 */
			DataItem mFirmwareLevel;

			/**
			 * This field usually holds the version number for the alterable
			 * ROM.
			 *
			 * @brief
			 *   The firmware version for this Component.
			 */
			DataItem mFirmwareVersion;

			/**
			 * @brief
			 *   The Field Replaceable Part Number for this Component.
			 */
			DataItem mFRU;

			/**
			 * @brief
			 *   The manufacturer name for this Component.
			 */
			DataItem mManufacturer;

			/**
			 * @brief
			 *   The model of this Component.
			 */
			DataItem mModel;

			/**
			 * @brief
			 *   The Manufacturer ID of thie Component.
			 */
			DataItem mManufacturerID;

			/**
			 * Usually holds the hardware revision number or version.
			 *
			 * @brief
			 *   The Engineering Change Level of this Component.
			 */
			DataItem mEngChangeLevel;

			/**
			 * @brief
			 *   The ID of this Component's parent.
			 */
			DataItem mParent;

			/**
			 * @brief
			 *   The Subsystem of this Component (e.g. pci or scsi)
			 */
			DataItem devSubsystem;

			/**
			 * @brief
			 * 	Purely internal - used to indicate state during
			 * 	tree merger.  If COMPONENT_STATE_DEAD, this component is already
			 * 	represented in target tree, and should not be copied.  In
			 * 	this state, devSelfInSysfs will point to equivalent node in
			 *  sysfs tree
			 */
			int devState;

			/**
			 * @brief
			 * n5 and n5 are keywords parsed out and used by inventory scout,
			 * and relate to Capacity on Demand system information.
			 */
			DataItem n5;
			DataItem n6;

			/**
			 * @brief
			 * 	Used during tree merge; this devices equivalent in the sys
			 * 	tree
			 */
			Component * devSelfInSysfs;

			/**
			 * @brief
			 *   Device Driver name for this Component.
			 */
			DataItem devDriver;

			DataItem devKernel;
			DataItem devKernelNumber;
			DataItem devSysName;  ///< Device name, as seen by sysFS
			DataItem devDevTreeName; ///< Device name, as seen by device-tree
			DataItem devBus;  ///<  /sys/bus/XXX
			DataItem devType; ///< usually similar to devBus, but more detailed
			DataItem devBusAddr; ///< /sys/bus/*/devices/XXX
			DataItem mRecordType;
			DataItem scsiDetail;
			DataItem plantMfg;  ///< SE - Plant of manufacture
			DataItem mFeatureCode;
			DataItem mKeywordVersion;
			DataItem mMicroCodeImage;
			DataItem mPhysicalLocation;
			DataItem mSecondLocation;

			vector<string> mChildren;
			vector<DataItem*> mDeviceSpecific;
			vector<DataItem*> mUserData;
			/**
			 * @brief
			 *   Holds the kernel names for this device
			 */
			vector<DataItem*> mAIXNames;

			// These are not stored in the DB, they are computed.
			vector<Component*> mLeaves;
			Component* mpParent;
			int devMajor;  ///< Major:minor codes for device lookup
			int devMinor;
			int devAccessMode;  // For direct query of device

			/**
			 * This creates a new DataItem, filling it with the provided
			 * data and adds it to the mDeviceSpecific vector.
			 *
			 * @brief
			 *   Adds an entry in to the Device Specifc vector.
			 * @param ac
			 *   Holds the acronymn (in vpd-ese) for the new entry
			 * @param humanName
			 *   A nore descriptive label for the new entry
			 * @param val
			 *   The value of the new entry
			 * @param prefLvl
			 *   The preference level to use for the new entry
			 */
			void addDeviceSpecific( const string& ac, const string& humanName,
									const string& val, int prefLvl );

			/**
			 * Like addDeviceSpecific, however, if it finds an existing
			 * entry, we modify the existing one with the new Value.
			 * @brief
			 *   Adds an entry in to the Device Specifc vector.
			 * @param ac
			 *   Holds the acronymn (in vpd-ese) for the new entry
			 * @param humanName
			 *   A more descriptive label for the new entry
			 * @param val
			 *   The value of the new entry
			 * @param prefLvl
			 *   The preference level to use for the new entry
			 */
			void updateDeviceSpecific( const string& ac, const string& humanName,
									const string& val, int prefLvl );
			/**
			 * This creates a new DataItem, filling it with the provided data
			 * and attempts to add it to the UserData vector.  Because the
			 * UserData vector has to have unique acronymns the caller needs
			 * to specify if this data should replace any data that might be
			 * present.
			 *
			 * @brief
			 *   Adds or overwrites an entry in the UserData vector.
			 * @param ac
			 *   The acronymn for the new entry
			 * @param humanName
			 *   Descriptive label for the nex entry
			 * @param prefLvl
			 *   The preference level to use for the new entry
			 * @param clobber
			 *   If the Item exists, should the value be clobbered
			 */
			void addUserData( const string& ac, const string& humanName,
								const string& val, int prefLvl, bool clobber );
			/**
			 * This creates a new DataItem, filling it with the provided
			 * data and adds it to the mAIXNames vector.
			 *
			 * @brief
			 *   Adds an entry in to the AIX Names vector.
			 * @param val
			 *   The value of the new entry
			 * @param prefLvl
			 *   The preference level to use for the new entry
			 */
			void addAIXName( const string& val, int prefLvl );

			/**
			 * @brief
			 *   Removes the specified entry from the mLeaves vector.
			 * @param index
			 *   The index of the Item to remove
			 */
			bool removeLeaf( int index );

			/**
			 * @brief
			 *   Removes the specified entry from the mLeaves vector.
			 * @param in
			 *   The Component ID to be removed
			 */
			bool removeLeaf( const string& in );

			bool isChild(Component *child);
			bool isChild(const string& child);
			int numChildren();

			/**
			 * @brief
			 *   Copies data from arg to this.
			 * @param copyMe
			 *   The Component to be copied.
			 */
			void copyToMe( const Component& copyMe );

			/**
			 * Starts with the base size of a Component (INIT_BUF_SIZE), and
			 * sums the size of each Data Item and returns the number of
			 * bytes required to store *this.
			 *
			 * @brief
			 *   Calculates the size of *this
			 */
			unsigned int getPackedSize( );

		public:
			/**
			 * This is the number of bytes required to store an empty
			 * Component object into the DB.  To recalculate this value
			 * sum (remembering to add 1 to each for the null byte)
			 * the length of each vector separator (CHILD_START,
			 * CHILD_END, DEVICE_START, etc.)
			 */
			static const int INIT_BUF_SIZE = 129;

			//Delimiters for data set vectors.
			static const string CHILD_START;
			static const string CHILD_END;

			static const string DEVICE_START;
			static const string DEVICE_END;

			static const string USER_START;
			static const string USER_END;

			static const string AX_START;
			static const string AX_END;

			/**
			 * @brief
			 *   Builds empty Component.
			 */
			Component( );

			/**
			 * @brief
			 *   Copies provided Component
			 */
			Component( const Component& copyMe );

			/**
			 * Uses the unpack method to populate a new Component with the
			 * "serialized" data in the provided buffer.
			 *
			 * @brief
			 *   Unpackes the "serialized" Component from buffer.
			 * @param packedData
			 *   Buffer that contains Component information
			 */
			Component( const void* packedData );

			~Component( );

			Component& operator=( const Component& rhs );



			void addChild( const string& in);

			void removeChild( const string& id );

			/**
			 * getDevClass
			 * @brief Returns the class name -
			 * 	 ie: /sys/class/net/eth0 would return net
			 */
			const string getDevClass();

			/**
			 * getClassNode
			 * @brief Returns the full path for the device in the /sys/class
			 */
			inline string getClassNode( )
			{
				return mDevClass.getValue();
			}

			void setMajor(int in)
			{
				devMajor = in;
			}

			void setMinor(int in)
			{
				devMinor = in;
			}

			void setMode(int in)
			{
				devAccessMode = in;
			}

			inline int getMajor()
			{
				return devMajor;
			}

			inline int getMinor()
			{
				return devMinor;
			}

			inline int getMode()
			{
				return devAccessMode;
			}

			const string& getChild( int i ) const;

			/**
			 * Unpacks the packed Component information stored in the buffer.
			 * This method will fill *this will the information stored in
			 * the buffer.  This method will not do any memory management,
			 * that is it will not free the buffer passed in, that is the
			 * responsibility of the caller.
			 *
			 * @brief
			 *   Unpacks the provided buffer into *this
			 * @param packed
			 *   The buffer holding a packed Component
			 */
			void unpack( const void* packed );

			/**
			 * This method takes a char ** and creates a buffer sized
			 * appropriately to hold the information stored in *this.
			 * It will store the address of this buffer in *buffer for
			 * use outside of the method.  Pack will allocate the buffer
			 * or reallocate if *buffer is not NULL.  The caller will be
			 * responsible for deleting the buffer when they are finished
			 * with it.
			 *
			 * @brief
			 *   Packes *this into a buffer.
			 * @param buffer
			 *   Address of a pointer to fill.
			 * @return
			 *   The size of the buffer filled.
			 */
			unsigned int pack( void** buffer );

			//The Get Value methods
			inline const string& getDescription( ) const
			{ return mDescription.getValue(); }
			inline const string& getRecordType( ) const
			{ return mRecordType.getValue( ); }
			inline const string& getSerialNumber( ) const
			{ return mSerialNumber.getValue(); }
			inline const string& getPartNumber( ) const
			{ return mPartNumber.getValue(); }
			inline const string& getFRU( ) const
			{ return mFRU.getValue(); }
			inline const string& getFirmwareLvl( ) const
			{ return mFirmwareLevel.getValue(); }
			inline const string& getFirmwareVersion( ) const
			{ return mFirmwareVersion.getValue(); }
			inline const string& getFirmwareLevel( ) const
			{ return mFirmwareLevel.getValue(); }
			inline const string& getManufacturer( ) const
			{ return mManufacturer.getValue(); }
			inline const string& getDeviceDriverName( ) const
			{ return devDriver.getValue(); }
			inline const string& getModel( ) const
			{ return mModel.getValue(); }
			inline const string& getID( ) const
			{ return idNode.getValue(); }
			inline const string& getFeatureCode( ) const
			{ return mFeatureCode.getValue(); }
			inline const string& getEngChange( ) const
			{ return mEngChangeLevel.getValue(); }
			inline const string& getParent( ) const
			{ return mParent.getValue(); }
			inline const string& getManufacturerID( ) const
			{ return mManufacturerID.getValue( ); }
			inline const string& getCD( ) const
			{ return mCDField.getValue( ); }
			inline const string& getNetAddr( ) const
			{ return mNetAddr.getValue( ); }
			inline const string& getPhysicalLocation( ) const
			{ return mPhysicalLocation.getValue( ); }
			inline const string& getSecondLocation( ) const
			{ return mSecondLocation.getValue( ); }
			inline const string& getIdNode( ) const
			{ return idNode.getValue( ); }
			inline const vector<string>& getChildren( ) const
			{ return mChildren; }
			inline const vector<DataItem*>& getDeviceSpecific( ) const
			{ return mDeviceSpecific; }
			const DataItem * getDeviceSpecific(const string& itemAC );
			inline const vector<DataItem*>& getUserData( ) const
			{ return mUserData; }
			inline const string& getDevTreeNode( ) const
			{ return deviceTreeNode.getValue( ); }
			inline const string& getKeywordVersion( ) const
			{ return mKeywordVersion.getValue( ); }
			inline const string& getMicroCodeImage( ) const
			{ return mMicroCodeImage.getValue( ); }
			inline const vector<DataItem*>& getAIXNames( ) const
			{ return mAIXNames; }
			inline const string& getDeviceTreeNode( ) const
			{ return deviceTreeNode.getValue( ); }
			inline const string& getSysFsNode( ) const
			{ return sysFsNode.getValue( ); }
			inline const string& getSysFsLinkTarget( ) const
			{ return sysFsLinkTarget.getValue( ); }
			inline const string& getHalUDI( ) const
			{ return halUDI.getValue( ); }
			inline const string& getDevSysName( ) const
			{ return devSysName.getValue( ); }
			inline const string& getDevTreeName( ) const
			{ return devDevTreeName.getValue( ); }
			inline const string& getDevBus( ) const
			{ return devBus.getValue( ); }
			inline const string& getDevType( ) const
			{ return devType.getValue( ); }
			inline const string& getDevBusAddr( ) const
			{ return devBusAddr.getValue( ); }
			inline const string& getn5( ) const
			{ return n5.getValue(); }
			inline const string& getn6( ) const
			{ return n6.getValue(); }
			inline const string& getMachineSerial() const
			{ return plantMfg.getValue(); }
			const string* getMicroCodeLevel( );

			// The Get Acronymn methods
			inline const string& getDescriptionAC( ) const
			{return mDescription.getAC( ); }
			inline const string& getRecordTypeAC( ) const
			{ return mRecordType.getAC( ); }
			inline const string& getSerialNumberAC( ) const
			{ return mSerialNumber.getAC(); }
			inline const string& getPartNumberAC( ) const
			{ return mPartNumber.getAC(); }
			inline const string& getFRUAC( ) const
			{ return mFRU.getAC(); }
			inline const string& getFirmwareLevelAC( ) const
			{ return mFirmwareLevel.getAC(); }
			inline const string& getFirmwareVersionAC( ) const
			{ return mFirmwareVersion.getAC(); }
			inline const string& getManufacturerAC( ) const
			{ return mManufacturer.getAC(); }
			inline const string& getModelAC( ) const
			{ return mModel.getAC(); }
			inline const string& getIDAC( ) const
			{ return idNode.getAC(); }
			inline const string& getFeatureCodeAC( ) const
			{ return mFeatureCode.getAC(); }
			inline const string& getEngChangeAC( ) const
			{ return mEngChangeLevel.getAC(); }
			inline const string& getParentAC( ) const
			{ return mParent.getAC(); }
			inline const string& getManufacturerIDAC( ) const
			{ return mManufacturerID.getAC( ); }
			inline const string& getCDAC( ) const
			{ return mCDField.getAC( ); }
			inline const string& getNetAddrAC( ) const
			{ return mNetAddr.getAC( ); }
			inline const string& getPhysicalLocationAC( ) const
			{ return mPhysicalLocation.getAC( ); }
			inline const string& getSecondLocationAC( ) const
			{ return mSecondLocation.getAC( ); }
			inline const string& getidNodeAC( ) const
			{ return idNode.getAC(); }
			inline const string& getDevTreeNodeAC( ) const
			{ return deviceTreeNode.getAC( ); }
			inline const string& getKeywordVersionAC( ) const
			{ return mKeywordVersion.getAC( ); }
			inline const string& getMicroCodeImageAC( ) const
			{ return mMicroCodeImage.getAC( ); }
			inline const string& getn5AC( ) const
			{ return n5.getAC( ); }
			inline const string& getn6AC( ) const
			{ return n6.getAC( ); }
			inline const string& getMachineSerialAC() const
			{ return plantMfg.getAC(); }

			// The get human name methods
			inline const string& getDescriptionHN( ) const
			{ return mDescription.getHumanName(); }
			inline const string& getRecordTypeHN( ) const
			{ return mRecordType.getHumanName( ); }
			inline const string& getSerialNumberHN( ) const
			{ return mSerialNumber.getHumanName(); }
			inline const string& getPartNumberHN( ) const
			{ return mPartNumber.getHumanName(); }
			inline const string& getFRUHN( ) const
			{ return mFRU.getHumanName(); }
			inline const string& getFirmwareLvlHN( ) const
			{ return mFirmwareLevel.getHumanName(); }
			inline const string& getFirmwareVersionHN( ) const
			{ return mFirmwareVersion.getHumanName(); }
			inline const string& getManufacturerHN( ) const
			{ return mManufacturer.getHumanName(); }
			inline const string& getModelHN( ) const
			{ return mModel.getHumanName(); }
			inline const string& getIDHN( ) const
			{ return idNode.getHumanName(); }
			inline const string& getFeatureCodeHN( ) const
			{ return mFeatureCode.getHumanName(); }
			inline const string& getEngChangeHN( ) const
			{ return mEngChangeLevel.getHumanName(); }
			inline const string& getParentHN( ) const
			{ return mParent.getHumanName(); }
			inline const string& getManufacturerIDHN( ) const
			{ return mManufacturerID.getHumanName( ); }
			inline const string& getCDHN( ) const
			{ return mCDField.getHumanName( ); }
			inline const string& getNetAddrHN( ) const
			{ return mNetAddr.getHumanName( ); }
			inline const string& getPhysicalLocationHN( ) const
			{ return mPhysicalLocation.getHumanName( ); }
			inline const string& getSecondLocationHN( ) const
			{ return mSecondLocation.getHumanName( ); }
			inline const string& getidNodeHN( ) const
			{ return idNode.getHumanName(); }
			inline const string& getDevTreeNodeHN( ) const
			{ return deviceTreeNode.getHumanName( ); }
			inline const string& getKeywordVersionHN( ) const
			{ return mKeywordVersion.getHumanName( ); }
			inline const string& getMicroCodeImageHN( ) const
			{ return mMicroCodeImage.getHumanName( ); }
			inline const string& getn5HN( ) const
			{ return n5.getHumanName( ); }
			inline const string& getn6HN( ) const
			{ return n6.getHumanName( ); }
			inline const string& getMachineSerialHN() const
			{ return plantMfg.getHumanName(); }

			inline const vector<Component*>& getLeaves( ) const { return mLeaves; }
			inline void addLeaf( Component* in ) { mLeaves.push_back( in ); }
	};

}

#endif
