//  time.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef BOOST_DETAIL_WIN_GETLASTERROR_HPP
#define BOOST_DETAIL_WIN_GETLASTERROR_HPP
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
		
            using ::GetLastError;
        	
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
            	
//            	  using ::GetLastError;
                __declspec(dllimport) 
                DWORD_ 
                WINAPI 
            	GetLastError();
            	
            }
        }
    }
}
#else
# error "Win32 functions not available"
#endif
#endif // BOOST_DETAIL_WIN_TIME_HPP
