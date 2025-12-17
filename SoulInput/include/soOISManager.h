/************************************************************************/
/**
* @LC	  10/20/2016
* @file   soOISManager.h
* @Author Rubén Rodríguez Serrano (rubn2113@gmail.com)
* @date   10/5/2016
* @brief  Class in charge of manage inputs
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Headers												                */
/************************************************************************/
#include <OIS.h>
#include"soOISEventHandler.h"

namespace SoulSDK
{
	/************************************************************************/
	/* soOISManager class declaration							            */
	/************************************************************************/
	class soOISManager : public soModule<soOISManager, const size_t&>
	{
		/************************************************************************/
		/* Constructor and Destructor declaration	                            */
		/************************************************************************/
	public:
		soOISManager();																														/*!< Standard Constructor */
		virtual ~soOISManager();																											/*!< Standard Destructor */

		/************************************************************************/
		/* soOISManager member variables	 							        */
		/************************************************************************/
	private:
		OIS::InputManager* m_inputManager;																									/*!< Input system */
		OIS::Keyboard* m_keyboard;																											/*!< Keyboard device */
		OIS::Mouse* m_mouse;																												/*!< Mouse device */
		OIS::JoyStick* m_joystick[4] = { 0,0,0,0 };																							/*!< JoyStick device that supports up to 4 controllers */

		soOISEventHandler m_eventHandler;																									/*!< Reference to an event handler */

		/************************************************************************/
		/* soOISManager functions									            */
		/************************************************************************/
	public:
		void CreateComponents();																											/*!< Creates manage components */
		void Update();																														/*!< Updates inputs */
		void SetEventCallback(soOISEventHandler _handler);																					/*!< Stes event handler to events */

		void SetGamepadMap();																												/*!< Sets default gamepad */
		void SetGamepadMap(std::map<int, int> _buttonMap, std::map<int, int> _axisMap, std::map<const int, int> _dpadMap);					/*!< Sets owned gamepad */
		void SetSignedActors(std::vector<SignedObject*> _signedActors);																		/*!< Sets signed actor list */

	private:
		RESULT OnStartUp(const size_t& _winHandle);																								/*!< Standard Initialize */
		virtual void OnShutDown() override;																										/*!< Standard Shutdown */

		void HandleNonBufferedKeys();																										/*!< Handles non buffered keys */
		void HandleNonBufferedMouse();																										/*!< Handles non buffered mouse */
		void HandleNonBufferedJoyStick(OIS::JoyStick* _joystick);																			/*!< Handles non buffered joystick */

	};
}