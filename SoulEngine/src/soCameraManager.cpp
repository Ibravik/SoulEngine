/********************************************************************/
/**
* @LC	  11/30/2016
* @file   soCameraManager.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   11/16/2015
* @brief  Implementacion de la clase soCameraManager
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soCamera.h"
#include "soCameraManager.h"

/************************************************************************/
/* Definicion de la clase soCameraManager							    */
/************************************************************************/
namespace SoulSDK
{
	soCameraManager::soCameraManager()
	{
		Connect(std::bind(&soCameraManager::OnStartUp, this));
		m_ActiveCamera = NULL;
		m_CameraList.clear();
		m_CameraMap.clear();
	}

	soCameraManager::~soCameraManager()
	{
	}

	RESULT soCameraManager::OnStartUp()
	{
		return EC_OK;
	}

	soCamera* soCameraManager::CreateStandardCamera(CameraData& _CameraData)
	{
		//Primero verificamos que ninguna camara ya creada coincida con la camara que se solicita crear en nombre
		soCamera* Camera = ChangeTo(_CameraData.CameraName);

		if (Camera != NULL)
		{
			return Camera;
		}

		//Creamos la nueva camara
		Camera = SOE_NEW soCamera;

		//Seteamos la informacion de la camara y la inicializamos
		if (FAIL(Camera->StartUp(_CameraData)))
		{
			SAFE_DELETE(Camera);
			return NULL;
		}

		//Una vez inicializada, la añadimos a la lista local de camaras
		m_CameraMap[_CameraData.CameraName] = static_cast<uint32>(m_CameraList.size());
		Camera->m_ID = m_CameraList.size();

		m_CameraList[Camera->m_ID] = Camera;
		//Retornamos la camara creada
		return Camera;
	}

	soEditorCamera* soCameraManager::CreateEditorCamera(CameraData& _CameraData)
	{
		//Primero verificamos que ninguna camara ya creada coincida con la camara que se solicita crear en nombre
		soCamera* Camera = ChangeTo(_CameraData.CameraName);

		if (Camera != NULL || dynamic_cast<soEditorCamera*>(Camera) != NULL)
		{
			return dynamic_cast<soEditorCamera*>(Camera);
		}

		//Creamos la nueva camara
		Camera = SOE_NEW soEditorCamera;

		//Seteamos la informacion de la camara y la inicializamos
		if (FAIL(Camera->StartUp(_CameraData)))
		{
			SAFE_DELETE(Camera);
			return NULL;
		}

		//Una vez inicializada, la añadimos a la lista local de camaras
		m_CameraMap[_CameraData.CameraName] = static_cast<uint32>(m_CameraList.size());
		Camera->m_ID = m_CameraList.size();

		m_CameraList[Camera->m_ID] = Camera;
		//Retornamos la camara creada
		return dynamic_cast<soEditorCamera*>(Camera);
	}

	RESULT soCameraManager::DeleteCamera(soString _CameraNameID)
	{
		//Primero verificamos que la camara exista
		if (m_CameraMap.find(_CameraNameID) != m_CameraMap.end())
		{
			uint32 Index = m_CameraMap.at(_CameraNameID);
			SAFE_DELETE(m_CameraList.at(Index));
			return EC_OK;
		}

		return EC_FALSE;
	}

	soCamera* soCameraManager::ChangeTo(uint32 _CameraNameID)
	{
		//Primero verificamos que la camara exista
		if (m_CameraList.find(_CameraNameID) != m_CameraList.end())
		{			
			m_ActiveCamera = m_CameraList.at(_CameraNameID);

			return m_ActiveCamera;
		}
		return NULL;
	}

	soCamera* soCameraManager::ChangeTo(soString _CameraNameID)
	{
		//Primero verificamos que la camara exista
		if (m_CameraMap.find(_CameraNameID) != m_CameraMap.end())
		{
			uint32 Index = m_CameraMap.at(_CameraNameID);
			//Retornamos la camara 
			m_ActiveCamera = m_CameraList.at(Index);

			return m_ActiveCamera;
		}
		return NULL;
	}

	void soCameraManager::Update(float _DeltaTime)
	{
		for (unsigned int i = 0; i < m_CameraList.size(); i++)
		{
			m_CameraList.at(i)->Update(_DeltaTime);
		}
	}
		
	void soCameraManager::Resize()
	{
		for (unsigned int i = 0; i < m_CameraList.size(); i++)
		{
			m_CameraList.at(i)->Resize();
		}
	}

	void soCameraManager::OnShutDown()
	{
		std::map<uint32, soCamera*>::iterator it = m_CameraList.begin();
		while (it != m_CameraList.end()) 
		{
			SAFE_DELETE((*it).second);
			m_CameraList.erase(it);
			it = m_CameraList.begin();
		}
	}
}
