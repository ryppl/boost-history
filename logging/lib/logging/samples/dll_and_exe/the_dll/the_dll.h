#ifndef THE_DLL_H
#define THE_DLL_H


// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the THE_DLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// THE_DLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef THE_DLL_EXPORTS
#define THE_DLL_API __declspec(dllexport)
#else
#define THE_DLL_API __declspec(dllimport)
#endif

#endif
