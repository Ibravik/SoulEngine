/************************************************************************/
/**
* @LC	  10/8/2016
* @file   soNode.h
* @Author Rubén Rodríguez Serrano & Lehi Muñoz (rubn2113@gmail.com / lehi.munoz@gmail.com)
* @date   09/06/2016
* @brief  Class for create nodes. Un nodo es un contenedor de informacion
		  dentro de una red definida con conecciones entre diferentes nodos
		  tambien conocida como grafo.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "Soul_Math.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soNode									    */
	/************************************************************************/
	class soNode : public soStandardClass<>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soNode();												/*!< Constructor Standard */
		virtual ~soNode();										/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	public:
		uint32 m_ID;											/*!< Node ID */		
		bool m_visited;											/*!< bool to check if the node is visited */
		soNode* m_prev;											/*!< Pointer to the prev conexion where you come from */
		soNode* m_next;											/*!< Pointer to the next conexion where you go */
		soString m_name;										/*!< Node name */
		std::vector<soNode*> m_conexions;						/*!< List of conexion */		

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	public:
		virtual void Visit(soNode* _prev);						/*!< virtual function for a visited node */
		bool IsVisited();										/*!< function to check if a node is visited */
		RESULT AddConexions(soNode* _conexions);				/*!< Adds a new conexion */
		RESULT RemoveConexionByName(soString& _name);			/*!< Removes a conexion by name */
		RESULT RemoveConexionByID(uint32 _ID);					/*!< Removes a conexion by ID */
		RESULT RemoveConexion(soNode& _node);					/*!< Removes a conexion */
	};
}