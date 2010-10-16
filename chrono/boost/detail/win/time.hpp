//  time.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef BOOST_DETAIL_WIN_TIME_HPP
#define BOOST_DETAIL_WIN_TIME_HPP
#include <boost/config.hpp>

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
    		typedef BOOL BOOL_;
			typedef DWORD DWORD_;
        	typedef HANDLE HANDLE_;
    		typedef LONG LONG_;
			typedef LONGLONG LONGLONG_; 
	        typedef LARGE_INTEGER LARGE_INTEGER_;
	        typedef PLARGE_INTEGER PLARGE_INTEGER_;
	        typedef FILETIME FILETIME_;
	        typedef PFILETIME PFILETIME_;
			
            using ::GetLastError;
            using ::GetCurrentProcess;
            using ::GetCurrentThread;
            
            using ::GetProcessTimes;
            using ::GetSystemTimeAsFileTime;
            using ::GetThreadTimes;
            
            using ::QueryPerformanceCounter;
            using ::QueryPerformanceFrequency;
            
            static const DWORD ERROR_NOT_SUPPORTED_ = ERROR_NOT_SUPPORTED; 
            
        }
    }
}

#elif defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32__ )
# include <WinError.h>
//#    define WINAPI  __stdcall

// @FIXME Which condition must be tested            	
# ifdef UNDER_CE
#  ifndef WINAPI
#   ifndef _WIN32_WCE_EMULATION
#    define WINAPI  __cdecl     // Note this doesn't match the desktop definition
#   else
#    define WINAPI  __stdcall
#   endif
#  endif

#  ifdef __cplusplus
extern "C" {
#  endif



#  ifdef __cplusplus
}
#  endif
# else
#  ifndef WINAPI
#    define WINAPI  __stdcall
#  endif
# endif

namespace boost
{
    namespace detail
    {
        namespace win32
        {
            extern "C"
            {
            	typedef int BOOL_;
            	
            	typedef unsigned long DWORD_;
            	
            	typedef void* HANDLE_;
            	
            	typedef long LONG_;

// @FIXME Which condition must be tested            	
//            	#if !defined(_M_IX86)
            	typedef __int64 LONGLONG_; 
//            	#else
//            	typedef double LONGLONG_;
//            	#endif
            	           	
//            	typedef union _LARGE_INTEGER_ {
////            	  struct {
////            	    DWORD_ LowPart;
////            	    LONG_  HighPart;
////            	  } ;
//            	  struct {
//            	    DWORD_ LowPart;
//            	    LONG_  HighPart;
//            	  } u;
//            	  LONGLONG_ QuadPart;
//            	} LARGE_INTEGER_, *PLARGE_INTEGER_;
            	typedef struct _LARGE_INTEGER {
            	    LONGLONG_ QuadPart;
            	} LARGE_INTEGER_;           	
            	typedef LARGE_INTEGER_ *PLARGE_INTEGER_;
            	
            	typedef struct _FILETIME {
            	  DWORD_ dwLowDateTime;
            	  DWORD_ dwHighDateTime;
            	} FILETIME_, *PFILETIME_, *LPFILETIME_;

//            	  using ::GetLastError;
                __declspec(dllimport) 
                DWORD_ 
                WINAPI 
            	GetLastError();

//                using ::GetCurrentProcess;
            	__declspec(dllimport) 
            	HANDLE_ 
            	WINAPI 
            	GetCurrentProcess();
            		
//                using ::GetCurrentThread;
                __declspec(dllimport) 
                HANDLE_ 
                WINAPI 
                GetCurrentThread();
            	
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
