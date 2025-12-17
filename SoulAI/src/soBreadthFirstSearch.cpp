#include "soBreadthFirstSearch.h"
#include <vector>
#include <queue>



namespace SoulSDK
{
	soBreadthFirstSearch::soBreadthFirstSearch()
	{
	}


	soBreadthFirstSearch::~soBreadthFirstSearch()
	{
	}
	
	std::vector<soNode*> soBreadthFirstSearch::Search(soNode & _init, soNode & _end)
	{
		std::queue<soNode*> nodeQueue;
		std::vector<soNode*> UsedNodeList;
		std::vector<soNode*> FinalPath;

		soNode* actualNode = &_init;

		//Si no hay nodo inicial salimos
		if (&_init == NULL)
		{
			return std::vector<soNode*>();;
		}

		//Setamos al inicio con un padre null
		actualNode->Visit(NULL);

		//metemos al nodo actual al queue
		nodeQueue.push(actualNode);		

		//mientras el queue no este vacio
		while (!nodeQueue.empty())
		{
			//sacamos el primer nodo del queue
			nodeQueue.pop();

			UsedNodeList.push_back(actualNode);

			//Checamos las conexiones del nodo actual
			for (SIZE_T i = 0; i < actualNode->m_conexions.size(); i++)
			{				
				if (actualNode->m_conexions[i]->IsVisited())
				{
					continue;
				}
				else
				{
					//Si hay conexiones le damos visited
					actualNode->m_conexions[i]->Visit(actualNode);

					//Metemos la conexion al queue
					nodeQueue.push(actualNode->m_conexions[i]);
				}
			}
			//actualNode->Visited();
			if (!nodeQueue.empty())
			{
				actualNode = nodeQueue.front();
			}
		}

		//Si existe un end retornamos el path
		if (&_end != NULL)
		{
			//return path
		}

		for (uint32 i = 0; i < static_cast<uint32>(UsedNodeList.size()); i++)
		{
			UsedNodeList.at(i)->m_visited = false;
			UsedNodeList.at(i)->m_prev = NULL;
			UsedNodeList.at(i)->m_next = NULL;
		}

		//si no retornamos un path vacio
		return FinalPath;
	}
}
