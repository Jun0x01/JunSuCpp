/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.11
 */
#include "Data/UGStrConvertor.h"

using namespace SuperMap;
string UGStrConvertor::Tostring(const UGString &ugstr, OGDCCharset::Charset charset /*= OGDCCharset::UTF8*/)
{
	UGMBString mbStr;
    mbStr.m_nCharset = charset;
	ugstr.ToMBString(mbStr);
	
	string str(mbStr.Cstr());
	return str;
	
}