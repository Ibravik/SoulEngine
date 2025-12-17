#pragma once

#include "Soul_StandardTools.h"

namespace SoulSDK
{
	typedef std::function <bool(void)> soTransitionFunction;
	typedef std::function<void(void)> soStateFunction;


	struct Transition
	{
		soString name;
		soTransitionFunction callbackFunction;

		Transition() {}
	};

	class soState
	{
	public:
		soState();
		virtual ~soState();

		private:
			static uint32 m_stateCount;
			uint32 m_ID;
			soString m_name;
			soStateFunction m_callbackFucntion;
			Transition m_currentTransition;
			std::vector<Transition*> m_transitionList;

		public:
			virtual void OnEnter() {}
			virtual void OnExit() {}

			void Initialize();
			void AddTransition(soTransitionFunction _calbackTransition, soString _stateName);
			bool IsTransition();
			void Execute();
			void SetData(soString& _name, soStateFunction _callbackFunction);
			
			uint32 GetID();
			soString GetName();
			soString GetTransition();
	};
}