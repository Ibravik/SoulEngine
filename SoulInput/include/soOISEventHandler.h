/************************************************************************/
/**
* @LC	  10/20/2016
* @file   soOISEventHandler.h
* @Author Rubén Rodríguez Serrano (rubn2113@gmail.com)
* @date   10/5/2016
* @brief  Class in charge of handle the input events
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Headers												                */
/************************************************************************/
#include <OIS.h>
#include "soSuscription.h"

namespace SoulSDK
{
	/************************************************************************/
	/* soOISEventHandler class declaration					                */
	/************************************************************************/
	class soOISEventHandler : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
	{
		/************************************************************************/
		/* Constructor and Destructor declaration	                            */
		/************************************************************************/
	public:
		soOISEventHandler();																/*!< Standard Constructor */
		virtual ~soOISEventHandler();														/*!< Standard Destructor */

																							/************************************************************************/
																							/* soOISEventHandler member variables	 					            */
																							/************************************************************************/
	private:
		std::vector<SignedObject*> m_signedList;											/*!< List for signed actors */
		std::map<int, int> m_buttonMap;														/*!< Map for buttons */
		std::map<int, int> m_axisMap;														/*!< Map for axis */
		std::map<const int, int> m_dpadMap;													/*!< Map for dpad */

																							/************************************************************************/
																							/* soOISEventHandler functions						                    */
																							/************************************************************************/
	public:
		bool keyPressed(const OIS::KeyEvent &arg);											/*!< Key pressed event */
		bool keyReleased(const OIS::KeyEvent &arg);											/*!< Key released event */
		bool mouseMoved(const OIS::MouseEvent &arg);										/*!< Mouse moved event */
		bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);				/*!< Mouse pressed event */
		bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);				/*!< Mouse released event */
		bool buttonPressed(const OIS::JoyStickEvent &arg, int button);						/*!< Button pressed event */
		bool buttonReleased(const OIS::JoyStickEvent &arg, int button);						/*!< Button released event */
		bool axisMoved(const OIS::JoyStickEvent &arg, int axis);							/*!< Axis moved event */
		bool povMoved(const OIS::JoyStickEvent &arg, int pov);								/*!< Pov moved event */
		bool vector3Moved(const OIS::JoyStickEvent &arg, int index);						/*!< Vector3 moved event */

		void SetSignedActors(std::vector<SignedObject*> _signedActors);						/*!< Sets the list of signed actors */
		void SetButtonMap(std::map<int, int> _buttonMap);									/*!< Sets button map */
		void SetAxisMap(std::map<int, int> _axisMap);										/*!< Sets axis map */
		void SetDpadMap(std::map<const int, int> _dpadMap);									/*!< Sets dpad map */
	};
}