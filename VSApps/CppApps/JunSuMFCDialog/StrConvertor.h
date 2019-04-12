#pragma once
#include <string>
#include <afx.h>

class StrConvertor
{
public:
	StrConvertor();
	~StrConvertor();

public:
	// Convert CString to std::string
	static std::string CStringTstring(CString str);
};

