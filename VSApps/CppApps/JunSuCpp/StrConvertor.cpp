/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.13
 */
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
	int len = WideCharToMultiByte(CP_UTF8, 0, LPCWSTR(cStr), -1, NULL, 0, NULL, NULL);
	char *str = new char[len];
	memset(str, 0, len);
	WideCharToMultiByte(CP_UTF8, 0, LPCWSTR(cStr), -1, str, len, NULL, NULL);
	std::string path(str);
	delete[] str;
	return path;
#else
	//Multi-Byte
	std::string path = cStr.GetBuffer(); 
	return path;
#endif
}

CString StrConvertor::stringTCString(std::string str)
{
#ifdef UNICODE
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t *wstr = new wchar_t[len];
	memset(wstr, 0, len * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr, len);
	CStringW output = wstr;
	delete[] wstr;

	return output;
#else
	return CString(str.c_str());
#endif 
}
