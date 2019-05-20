/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.11
 */
#include "stdafx.h"
#include "Data/UGStrConvertor.h"

using namespace SuperMap;
string UGStrConvertor::Tostring(UGString &ugstr)
{
	UGMBString mbStr;
	ugstr.ToMBString(mbStr);
	
	string str(mbStr.Cstr());
	return str;
	
}