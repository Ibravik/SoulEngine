/********************************************************************/
/**
* @LC     10/11/2016
* @file   soResource.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   6/8/2016
* @brief  Implementacion de la clase soResource
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soResource.h"

/************************************************************************/
/* Definicion de la clase soResource                                    */
/************************************************************************/
namespace SoulSDK
{
	soResource::soResource()
	{
		m_nID = 0;																		
		m_szResourceName = TEXT("");													
		m_szFilePath = TEXT("");														
		m_szFileName = TEXT("");														
		m_References = 0;
	}

	soResource::~soResource()
	{
		ShutDown();
	}

	void soResource::CreateFileName(soString _FilePath)
	{
		//Para encontrar el nombre de archivo, primero buscamos 
		//de atras hacia adelante hasta encontrar \

		size_t CheckPoint = 0;

		for (int32 i = static_cast<int32>(_FilePath.size()) - 1; i >= 0; i--)
		{
			if (_FilePath.at(i) == L'\\')
			{//Si encontramos el signo, guardamos el lugar siguiente, que es
			 //de donde comenzara el nombre
				CheckPoint = i + 1;
				break;
			}
		}

		for (; CheckPoint < _FilePath.size(); CheckPoint++)
		{
			m_szFileName.push_back(_FilePath.at(CheckPoint));
		}
	}

	void soResource::AssignResourceData(const ResourceParameters & _ResourceParameters)
	{		
		m_szResourceName = _ResourceParameters.ResourceName;
		m_szFilePath = _ResourceParameters.FilePath;
		CreateFileName(m_szFilePath);
		m_ResourceType = _ResourceParameters.ResourceType;
	}

	RESULT soResource::StartUp()
	{		
		return EC_OK;
	}

	void soResource::ShutDown()
	{
	}

	RESULT soResource::Load(const ResourceParameters & _ResourceParameters)
	{
		AssignResourceData(_ResourceParameters);
		return EC_OK;
	}

	RESULT soResource::Create(const ResourceParameters & _ResourceParameters)
	{
		AssignResourceData(_ResourceParameters);
		return EC_OK;
	}
}