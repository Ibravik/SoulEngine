#include "soInputMap.h"


namespace SoulSDK
{
	soInputMap::soInputMap()
	{
	}


	soInputMap::~soInputMap()
	{
	}
	void soInputMap::CreateGamepadMap(GamepadInputData _data)
	{
		m_buttonMap.insert(std::pair<int, int>(OISBS_A, _data.A));
		m_buttonMap.insert(std::pair<int, int>(OISBS_B, _data.B));
		m_buttonMap.insert(std::pair<int, int>(OISBS_X, _data.X));
		m_buttonMap.insert(std::pair<int, int>(OISBS_Y, _data.Y));
		m_buttonMap.insert(std::pair<int, int>(OISBS_START, _data.START));
		m_buttonMap.insert(std::pair<int, int>(OISBS_BACK, _data.BACK));
		m_buttonMap.insert(std::pair<int, int>(OISBS_LB, _data.LEFTBUMPER));
		m_buttonMap.insert(std::pair<int, int>(OISBS_RB, _data.RIGHTBUMPER));
		m_buttonMap.insert(std::pair<int, int>(OISBS_LASTICK, _data.LEFTANALOGUESTICK));
		m_buttonMap.insert(std::pair<int, int>(OISBS_RASTICK, _data.RIGHTANALOGUESTICK));

		m_axisMap.insert(std::pair<int, int>(OISAS_LASTICKVER, _data.LEFTANALOGUESTICKMOVEVER));
		m_axisMap.insert(std::pair<int, int>(OISAS_LASTICKHOR, _data.LEFTANALOGUESTICKMOVEHOR));
		m_axisMap.insert(std::pair<int, int>(OISAS_RASTICKVER, _data.RIGHTANALOGUESTICKMOVEVER));
		m_axisMap.insert(std::pair<int, int>(OISAS_RASTICKHOR, _data.RIGHTANALOGUESTICKMOVEHOR));
		m_axisMap.insert(std::pair<int, int>(OISAS_RLTRIGGER, _data.LRTRIGGER));

		m_dpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSCentered, _data.DPADCENTERED));
		m_dpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSNorth, _data.DPADNORTH));
		m_dpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSSouth, _data.DPADSOUTH));
		m_dpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSEast, _data.DPADEAST));
		m_dpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSWest, _data.DPADWEST));
		m_dpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSNorthEast, _data.DPADNORTHEAST));
		m_dpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSSouthEast, _data.DPADSOUTHEAST));
		m_dpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSNorthWest, _data.DPADNORTHWEST));
		m_dpadMap.insert(std::pair<const int, int>(OISDPadState::OISDSSouthWest, _data.DPADSOUTHWEST));
	}
	std::map<int, int> soInputMap::GetButtonMap()
	{
		return m_buttonMap;
	}
	std::map<int, int> soInputMap::GetAxisMap()
	{
		return m_axisMap;
	}
	std::map<const int, int> soInputMap::GetDpadMap()
	{
		return m_dpadMap;
	}
}
