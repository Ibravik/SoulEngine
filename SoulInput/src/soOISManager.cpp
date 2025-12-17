#include "soOISManager.h"


namespace SoulSDK
{
	soOISManager::soOISManager()
	{
		Connect(std::bind(&soOISManager::OnStartUp, this, std::placeholders::_1));
		m_inputManager = NULL;
		m_keyboard = NULL;
		m_mouse = NULL;
	}

	soOISManager::~soOISManager()
	{
	}

	RESULT soOISManager::OnStartUp(const size_t& _winHandle)
	{
		//Init manager system
		m_inputManager = OIS::InputManager::createInputSystem(_winHandle);

		if (m_inputManager)
			return EC_OK;

		return EC_FALSE;
	}

	void soOISManager::OnShutDown()
	{
		OIS::InputManager::destroyInputSystem(m_inputManager);
	}
	void soOISManager::CreateComponents()
	{
		//Init keyboard
		if (m_inputManager->getNumberOfDevices(OIS::Type::OISKeyboard) > 0)
		{
			m_keyboard = static_cast<OIS::Keyboard*>(m_inputManager->createInputObject(OIS::Type::OISKeyboard, true));
			m_keyboard->setEventCallback(&m_eventHandler);
		}

		//Init mouse
		if (m_inputManager->getNumberOfDevices(OIS::Type::OISMouse) > 0)
		{
			m_mouse = static_cast<OIS::Mouse*>(m_inputManager->createInputObject(OIS::Type::OISMouse, true));
			m_mouse->setEventCallback(&m_eventHandler);
			const OIS::MouseState &ms = m_mouse->getMouseState();
			ms.width = 100;
			ms.height = 100;
		}

		//Init Joystick
		if (m_inputManager->getNumberOfDevices(OIS::Type::OISJoyStick) > 0)
		{
			uint32 numSticks = min(m_inputManager->getNumberOfDevices(OIS::Type::OISJoyStick), 4);
			for (uint32 i = 0; i < numSticks; ++i)
			{
				m_joystick[i] = static_cast<OIS::JoyStick*>(m_inputManager->createInputObject(OIS::Type::OISJoyStick, true));
				m_joystick[i]->setEventCallback(&m_eventHandler);

				/*soString jsdata = L"Creating Joystick: " + int2ws(i + 1) + L" Axis: " + int2ws(m_joystick[i]->getNumberOfComponents(OIS::OIS_Axis)) + L" Sliders: "
				+ int2ws(m_joystick[i]->getNumberOfComponents(OIS::OIS_Slider)) + L" POV/HATs: " + int2ws(m_joystick[i]->getNumberOfComponents(OIS::OIS_POV))
				+ L" Buttons: " + int2ws(m_joystick[i]->getNumberOfComponents(OIS::OIS_Button)) + L" Vector3: " + int2ws(m_joystick[i]->getNumberOfComponents(OIS::OIS_Vector3));
				OutputDebugString(jsdata.c_str());*/

				/*std::cout << "\n\nCreating Joystick " << (i + 1)
				<< "\n\tAxes: " << m_joystick[i]->getNumberOfComponents(OIS::OIS_Axis)
				<< "\n\tSliders: " << m_joystick[i]->getNumberOfComponents(OIS::OIS_Slider)
				<< "\n\tPOV/HATs: " << m_joystick[i]->getNumberOfComponents(OIS::OIS_POV)
				<< "\n\tButtons: " << m_joystick[i]->getNumberOfComponents(OIS::OIS_Button)
				<< "\n\tVector3: " << m_joystick[i]->getNumberOfComponents(OIS::OIS_Vector3);*/
			}
		}
	}
	void soOISManager::Update()
	{
		if (m_keyboard)
		{
			m_keyboard->capture();
			if (!m_keyboard->buffered())
				HandleNonBufferedKeys();
		}

		if (m_mouse)
		{
			m_mouse->capture();
			if (!m_mouse->buffered())
				HandleNonBufferedMouse();
		}

		for (SIZE_T i = 0; i < 4; ++i)
		{
			if (m_joystick[i])
			{
				m_joystick[i]->capture();
				if (!m_joystick[i]->buffered())
					HandleNonBufferedJoyStick(m_joystick[i]);
			}
		}
	}
	void soOISManager::SetEventCallback(soOISEventHandler _handler)
	{
		m_eventHandler = _handler;
	}
	void soOISManager::SetGamepadMap()
	{
		GamepadInputData tempData;
		std::map<int, int> tempButtonMap;
		std::map<int, int> tempAxisMap;
		std::map<const int, int> tempDpadMap;

		tempButtonMap.insert(std::pair<int, int>(OISBS_A, tempData.A));
		tempButtonMap.insert(std::pair<int, int>(OISBS_B, tempData.B));
		tempButtonMap.insert(std::pair<int, int>(OISBS_X, tempData.X));
		tempButtonMap.insert(std::pair<int, int>(OISBS_Y, tempData.Y));
		tempButtonMap.insert(std::pair<int, int>(OISBS_START, tempData.START));
		tempButtonMap.insert(std::pair<int, int>(OISBS_BACK, tempData.BACK));
		tempButtonMap.insert(std::pair<int, int>(OISBS_LB, tempData.LEFTBUMPER));
		tempButtonMap.insert(std::pair<int, int>(OISBS_RB, tempData.RIGHTBUMPER));
		tempButtonMap.insert(std::pair<int, int>(OISBS_LASTICK, tempData.LEFTANALOGUESTICK));
		tempButtonMap.insert(std::pair<int, int>(OISBS_RASTICK, tempData.RIGHTANALOGUESTICK));

		tempAxisMap.insert(std::pair<int, int>(OISAS_LASTICKVER, tempData.LEFTANALOGUESTICKMOVEVER));
		tempAxisMap.insert(std::pair<int, int>(OISAS_LASTICKHOR, tempData.LEFTANALOGUESTICKMOVEHOR));
		tempAxisMap.insert(std::pair<int, int>(OISAS_RASTICKVER, tempData.RIGHTANALOGUESTICKMOVEVER));
		tempAxisMap.insert(std::pair<int, int>(OISAS_RASTICKHOR, tempData.RIGHTANALOGUESTICKMOVEHOR));
		tempAxisMap.insert(std::pair<int, int>(OISAS_RLTRIGGER, tempData.LRTRIGGER));

		tempDpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSCentered, tempData.DPADCENTERED));
		tempDpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSNorth, tempData.DPADNORTH));
		tempDpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSSouth, tempData.DPADSOUTH));
		tempDpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSEast, tempData.DPADEAST));
		tempDpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSWest, tempData.DPADWEST));
		tempDpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSNorthEast, tempData.DPADNORTHEAST));
		tempDpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSSouthEast, tempData.DPADSOUTHEAST));
		tempDpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSNorthWest, tempData.DPADNORTHWEST));
		tempDpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSSouthWest, tempData.DPADSOUTHWEST));

		m_eventHandler.SetButtonMap(tempButtonMap);
		m_eventHandler.SetAxisMap(tempAxisMap);
		m_eventHandler.SetDpadMap(tempDpadMap);
	}
	void soOISManager::SetGamepadMap(std::map<int, int> _buttonMap, std::map<int, int> _axisMap, std::map<const int, int> _dpadMap)
	{
		m_eventHandler.SetButtonMap(_buttonMap);
		m_eventHandler.SetAxisMap(_axisMap);
		m_eventHandler.SetDpadMap(_dpadMap);
	}
	void soOISManager::SetSignedActors(std::vector<SignedObject*> _signedActors)
	{
		m_eventHandler.SetSignedActors(_signedActors);
	}
	void soOISManager::HandleNonBufferedKeys()
	{
		if (m_keyboard->isModifierDown(OIS::Keyboard::Shift))
			OutputDebugString(L"Shift is down..\n");
		if (m_keyboard->isModifierDown(OIS::Keyboard::Alt))
			OutputDebugString(L"Alt is down..\n");
		if (m_keyboard->isModifierDown(OIS::Keyboard::Ctrl))
			OutputDebugString(L"Ctrl is down..\n");
	}
	void soOISManager::HandleNonBufferedMouse()
	{
		//Just dump the current mouse state
		const OIS::MouseState &ms = m_mouse->getMouseState();
		soString nonBufferedMouseData = L"Mouse: Abs(" + int2ws(ms.X.abs) + L" " + int2ws(ms.Y.abs)
			+ L" " + int2ws(ms.Z.abs) + L") B: " + int2ws(ms.buttons) + L"Rel(" + int2ws(ms.X.rel) + L" " + int2ws(ms.Y.rel) + L" " + int2ws(ms.Z.rel) + L")";

		OutputDebugString(nonBufferedMouseData.c_str());

		/*std::cout << "\nMouse: Abs(" << ms.X.abs << " " << ms.Y.abs << " " << ms.Z.abs
		<< ") B: " << ms.buttons << " Rel(" << ms.X.rel << " " << ms.Y.rel << " " << ms.Z.rel << ")";*/
	}
	void soOISManager::HandleNonBufferedJoyStick(OIS::JoyStick * _joystick)
	{
		//Just dump the current joy state_joystick
		const OIS::JoyStickState &joy = _joystick->getJoyStickState();
		for (unsigned int i = 0; i < joy.mAxes.size(); ++i)
		{
			soString joydata = L"Axis " + int2ws(i) + L" X: " + int2ws(joy.mAxes[i].abs);
			OutputDebugString(joydata.c_str());
			//std::cout << "\nAxis " << i << " X: " << joy.mAxes[i].abs;
		}
	}
}