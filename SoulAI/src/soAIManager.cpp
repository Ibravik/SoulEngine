#include "soAIManager.h"

namespace SoulSDK
{
	soAIManager::soAIManager()
	{
		Connect(std::bind(&soAIManager::OnStartUp, this));
	}

	soAIManager::~soAIManager()
	{
	}

	RESULT soAIManager::OnStartUp()
	{
		return EC_OK;
	}

	void soAIManager::OnShutDown()
	{
	}

	std::vector<soNode*> soAIManager::Search(soNode* _initNode, soNode* _finalNode, PathFindingType _type)
	{
		switch (_type)
		{
		case PFT_NULL:
			break;
		case PFT_BFS:
			return m_BFS.Search(*_initNode, *_finalNode);
			break;
		default:
			break;
		}
		return std::vector<soNode*>();
	}
}