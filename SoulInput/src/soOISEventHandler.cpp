#include "soOISEventHandler.h"

namespace SoulSDK
{
	soOISEventHandler::soOISEventHandler()
	{
	}


	soOISEventHandler::~soOISEventHandler()
	{
	}
	bool soOISEventHandler::keyPressed(const OIS::KeyEvent & arg)
	{
		soString keyData = L"KeyPressed {" + int2ws(arg.key) + L", " + s2ws(((OIS::Keyboard*)(arg.device))->getAsString(arg.key)) + L"} || Characters (" + int2ws(arg.text) + L")\n";
		OutputDebugString(keyData.c_str());



		return true;
	}
	bool soOISEventHandler::keyReleased(const OIS::KeyEvent & arg)
	{
		soString keyData = L"KeyReleased {" + s2ws(((OIS::Keyboard*)(arg.device))->getAsString(arg.key)) + L"}\n";
		OutputDebugString(keyData.c_str());



		return true;
	}
	bool soOISEventHandler::mouseMoved(const OIS::MouseEvent & arg)
	{
		const OIS::MouseState& s = arg.state;
		soString mouseData = L"MouseMoved: Abs(" + int2ws(s.X.abs) + L", " + int2ws(s.Y.abs) + L", "
			+ int2ws(s.Z.abs) + L") Rel(" + int2ws(s.X.rel) + L", " + int2ws(s.Y.rel) + L", " + int2ws(s.Z.rel) + L")";
		OutputDebugString(mouseData.c_str());



		return true;
	}
	bool soOISEventHandler::mousePressed(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
	{
		const OIS::MouseState& s = arg.state;
		soString mouseData = L"Mouse button #" + int2ws(id) + L"pressed. Abs(" + int2ws(s.X.abs) + L", "
			+ int2ws(s.Y.abs) + L", " + int2ws(s.Z.abs) + L") Rel(" + int2ws(s.X.rel) + L", " + int2ws(s.Y.rel) + L", " + int2ws(s.Z.rel) + L")";
		OutputDebugString(mouseData.c_str());

		return true;
	}
	bool soOISEventHandler::mouseReleased(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
	{
		const OIS::MouseState& s = arg.state;
		soString mouseData = L"Mouse button #" + int2ws(id) + L"released. Abs(" + int2ws(s.X.abs) + L", "
			+ int2ws(s.Y.abs) + L", " + int2ws(s.Z.abs) + L") Rel(" + int2ws(s.X.rel) + L", " + int2ws(s.Y.rel) + L", " + int2ws(s.Z.rel) + L")";
		OutputDebugString(mouseData.c_str());


		return true;
	}
	bool soOISEventHandler::buttonPressed(const OIS::JoyStickEvent & arg, int button)
	{
		soString buttonData = s2ws(arg.device->vendor()) + L". Button Pressed # " + int2ws(button);
		OutputDebugString(buttonData.c_str());

		uint32 tempInputCheck = m_buttonMap.find(button)->second;

		switch (tempInputCheck)
		{
		case OISBS_A:
			for (SIZE_T i = 0; i < m_signedList.size(); i++)
			{
				if (m_signedList[i]->buttonInput == OISBS_A)
				{
					m_signedList[i]->callbackFunction();
				}
			}
			break;
		case OISBS_B:
			for (SIZE_T i = 0; i < m_signedList.size(); i++)
			{
				if (m_signedList[i]->buttonInput == OISBS_B)
				{
					m_signedList[i]->callbackFunction();
				}
			}
			break;
		case OISBS_X:
			for (SIZE_T i = 0; i < m_signedList.size(); i++)
			{
				if (m_signedList[i]->buttonInput == OISBS_X)
				{
					m_signedList[i]->callbackFunction();
				}
			}
			break;
		case OISBS_Y:
			for (SIZE_T i = 0; i < m_signedList.size(); i++)
			{
				if (m_signedList[i]->buttonInput == OISBS_Y)
				{
					m_signedList[i]->callbackFunction();
				}
			}
			break;
		case OISBS_RB:
			for (SIZE_T i = 0; i < m_signedList.size(); i++)
			{
				if (m_signedList[i]->buttonInput == OISBS_RB)
				{
					m_signedList[i]->callbackFunction();
				}
			}
			break;
		case OISBS_LB:
			for (SIZE_T i = 0; i < m_signedList.size(); i++)
			{
				if (m_signedList[i]->buttonInput == OISBS_LB)
				{
					m_signedList[i]->callbackFunction();
				}
			}
			break;
		case OISBS_BACK:
			for (SIZE_T i = 0; i < m_signedList.size(); i++)
			{
				if (m_signedList[i]->buttonInput == OISBS_BACK)
				{
					m_signedList[i]->callbackFunction();
				}
			}
			break;
		case OISBS_START:
			for (SIZE_T i = 0; i < m_signedList.size(); i++)
			{
				if (m_signedList[i]->buttonInput == OISBS_START)
				{
					m_signedList[i]->callbackFunction();
				}
			}
			break;
		case OISBS_LASTICK:
			for (SIZE_T i = 0; i < m_signedList.size(); i++)
			{
				if (m_signedList[i]->buttonInput == OISBS_LASTICK)
				{
					m_signedList[i]->callbackFunction();
				}
			}
			break;
		case OISBS_RASTICK:
			for (SIZE_T i = 0; i < m_signedList.size(); i++)
			{
				if (m_signedList[i]->buttonInput == OISBS_RASTICK)
				{
					m_signedList[i]->callbackFunction();
				}
			}
			break;
		default:
			break;
		}

		return true;
	}
	bool soOISEventHandler::buttonReleased(const OIS::JoyStickEvent & arg, int button)
	{
		soString buttonData = s2ws(arg.device->vendor()) + L". Button Released # " + int2ws(button);
		OutputDebugString(buttonData.c_str());


		return true;
	}
	bool soOISEventHandler::axisMoved(const OIS::JoyStickEvent & arg, int axis)
	{
		//Provide a little dead zone
		if (arg.state.mAxes[axis].abs > 2500 || arg.state.mAxes[axis].abs < -2500)
		{
			soString axisData = /*s2ws(arg.device->vendor()) +*/ L". Axis # " + int2ws(axis) + L" Value: " + int2ws(arg.state.mAxes[axis].abs);
			OutputDebugString(axisData.c_str());

		}

		uint32 tempInputCheck = m_buttonMap.find(axis)->second;

		switch (tempInputCheck)
		{
		case OISAS_LASTICKVER:
			for (SIZE_T i = 0; i < m_signedList.size(); i++)
			{
				if (m_signedList[i]->buttonInput == OISAS_LASTICKVER)
				{
					m_signedList[i]->callbackFunction();
				}
			}
			break;
		case OISAS_LASTICKHOR:
			for (SIZE_T i = 0; i < m_signedList.size(); i++)
			{
				if (m_signedList[i]->buttonInput == OISAS_LASTICKHOR)
				{
					m_signedList[i]->callbackFunction();
				}
			}
			break;
		case OISAS_RASTICKVER:
			for (SIZE_T i = 0; i < m_signedList.size(); i++)
			{
				if (m_signedList[i]->buttonInput == OISAS_RASTICKVER)
				{
					m_signedList[i]->callbackFunction();
				}
			}
			break;
		case OISAS_RASTICKHOR:
			for (SIZE_T i = 0; i < m_signedList.size(); i++)
			{
				if (m_signedList[i]->buttonInput == OISAS_RASTICKHOR)
				{
					m_signedList[i]->callbackFunction();
				}
			}
			break;
		case OISAS_RLTRIGGER:
			for (SIZE_T i = 0; i < m_signedList.size(); i++)
			{
				if (m_signedList[i]->buttonInput == OISAS_RLTRIGGER)
				{
					m_signedList[i]->callbackFunction();
				}
			}
			break;
		default:
			break;
		}

		return true;
	}
	bool soOISEventHandler::povMoved(const OIS::JoyStickEvent & arg, int pov)
	{
		soString povData = s2ws(arg.device->vendor()) + L". POV" + int2ws(pov) + L" ";
		OutputDebugString(povData.c_str());

		if (arg.state.mPOV[pov].direction & OISDPadState::OISDSNorth) //Going up
			OutputDebugString(L"North");
		else if (arg.state.mPOV[pov].direction & OISDPadState::OISDSSouth) //Going down
			OutputDebugString(L"South");

		if (arg.state.mPOV[pov].direction & OISDPadState::OISDSEast) //Going right
			OutputDebugString(L"East");
		else if (arg.state.mPOV[pov].direction & OISDPadState::OISDSWest) //Going left
			OutputDebugString(L"West");

		/*if (arg.state.mPOV[pov].direction & OISDPadState::OISDSNorthEast)
		{
		OutputDebugString(L"NorthEast");
		}

		if (arg.state.mPOV[pov].direction & OISDPadState::OISDSSouthEast)
		{
		OutputDebugString(L"SouthEast");
		}

		if (arg.state.mPOV[pov].direction & OISDPadState::OISDSNorthWest)
		{
		OutputDebugString(L"NorthWest");
		}

		if (arg.state.mPOV[pov].direction & OISDPadState::OISDSSouthWest)
		{
		OutputDebugString(L"SouthWest");
		}*/

		if (arg.state.mPOV[pov].direction == OISDPadState::OISDSCentered) //stopped/centered out
			OutputDebugString(L"Centered");

		return true;
	}
	bool soOISEventHandler::vector3Moved(const OIS::JoyStickEvent & arg, int index)
	{
		soString vectorData = s2ws(arg.device->vendor()) + L". Orientation # " + int2ws(index) + L" X Value: " + float2ws(arg.state.mVectors[index].x)
			+ L" Y Value: " + float2ws(arg.state.mVectors[index].y) + L" Z Value: " + float2ws(arg.state.mVectors[index].z);
		OutputDebugString(vectorData.c_str());

		return true;
	}
	void soOISEventHandler::SetButtonMap(std::map<int, int> _buttonMap)
	{
		m_buttonMap = _buttonMap;
	}
	void soOISEventHandler::SetAxisMap(std::map<int, int> _axisMap)
	{
		m_axisMap = _axisMap;
	}
	void soOISEventHandler::SetDpadMap(std::map<const int, int> _dpadMap)
	{
		m_dpadMap = _dpadMap;
	}
	void soOISEventHandler::SetSignedActors(std::vector<SignedObject*> _signedActors)
	{
		m_signedList = _signedActors;
	}

}
