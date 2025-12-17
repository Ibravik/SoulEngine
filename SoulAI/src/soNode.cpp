/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soNode.h
* @Author Rubén Rodríguez Serrano & Lehi Muñoz (rubn2113@gmail.com / lehi.munoz@gmail.com)
* @date   09/06/2016
* @brief  Implementacion de la clase soScenneNode
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soNode.h"

/************************************************************************/
/* Definicion de la clase soNode									    */
/************************************************************************/
namespace SoulSDK
{
	soNode::soNode()
	{
		m_prev = NULL;
		m_next = NULL;
		m_ID = 0;
		m_visited = false;
	}

	soNode::~soNode()
	{		
	}

	void soNode::Visit(soNode * _prev)
	{
		m_prev = _prev;
		m_visited = true;
	}

	bool soNode::IsVisited()
	{
		return m_visited;
	}

	RESULT soNode::AddConexions(soNode* _conexion)
	{
		if (_conexion == NULL)
		{
			return EC_FALSE;
		}

		m_conexions.push_back(_conexion);

		return EC_OK;
	}

	RESULT soNode::RemoveConexionByName(soString& _name)
	{
		for (uint32 i = 0; i < static_cast<uint32>(m_conexions.size()); i++)
		{
			if (m_conexions.at(i)->m_name == _name)
			{
				m_conexions.erase(m_conexions.begin() + i);
				return EC_OK;
			}
		}
		return EC_FALSE;
	}

	RESULT soNode::RemoveConexionByID(uint32 _ID)
	{
		for (uint32 i = 0; i < static_cast<uint32>(m_conexions.size()); i++)
		{
			if (m_conexions.at(i)->m_ID == _ID)
			{
				m_conexions.erase(m_conexions.begin() + i);
				return EC_OK;
			}
		}
		return EC_FALSE;
	}

	RESULT soNode::RemoveConexion(soNode& _node)
	{
		return RemoveConexionByID(_node.m_ID);
	}

}