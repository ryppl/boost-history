//  time.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef BOOST_DETAIL_WIN_TIME_HPP
#define BOOST_DETAIL_WIN_TIME_HPP

#include <boost/detail/win/basic_types.hpp>


namespace boost {
namespace detail {
namespace win32 {
#if defined( BOOST_USE_WINDOWS_H )
    typedef FILETIME FILETIME_;
    typedef PFILETIME PFILETIME_;
    typedef LPFILETIME LPFILETIME_;

    using ::GetSystemTimeAsFileTime;
#else
    extern "C" typedef struct _FILETIME {
        DWORD_ dwLowDateTime;
        DWORD_ dwHighDateTime;
    } FILETIME_, *PFILETIME_, *LPFILETIME_;

    extern "C" __declspec(dllimport) void WINAPI
        GetSystemTimeAsFileTime(FILETIME_* lpFileTime);

#endif
}
}
}

#endif // BOOST_DETAIL_WIN_TIME_HPP
