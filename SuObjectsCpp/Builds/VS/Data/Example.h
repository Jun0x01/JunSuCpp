// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DATA_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DATA_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

/*
#ifdef DATA_EXPORTS
#define DATA_API __declspec(dllexport)
#else
#define DATA_API __declspec(dllimport)
#endif
*/

#include "Data/exportsDefine.h"


// This class is exported from the Data.dll
class SU_DATA_API Example {
public:
	Example(void);
	// TODO: add your methods here.
};

extern SU_DATA_API int nData;

SU_DATA_API int fnData(void);
