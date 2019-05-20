/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.08
 */
#include "Data/LicenseManager.h"
#include "Toolkit/UGLicense.h"
using namespace SuperMap;


bool LicenseManager::IsValidStd()
{
	UGbool isValid = UGLicense::VerifyLicense(UGLicense_iObjectsCppCore);
	return isValid;
}