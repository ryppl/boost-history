// the_dll.cpp : Defines the entry point for the DLL application.
//
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "the_dll.h"
#include "dll_log.h"

void init_logs() ;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        // initialize our logs
        init_logs();
        L_DLL_ << "beginning of dll log";
        break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
        L_DLL_ << "end of dll log";
		break;
	}
    return TRUE;
}


THE_DLL_API void write_to_dll_logs() {
    int i = 0;
    L_DLL_ << "message from dll " << ++i;
    L_DLL_ << "hello world from dll " << ++i;
}
