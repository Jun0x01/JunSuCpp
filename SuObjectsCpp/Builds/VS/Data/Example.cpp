// Data.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Example.h"


// This is an example of an exported variable
SU_DATA_API int nData=0;

// This is an example of an exported function.
SU_DATA_API int fnData(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see Data.h for the class definition
Example::Example()
{
	return;
}
