/************************************************************************/
/**
* @LC	  10/20/2016
* @file   soInputMap.h
* @Author Rubén Rodríguez Serrano (rubn2113@gmail.com)
* @date   10/5/2016
* @brief  Class in charge of creates an user input map
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Headers												                */
/************************************************************************/
#include <Soul_Math.h>

namespace SoulSDK
{
	/************************************************************************/
	/* Button enum for check input state					                */
	/************************************************************************/
	enum OISButtonState
	{
		OISBS_A = 0,
		OISBS_B = 1,
		OISBS_X = 2,
		OISBS_Y = 3,
		OISBS_RB = 4,
		OISBS_LB = 5,
		OISBS_BACK = 6,
		OISBS_START = 7,
		OISBS_LASTICK = 8,
		OISBS_RASTICK = 9
	};

	/************************************************************************/
	/* Axis enum for check input state						                */
	/************************************************************************/
	enum OISAxisState
	{
		OISAS_LASTICKVER = 0,
		OISAS_LASTICKHOR = 1,
		OISAS_RASTICKVER = 2,
		OISAS_RASTICKHOR = 3,
		OISAS_RLTRIGGER = 4
	};

	/************************************************************************/
	/* Dpad struct for check input state						            */
	/************************************************************************/
	struct OISDPadState
	{
		static const int OISDSCentered = 0x00000000;
		static const int OISDSNorth = 0x00000001;
		static const int OISDSSouth = 0x00000010;
		static const int OISDSEast = 0x00000100;
		static const int OISDSWest = 0x00001000;
		static const int OISDSNorthEast = 0x00000101;
		static const int OISDSSouthEast = 0x00000110;
		static const int OISDSNorthWest = 0x00001001;
		static const int OISDSSouthWest = 0x00001010;
	};

	/************************************************************************/
	/* Gamepad input data for creates input map				                */
	/************************************************************************/
	struct GamepadInputData
	{
		uint32 A;
		uint32 B;
		uint32 X;
		uint32 Y;
		uint32 START;
		uint32 BACK;
		uint32 LEFTBUMPER;
		uint32 RIGHTBUMPER;
		uint32 LEFTANALOGUESTICK;
		uint32 RIGHTANALOGUESTICK;
		uint32 LRTRIGGER;
		uint32 LEFTANALOGUESTICKMOVEHOR;
		uint32 LEFTANALOGUESTICKMOVEVER;
		uint32 RIGHTANALOGUESTICKMOVEHOR;
		uint32 RIGHTANALOGUESTICKMOVEVER;
		uint32 DPADCENTERED;
		uint32 DPADNORTH;
		uint32 DPADSOUTH;
		uint32 DPADEAST;
		uint32 DPADWEST;
		uint32 DPADNORTHEAST;
		uint32 DPADSOUTHEAST;
		uint32 DPADNORTHWEST;
		uint32 DPADSOUTHWEST;

		GamepadInputData()
		{
			A = OISBS_A;
			B = OISBS_B;
			X = OISBS_X;
			Y = OISBS_Y;
			START = OISBS_START;
			BACK = OISBS_BACK;
			LEFTBUMPER = OISBS_LB;
			RIGHTBUMPER = OISBS_RB;
			LEFTANALOGUESTICK = OISBS_LASTICK;
			RIGHTANALOGUESTICK = OISBS_RASTICK;

			LEFTANALOGUESTICKMOVEVER = OISAS_LASTICKVER;
			LEFTANALOGUESTICKMOVEHOR = OISAS_LASTICKHOR;
			RIGHTANALOGUESTICKMOVEVER = OISAS_RASTICKVER;
			RIGHTANALOGUESTICKMOVEHOR = OISAS_RASTICKHOR;
			LRTRIGGER = OISAS_RLTRIGGER;

			DPADCENTERED = OISDPadState::OISDSCentered;
			DPADNORTH = OISDPadState::OISDSNorth;
			DPADSOUTH = OISDPadState::OISDSSouth;
			DPADEAST = OISDPadState::OISDSEast;
			DPADWEST = OISDPadState::OISDSWest;
			DPADNORTHEAST = OISDPadState::OISDSNorthEast;
			DPADSOUTHEAST = OISDPadState::OISDSSouthEast;
			DPADNORTHWEST = OISDPadState::OISDSNorthWest;
			DPADSOUTHWEST = OISDPadState::OISDSSouthWest;
		}
	};

	/************************************************************************/
	/* soInputMap class declaration							                */
	/************************************************************************/
	class soInputMap
	{
		/************************************************************************/
		/* Constructor and Destructor declaration	                            */
		/************************************************************************/
	public:
		soInputMap();																				/*!< Standard Constructor */
		virtual ~soInputMap();																		/*!< Standard Destructor */

																									/************************************************************************/
																									/* soInputMap member variables	 							            */
																									/************************************************************************/
	private:
		std::map<int, int> m_buttonMap;																/*!< Map for buttons */
		std::map<int, int> m_axisMap;																/*!< Map for axis */
		std::map<const int, int> m_dpadMap;															/*!< Map for dpad */

																									/************************************************************************/
																									/* soInputMap functions								                    */
																									/************************************************************************/
	public:
		void CreateGamepadMap(GamepadInputData _data);												/*!< Creates a gamepad map */

		std::map<int, int> GetButtonMap();															/*!< Gets the button map */
		std::map<int, int> GetAxisMap();															/*!< Gets the axis map */
		std::map<const int, int> GetDpadMap();														/*!< Gets the dpad map */
	};
}

