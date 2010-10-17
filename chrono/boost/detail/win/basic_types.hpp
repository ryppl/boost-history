//  time.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef BOOST_DETAIL_WIN_BASIC_TYPES_HPP
#define BOOST_DETAIL_WIN_BASIC_TYPES_HPP
#include <boost/config.hpp>
#include <cstdarg>

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
            typedef unsigned short WORD_;
            typedef DWORD DWORD_;
            typedef HANDLE HANDLE_;
            typedef LONG LONG_;
            typedef LONGLONG LONGLONG_;
            typedef LARGE_INTEGER LARGE_INTEGER_;
            typedef PLARGE_INTEGER PLARGE_INTEGER_;
            typedef void *PVOID_;
            typedef void *LPVOID_;
            typedef char CHAR_;
            typedef CHAR_ *LPSTR_;
            typedef const CHAR_ *LPCSTR_;
            typedef wchar_t WCHAR_;
            typedef WCHAR_ *LPWSTR_;
            typedef const WCHAR_ *LPCWSTR_;




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
                typedef unsigned short WORD_;
                typedef unsigned long DWORD_;
                typedef void* HANDLE_;

                typedef long LONG_;

// @FIXME Which condition must be tested
//                #if !defined(_M_IX86)
                typedef __int64 LONGLONG_;
//                #else
//                typedef double LONGLONG_;
//                #endif

                typedef struct _LARGE_INTEGER {
                    LONGLONG_ QuadPart;
                } LARGE_INTEGER_;
                typedef LARGE_INTEGER_ *PLARGE_INTEGER_;

                typedef void *PVOID_;
                typedef void *LPVOID_;
                typedef const void *LPCVOID_;

                typedef char CHAR_;
                typedef CHAR_ *LPSTR_;
                typedef const CHAR_ *LPCSTR_;

                typedef wchar_t WCHAR_;
                typedef WCHAR_ *LPWSTR_;
                typedef const WCHAR_ *LPCWSTR_;

            }
        }
    }
}
#else
# error "Win32 functions not available"
#endif
#endif // BOOST_DETAIL_WIN_TIME_HPP
