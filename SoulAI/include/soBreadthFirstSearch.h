/************************************************************************/
/**
* @LC	  09/18/2016
* @file   soBreadthFirstSearch.h
* @Author Rubén Rodríguez Serrano (rubn2113@gmail.com)
* @date   09/06/2016
* @brief  Class for Breadth First Search
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Headers												                */
/************************************************************************/
#include "soNode.h"

namespace SoulSDK
{
	/************************************************************************/
	/* soBreadthFirstSearch class declaration				                */
	/************************************************************************/
	class soBreadthFirstSearch
	{
		/************************************************************************/
		/* Constructor and Destructor declaration	                            */
		/************************************************************************/
	public:
		soBreadthFirstSearch();
		virtual ~soBreadthFirstSearch();

		/************************************************************************/
		/* soBreadthFirstSearch functions		                                */
		/************************************************************************/
	public:
		std::vector<soNode*> Search(soNode& _init, soNode& _end);			/*!< Returns a breadth path */
	};
}
