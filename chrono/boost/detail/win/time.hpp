//  time.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef BOOST_DETAIL_WIN_TIME_HPP
#define BOOST_DETAIL_WIN_TIME_HPP
#include <boost/config.hpp>
#include <cstdarg>
#include <boost/detail/win/basic_types.hpp>

#if defined( BOOST_USE_WINDOWS_H )
# if !defined( BOOST_DETAIL_WIN_WINDOWS_H_INCLUDED )
#  include <windows.h>
#  define BOOST_DETAIL_WIN_WINDOWS_H_INCLUDED
# endif

namespace boost
{
    namespace detail
    {
        namespace win32
        {
	        typedef FILETIME FILETIME_;
	        typedef PFILETIME PFILETIME_;
        
            using ::GetProcessTimes;
            using ::GetSystemTimeAsFileTime;
            using ::GetThreadTimes;
            
            using ::QueryPerformanceCounter;
            using ::QueryPerformanceFrequency;
        }
    }
}

#elif defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32__ )

namespace boost
{
    namespace detail
    {
        namespace win32
        {
            extern "C"
            {
            	
            	typedef struct _FILETIME {
            	  DWORD_ dwLowDateTime;
            	  DWORD_ dwHighDateTime;
            	} FILETIME_, *PFILETIME_, *LPFILETIME_;

//                using ::GetProcessTimes;
                __declspec(dllimport) 
                BOOL_ 
                WINAPI 
                GetProcessTimes(
                  HANDLE_ hProcess,
                  LPFILETIME_ lpCreationTime,
                  LPFILETIME_ lpExitTime,
                  LPFILETIME_ lpKernelTime,
                  LPFILETIME_ lpUserTime
                );
                
//                using ::GetSystemTimeAsFileTime;
                __declspec(dllimport) 
                void 
                WINAPI 
                GetSystemTimeAsFileTime(FILETIME_* lpFileTime);
                
//                using ::GetThreadTimes;
                __declspec(dllimport) 
                BOOL_ 
                WINAPI 
                GetThreadTimes(
                  HANDLE_ hThread,
                  LPFILETIME_ lpCreationTime,
                  LPFILETIME_ lpExitTime,
                  LPFILETIME_ lpKernelTime,
                  LPFILETIME_ lpUserTime
                );

//                using ::QueryPerformanceCounter;
            	__declspec(dllimport) 
                BOOL_ 
                WINAPI 
                QueryPerformanceCounter(
                	LARGE_INTEGER_ *lpPerformanceCount
                );
                
//                using ::QueryPerformanceFrequency;
            	__declspec(dllimport) 
            	BOOL_ 
            	WINAPI 
            	QueryPerformanceFrequency(
            	  LARGE_INTEGER_ *lpFrequency
            	);                   
            }
        }
    }
}
#else
# error "Win32 functions not available"
#endif
#endif // BOOST_DETAIL_WIN_TIME_HPP
