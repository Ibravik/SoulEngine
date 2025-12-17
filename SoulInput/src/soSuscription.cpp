#include "soSuscription.h"


namespace SoulSDK
{
	soSuscription::soSuscription()
	{
		Connect(std::bind(&soSuscription::OnStartUp, this));
	}


	soSuscription::~soSuscription()
	{
	}

	void soSuscription::Suscribe(soActor * _actor, OISButtonState _buttonInput, const soSignedFunction& _callbackFunction)
	{
		SignedObject* tempObj = new SignedObject();
		tempObj->actor = _actor;
		tempObj->buttonInput = _buttonInput;
		tempObj->callbackFunction = _callbackFunction;
		m_signedList.push_back(tempObj);
	}

	void soSuscription::Suscribe(soActor * _actor, OISAxisState _axisInput, const soSignedFunction& _callbackFunction)
	{
		SignedObject* tempObj = new SignedObject();
		tempObj->actor = _actor;
		tempObj->axisInput = _axisInput;
		tempObj->callbackFunction = _callbackFunction;
		m_signedList.push_back(tempObj);
	}

	void soSuscription::Suscribe(soActor * _actor, OISDPadState _dpadInput, const soSignedFunction& _callbackFunction)
	{
		SignedObject* tempObj = new SignedObject();
		tempObj->actor = _actor;
		tempObj->dpadInput = _dpadInput;
		tempObj->callbackFunction = _callbackFunction;
		m_signedList.push_back(tempObj);
	}


	std::vector<SignedObject*> soSuscription::GetSignedList()
	{
		return m_signedList;
	}
	RESULT soSuscription::OnStartUp()
	{
		return EC_OK;
	}
	void soSuscription::OnShutDown()
	{
		SAFE_DELETE_STDVECTOR(m_signedList);
	}
}