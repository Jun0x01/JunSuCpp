/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.09
 */
#ifndef SU_LOG_INCLUDED
#define SU_LOG_INCLUDED


#include "Data/exportsDefine.h"
#include <iostream>
using namespace std;

namespace SuperMap
{
	class SU_DATA_API Log
	{
	public:
		static void Warning(string msg);
		static void Error(string msg);
		static void Debug(string msg);

	};
}

#endif
