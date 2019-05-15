/*
 *  Author: Jun0x01@github.com
 *  Date:   2019.05.15
 */
#ifndef SU_EXPORTS_INCLUDED
#define SU_EXPORTS_INCLUDED

// define system's export and import
#ifdef WIN32
    #define SU_EXPORT __declspec(dllexport)
    #define SU_IMPORT __declspec(dllimport)
#else
// Linux
    #define SU_EXPORT
    #define SU_IMPORT 
#endif

// define module's export and import
#ifdef SU_DATA_EXPORTS
    #define SU_DATA_API SU_EXPORT
#else
    #define SU_DATA_API SU_IMPORT
#endif // !SU_DATA_EXPORTS

#ifdef SU_MAPPING_EXPORTS
    #define SU_MAPPING_API SU_EXPORT
#else
    #define SU_MAPPING_API SU_IMPORT
#endif // !SU_MAPPING_EXPORTS


#endif // !SU_EXPORTS_INCLUDED


// Callback
#ifdef WIN32
   #define SuCALLBACK __stdcall
#else
   #define SuCALLBACK
#endif


