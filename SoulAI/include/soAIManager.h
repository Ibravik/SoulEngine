/************************************************************************/
/**
* @LC	  09/18/2016
* @file   soNode.h
* @Author Rubén Rodríguez Serrano (rubn2113@gmail.com)
* @date   09/06/2016
* @brief  Class in charge to manage Path Finding
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Headers												                */
/************************************************************************/
#include <Soul_Math.h>
#include "soBreadthFirstSearch.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Enum for path finding types			                                */
	/************************************************************************/
	enum PathFindingType
	{
		PFT_NULL = 0,
		PFT_BFS,
		PFT_NUM
	};

	/************************************************************************/
	/* soAIManager class declaration		                                */
	/************************************************************************/
	class soAIManager : public soModule<soAIManager>
	{
		/************************************************************************/
		/* Constructor and Destructor declaration	                            */
		/************************************************************************/
	public:
		soAIManager();
		virtual ~soAIManager();

		/************************************************************************/
		/* soAIManager member variables	 	                                    */
		/************************************************************************/
	private:
		soBreadthFirstSearch m_BFS;																				/*!< BFS member for operations */

		/************************************************************************/
		/* soAIManager functions		                                        */
		/************************************************************************/
	private:
		RESULT OnStartUp();																						/*!< Inherit Module Start Up */
		virtual void OnShutDown() override;																		/*!< Inherit Module Shut Down */

	public:
		std::vector<soNode*> Search(soNode* _initNode, soNode* _finalNode, PathFindingType _type);				/*!< Calls BFS Search function. If finalNode is null the algorithm just go over the nodes */
	};
}
