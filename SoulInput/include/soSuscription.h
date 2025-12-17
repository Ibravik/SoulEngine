/************************************************************************/
/**
* @LC	  10/20/2016
* @file   soSuscription.h
* @Author Rubén Rodríguez Serrano (rubn2113@gmail.com)
* @date   10/5/2016
* @brief  Class in charge of suscribe actors to an input
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Headers												                */
/************************************************************************/
#include <Soul_Math.h>
#include <Soul_Engine.h>
#include "soInputMap.h"

namespace SoulSDK
{
	typedef std::function <void()> soSignedFunction;																				/*!< Typedef to callback functions for actors */

		/************************************************************************/
		/* Struct data for actor suscrption|					                */
		/************************************************************************/
	struct SignedObject
	{
		soActor* actor;																												/*!< Pointer to an object that will be singed */
		OISButtonState buttonInput;																									/*!< Button that actor is singed */
		OISAxisState axisInput;																										/*!< Axis that actor is singed */
		OISDPadState dpadInput;																										/*!< Dpad that actor is singed */
		soSignedFunction callbackFunction;																							/*!< Pointer to callback function */

		SignedObject() {}
	};

	/************************************************************************/
	/* soSuscription class declaration						                */
	/************************************************************************/
	class soSuscription : public soModule<soSuscription>
	{
		/************************************************************************/
		/* Constructor and Destructor declaration	                            */
		/************************************************************************/
	public:
		soSuscription();																											/*!< Standard Constructor */
		virtual ~soSuscription();																									/*!< Standard Destructor */

		/************************************************************************/
		/* soSuscription member variables	 		                            */
		/************************************************************************/
	private:
		std::vector<SignedObject*> m_signedList;																					/*!< List for signed actors */

		/************************************************************************/
		/* soSuscription functions			                                    */
		/************************************************************************/
	public:
		void Suscribe(soActor* _actor, OISButtonState _buttonInput, const soSignedFunction& _callbackFunction);						/*!< This function suscribe an actor to a button input */
		void Suscribe(soActor* _actor, OISAxisState _axisInput, const soSignedFunction& _callbackFunction);							/*!< This function suscribe an actor to a axis input */
		void Suscribe(soActor* _actor, OISDPadState _dpadInput, const soSignedFunction& _callbackFunction);							/*!< This function suscribe an actor to a dpad input */

		std::vector<SignedObject*> GetSignedList();																					/*!< Gets a list of signed actors */

	private:
		RESULT OnStartUp();																											/*!< Standard Initialize */
		virtual void OnShutDown() override;																							/*!< Standard Shutdown */

	};
}