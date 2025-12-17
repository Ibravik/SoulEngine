
#include "soStringFormat.h"
/************************************************************************************************************************/
/* Funciónes para hacer conversión de y hacia widestring y string         												*/
/************************************************************************************************************************/
extern std::string ws2s(const std::wstring& _WideString)
{
	return std::string(_WideString.begin(), _WideString.end());
}

extern std::wstring s2ws(const std::string& _String)
{
	return std::wstring(_String.begin(), _String.end());
}

extern std::wstring int2ws(const int32& _int)
{
	return std::to_wstring(_int);
}

extern std::string int2s(const int32& _int)
{
	return std::to_string(_int);
}

extern int32 ws2int(const std::wstring& _WideString)
{
	return std::stoi(_WideString);
}

extern int32 s2int(const std::string& _String)
{
	return std::stoi(_String);
}

extern std::wstring uint2ws(const uint32 &_uint)
{
	return std::to_wstring(_uint);
}

std::string uint2s(const uint32 & _int)
{
	return std::to_string(_int);
}

extern uint32 ws2uint(const std::wstring& _WideString)
{
	return std::wcstoul(_WideString.c_str(), NULL, 0);
}

extern uint32 s2uint(const std::string &_String)
{
	return std::stoul(_String, NULL, 0);
}

extern std::wstring float2ws(const float &_float)
{
	return std::to_wstring(_float);
}

std::string float2s(const float & _float)
{
	return std::to_string(_float);
}

extern float ws2float(const std::wstring& _WideString)
{
	return std::wcstof(_WideString.c_str(), NULL);
}

extern float s2float(const std::string &_String)
{
	return std::stof(_String, NULL);
}

extern soString GetFileName(const soString& _FilePath)
{
	soString FileName;
	soString Path = _FilePath;

	while (!Path.empty() && Path.back() != L'.')
	{
		Path.pop_back();
	}
	if(!Path.empty())
		Path.pop_back();
	
	while (!Path.empty() && Path.back() != L'\\'  && Path.back() != L'/')
	{
		FileName.push_back(Path.back());
		Path.pop_back();
	}

	std::reverse(FileName.begin(), FileName.end());

	return FileName;
}

extern soString GetFilePath(const soString& _FilePath)
{
	soString Path = _FilePath;

	while (!Path.empty() && Path.back() != L'\\' && Path.back() != L'/')
	{
		Path.pop_back();
	}

	return Path;
}

extern bool CheckExtentionFile(const soString& _FilePath, soString _ExtentionFile)
{
	soString FileExtention;
	soString Path = _FilePath;

	while (!Path.empty() && Path.back() != L'.')
	{
		FileExtention.push_back(Path.back());
		Path.pop_back();
	}

	std::reverse(FileExtention.begin(), FileExtention.end());

	return _ExtentionFile == FileExtention ? true : false;
}

