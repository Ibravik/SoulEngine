#include "soFilesManager.h"

namespace SoulSDK
{
	soFilesManager::soFilesManager()
	{
		Connect(std::bind(&soFilesManager::OnStartUp, this, std::placeholders::_1));
	}


	soFilesManager::~soFilesManager()
	{
	}
	std::vector<soString*> soFilesManager::GetDirectoryFilesName()
	{
		if (!m_filesNameList.empty())
			SAFE_DELETE_STDVECTOR(m_filesNameList);

		WIN32_FIND_DATA ffd;
		soString fullPath = m_path + L"\\*";
		soString* resultFile = NULL;

		HANDLE hFind = FindFirstFile(fullPath.c_str(), &ffd);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			OutputDebugString(L"Invalid Path");
		}

		bool finished = false;

		while (!finished)
		{
			resultFile = new soString(ffd.cFileName);
			m_filesNameList.push_back(resultFile);

			if (!FindNextFile(hFind, &ffd))
				finished = true;
		}

		//Para borrar (.) y (..) que salen como archivos de inicio
		m_filesNameList.erase(m_filesNameList.begin());
		m_filesNameList.erase(m_filesNameList.begin());

		return m_filesNameList;
	}
	soString soFilesManager::GetPathByID(uint32 _ID)
	{
		soString fullPath = m_path + L"\\" + m_filesNameList.at(_ID)->c_str();
		return fullPath;
	}
	
	RESULT soFilesManager::OnStartUp(const soString & _path)
	{
		m_path = _path;

		return EC_OK;
	}
	void soFilesManager::OnShutDown()
	{
		SAFE_DELETE_STDVECTOR(m_filesNameList);
	}
}