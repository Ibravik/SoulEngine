#include "soState.h"


namespace SoulSDK
{
	soState::soState()
	{
	}


	soState::~soState()
	{
		SAFE_DELETE_STDVECTOR(m_transitionList);
	}

	uint32 soState::m_stateCount = 0;

	void soState::Execute()
	{
		m_callbackFucntion();
	}

	void soState::SetData(soString& _name, soStateFunction _callbackFunction)
	{
		m_name = _name;
		m_callbackFucntion = _callbackFunction;
	}

	void soState::Initialize()
	{
		m_stateCount++;
		m_ID = m_stateCount;
	}

	void soState::AddTransition(soTransitionFunction _calbackTransition, soString _stateName)
	{
		Transition* tempTransition = new Transition();
		tempTransition->name = _stateName;
		tempTransition->callbackFunction = _calbackTransition;

		m_transitionList.push_back(tempTransition);
	}

	bool soState::IsTransition()
	{
		for (SIZE_T i = 0; i < m_transitionList.size(); i++)
		{
			if (m_transitionList[i]->callbackFunction())
			{
				m_currentTransition = *m_transitionList[i];
				return true;
			}
		}

		return false;
	}

	uint32 soState::GetID()
	{
		return m_ID;
	}

	soString soState::GetName()
	{
		return m_name;
	}
	soString soState::GetTransition()
	{
		return m_currentTransition.name;
	}
}
