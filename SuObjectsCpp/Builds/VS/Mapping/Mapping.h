// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MAPPING_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MAPPING_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include "Data/exportsDefine.h"

// This class is exported from the Mapping.dll
class SU_MAPPING_API CMapping {
public:
	CMapping(void);
	// TODO: add your methods here.
};

extern SU_MAPPING_API int nMapping;

SU_MAPPING_API int fnMapping(void);
