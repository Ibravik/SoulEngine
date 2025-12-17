#include "soFiniteStateMachine.h"

namespace SoulSDK
{
	soFSM::soFSM()
	{}

	soFSM::~soFSM()
	{}

	void soFSM::AddState(soString _satateName, soStateFunction _callbackState)
	{
		soState* temp = new soState();

		temp->Initialize();

		temp->SetData(_satateName, _callbackState);

		m_stateList.push_back(temp);
	}

	void soFSM::AssignTransition(soString _stateName, soTransitionFunction _calbackTransition, soString _stateChangeName)
	{
		//Checamos si los 2 estados ingresados existen sino regresamos un aviso
		if (IsState(_stateName) && IsState(_stateChangeName))
		{
			//Aisgnamos la transicion al primer estado
			for (SIZE_T i = 0; i < m_stateList.size(); i++)
			{
				if (m_stateList[i]->GetName() == _stateName)
				{
					m_stateList[i]->AddTransition(_calbackTransition, _stateChangeName);
				}
			}
		}
	}

	void soFSM::Update()
	{
		//Si tenemos un estado
		if (m_currentState != NULL)
		{
			//Si hay un cambio de transicion
			if (m_currentState->IsTransition())
			{
				for (SIZE_T i = 0; i < m_stateList.size(); i++)
				{
					//Buscamos el nuevo estado
					if (m_stateList[i]->GetName() == m_currentState->GetTransition())
					{
						//Llamamos la funcion exit del estado actual
						m_currentState->OnExit();

						//lo ponemos como current
						m_currentState = m_stateList[i];

						//Llamamos la funcion enter del nuevo estado
						m_currentState->OnEnter();
					}
				}
			}

			//lo ejecutamos
			m_currentState->Execute();
		}
	}

	void soFSM::SetInitState(soString _state)
	{
		//Checar si existe el estado
		if (IsState(_state))
		{
			//Asignamos el estado
			for (SIZE_T i = 0; i < m_stateList.size(); i++)
			{
				if (m_stateList[i]->GetName() == _state)
				{
					m_currentState = m_stateList[i];
				}
			}
		}
	}

	void soFSM::Destroy()
	{
		SAFE_DELETE_STDVECTOR(m_stateList);
	}

	bool soFSM::IsState(soString _stateName)
	{
		for (SIZE_T i = 0; i < m_stateList.size(); i++)
		{
			if (m_stateList[i]->GetName() == _stateName)
			{
				return true;
			}
		}
		return false;
	}

}