// Mapping.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Mapping.h"


// This is an example of an exported variable
SU_MAPPING_API int nMapping=0;

// This is an example of an exported function.
SU_MAPPING_API int fnMapping(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see Mapping.h for the class definition
CMapping::CMapping()
{
	return;
}
