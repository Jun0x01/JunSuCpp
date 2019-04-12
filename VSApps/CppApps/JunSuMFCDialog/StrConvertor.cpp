#include "stdafx.h"
#include "StrConvertor.h"


StrConvertor::StrConvertor()
{
}


StrConvertor::~StrConvertor()
{
}

std::string StrConvertor::CStringTstring(CString cStr)
{
#ifdef UNICODE
	// UNICODE
	// LPCWSTR(strFile) : errorC2440: '<function-style-cast>' : cannot convert from 'CString' to 'LPCWSTR'
	int len = WideCharToMultiByte(CP_ACP, 0, LPCWSTR(cStr), -1, NULL, 0, NULL, NULL);
	char *str = new char[len];
	memset(str, 0, len);
	WideCharToMultiByte(CP_ACP, 0, LPCWSTR(cStr), -1, str, len, NULL, NULL);
	std::string path(str);
	delete[] str;
	return path;
#else
	//Multi-Byte, 
	string path = strFile.GetBuffer(); // strFile.GetBuffer(), release: cannot convert parameter 1 from 'wchar_t *' to 'const std::string &'
	return path;
#endif
}