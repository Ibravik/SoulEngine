#pragma once

#include "soState.h"

namespace SoulSDK
{
	class soFSM
	{
	public:
		soFSM();
		~soFSM();

	private:
		soState *m_currentState;
		std::vector<soState*> m_stateList;

	public:
		void AddState(soString _satateName, soStateFunction _callbackState);
		void AssignTransition(soString _stateName, soTransitionFunction _calbackTransition, soString _stateChangeName);
		void Update();
		void SetInitState(soString _state);
		void Destroy();

	private:
		bool IsState(soString _stateName);
	};
}
