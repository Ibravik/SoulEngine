/************************************************************************/
/**
* @LC	  14/10/2016
* @file   soSystemMessage.h
* @Author Raul Portillo (ruulaasz_@hotmail.com)
* @date   14/10/2016
* @brief  Declaration of base class soSystemMessage and all its child classes
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "Soul_Core.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaración del enum SystemMessageTag								*/
	/************************************************************************/
	enum SystemMessageTag
	{
		MT_SET3DAUDIOSETTINGS,
		MT_ADDAUDIOLISTENER,
		MT_DELETEAUDIOLISTENER,
		MT_SETMASTERVOLUME,
		MT_MUTEMASTER,
		MT_UNMUTEMASTER,
		MT_SETMASTERPITCH,
		MT_SETMASTERPAUSE,
		MT_STOPMASTER,
		MT_SETGROUPVOLUME,
		MT_MUTEGROUP,
		MT_UNMUTEGROUP,
		MT_SETGROUPPITCH,
		MT_SETGROUPPAUSE,
		MT_STOPGROUP
	};

	/************************************************************************/
	/* Declaracion de la clase soSystemMessage				 				*/
	/************************************************************************/
	class soSystemMessage : public soStandardClass<>
	{
	public:
		soSystemMessage();
		~soSystemMessage();

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
		SystemMessageTag m_MessageTag;

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	protected:
		RESULT OnStartUp();										/*!< Inicializa los parametros del componente */
		virtual void OnShutDown() override;						/*!< Destruye los parametros del componente  */
	};

	/************************************************************************/
	/* Declaracion de la clase SM_Set3DAudioSettings				 		*/
	/************************************************************************/
	class SM_Set3DAudioSettings : public soSystemMessage
	{
	public:
		SM_Set3DAudioSettings() { m_MessageTag = MT_SET3DAUDIOSETTINGS; };
		~SM_Set3DAudioSettings() { ShutDown(); };

		float DopplerScale;
		float RollOffScale;

	private:
		RESULT OnStartUp() { return SUPER::OnStartUp(); };				/*!< Inicializa los parametros del componente */
		virtual void OnShutDown() override { SUPER::OnShutDown(); };	/*!< Destruye los parametros del componente  */
	};

	/************************************************************************/
	/* Declaracion de la clase SM_AddAudioListener				 			*/
	/************************************************************************/
	class SM_AddAudioListener : public soSystemMessage
	{
	public:
		SM_AddAudioListener() { m_MessageTag = MT_ADDAUDIOLISTENER; Listener = SOE_NEW soListener(); };
		~SM_AddAudioListener() { ShutDown(); };

		soListener* Listener;

	private:
		RESULT OnStartUp() { return SUPER::OnStartUp(); };				/*!< Inicializa los parametros del componente */
		virtual void OnShutDown() override { SUPER::OnShutDown(); SAFE_DELETE(Listener); };	/*!< Destruye los parametros del componente  */
	};

	/************************************************************************/
	/* Declaracion de la clase SM_SetMasterVolume				 			*/
	/************************************************************************/
	class SM_SetMasterVolume : public soSystemMessage
	{
	public:
		SM_SetMasterVolume() { m_MessageTag = MT_SETMASTERVOLUME; };
		~SM_SetMasterVolume() { ShutDown(); };

		float Volume;

	private:
		RESULT OnStartUp() { return SUPER::OnStartUp(); };				/*!< Inicializa los parametros del componente */
		virtual void OnShutDown() override { SUPER::OnShutDown(); };	/*!< Destruye los parametros del componente  */
	};

	/************************************************************************/
	/* Declaracion de la clase SM_SetMasterPitch				 			*/
	/************************************************************************/
	class SM_SetMasterPitch : public soSystemMessage
	{
	public:
		SM_SetMasterPitch() { m_MessageTag = MT_SETMASTERPITCH; };
		~SM_SetMasterPitch() { ShutDown(); };

		float Pitch;

	private:
		RESULT OnStartUp() { return SUPER::OnStartUp(); };				/*!< Inicializa los parametros del componente */
		virtual void OnShutDown() override { SUPER::OnShutDown(); };	/*!< Destruye los parametros del componente  */
	};

	/************************************************************************/
	/* Declaracion de la clase SM_MuteMaster					 			*/
	/************************************************************************/
	class SM_MuteMaster : public soSystemMessage
	{
	public:
		SM_MuteMaster() { m_MessageTag = MT_MUTEMASTER; };
		~SM_MuteMaster() { ShutDown(); };

	private:
		RESULT OnStartUp() { return SUPER::OnStartUp(); };				/*!< Inicializa los parametros del componente */
		virtual void OnShutDown() override { SUPER::OnShutDown(); };	/*!< Destruye los parametros del componente  */
	};

	/************************************************************************/
	/* Declaracion de la clase SM_UnMuteMaster					 			*/
	/************************************************************************/
	class SM_UnMuteMaster : public soSystemMessage
	{
	public:
		SM_UnMuteMaster() { m_MessageTag = MT_UNMUTEMASTER; };
		~SM_UnMuteMaster() { ShutDown(); };

	private:
		RESULT OnStartUp() { return SUPER::OnStartUp(); };				/*!< Inicializa los parametros del componente */
		virtual void OnShutDown() override { SUPER::OnShutDown(); };	/*!< Destruye los parametros del componente  */
	};

	/************************************************************************/
	/* Declaracion de la clase SM_SetMasterPaused				 			*/
	/************************************************************************/
	class SM_SetMasterPaused : public soSystemMessage
	{
	public:
		SM_SetMasterPaused() { m_MessageTag = MT_SETMASTERPAUSE; };
		~SM_SetMasterPaused() { ShutDown(); };

		bool Paused;

	private:
		RESULT OnStartUp() { return SUPER::OnStartUp(); };				/*!< Inicializa los parametros del componente */
		virtual void OnShutDown() override { SUPER::OnShutDown(); };	/*!< Destruye los parametros del componente  */
	};

	/************************************************************************/
	/* Declaracion de la clase SM_StopMaster					 			*/
	/************************************************************************/
	class SM_StopMaster : public soSystemMessage
	{
	public:
		SM_StopMaster() { m_MessageTag = MT_STOPMASTER; };
		~SM_StopMaster() { ShutDown(); };

	private:
		RESULT OnStartUp() { return SUPER::OnStartUp(); };				/*!< Inicializa los parametros del componente */
		virtual void OnShutDown() override { SUPER::OnShutDown(); };	/*!< Destruye los parametros del componente  */
	};

	/************************************************************************/
	/* Declaracion de la clase SM_SetGroupVolume				 			*/
	/************************************************************************/
	class SM_SetGroupVolume : public soSystemMessage
	{
	public:
		SM_SetGroupVolume() { m_MessageTag = MT_SETGROUPVOLUME; };
		~SM_SetGroupVolume() { ShutDown(); };

		soString GroupName;
		float Volume;

	private:
		RESULT OnStartUp() { return SUPER::OnStartUp(); };				/*!< Inicializa los parametros del componente */
		virtual void OnShutDown() override { SUPER::OnShutDown(); };	/*!< Destruye los parametros del componente  */
	};

	/************************************************************************/
	/* Declaracion de la clase SM_SetGroupPitch					 			*/
	/************************************************************************/
	class SM_SetGroupPitch : public soSystemMessage
	{
	public:
		SM_SetGroupPitch() { m_MessageTag = MT_SETGROUPPITCH; };
		~SM_SetGroupPitch() { ShutDown(); };

		soString GroupName;
		float Pitch;

	private:
		RESULT OnStartUp() { return SUPER::OnStartUp(); };				/*!< Inicializa los parametros del componente */
		virtual void OnShutDown() override { SUPER::OnShutDown(); };	/*!< Destruye los parametros del componente  */
	};

	/************************************************************************/
	/* Declaracion de la clase SM_MuteGroup						 			*/
	/************************************************************************/
	class SM_MuteGroup : public soSystemMessage
	{
	public:
		SM_MuteGroup() { m_MessageTag = MT_MUTEGROUP; };
		~SM_MuteGroup() { ShutDown(); };

		soString GroupName;

	private:
		RESULT OnStartUp() { return SUPER::OnStartUp(); };				/*!< Inicializa los parametros del componente */
		virtual void OnShutDown() override { SUPER::OnShutDown(); };	/*!< Destruye los parametros del componente  */
	};

	/************************************************************************/
	/* Declaracion de la clase SM_UnMuteGroup					 			*/
	/************************************************************************/
	class SM_UnMuteGroup : public soSystemMessage
	{
	public:
		SM_UnMuteGroup() { m_MessageTag = MT_UNMUTEGROUP; };
		~SM_UnMuteGroup() { ShutDown(); };

		soString GroupName;

	private:
		RESULT OnStartUp() { return SUPER::OnStartUp(); };				/*!< Inicializa los parametros del componente */
		virtual void OnShutDown() override { SUPER::OnShutDown(); };	/*!< Destruye los parametros del componente  */
	};

	/************************************************************************/
	/* Declaracion de la clase SM_SetGroupPaused				 			*/
	/************************************************************************/
	class SM_SetGroupPaused : public soSystemMessage
	{
	public:
		SM_SetGroupPaused() { m_MessageTag = MT_SETGROUPPAUSE; };
		~SM_SetGroupPaused() { ShutDown(); };

		soString GroupName;
		bool Paused;

	private:
		RESULT OnStartUp() { return SUPER::OnStartUp(); };				/*!< Inicializa los parametros del componente */
		virtual void OnShutDown() override { SUPER::OnShutDown(); };	/*!< Destruye los parametros del componente  */
	};

	/************************************************************************/
	/* Declaracion de la clase SM_StopGroup						 			*/
	/************************************************************************/
	class SM_StopGroup : public soSystemMessage
	{
	public:
		SM_StopGroup() { m_MessageTag = MT_STOPGROUP; };
		~SM_StopGroup() { ShutDown(); };

		soString GroupName;

	private:
		RESULT OnStartUp() { return SUPER::OnStartUp(); };				/*!< Inicializa los parametros del componente */
		virtual void OnShutDown() override { SUPER::OnShutDown(); };	/*!< Destruye los parametros del componente  */
	};
}
