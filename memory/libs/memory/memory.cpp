// memory.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <boost/memory/scoped_alloc.hpp>

// -------------------------------------------------------------------------

using namespace boost;

extern tls_block_pool_t _tls_blockPool;

#if defined(_WIN32)

BOOL APIENTRY DllMain(HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		_tls_blockPool.init();
		break;
	case DLL_PROCESS_DETACH:
		_tls_blockPool.term();
		break;
	}
    return TRUE;
}

#endif

// -------------------------------------------------------------------------
