// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_WIN_WINDOWS_HPP
#define BOOST_GUI_WIN_WINDOWS_HPP

#if(( defined(_MSC_VER) && (_MSC_VER >= 1020 )) || defined(__WAVE__))
#  pragma once
#endif

#include <windows.h>
#include <boost/config.hpp>

#if defined(BOOST_MSVC)
#  pragma comment(lib, "user32.lib")
#endif

namespace boost { namespace gui { namespace win
{

/** platform-dependant types */

typedef TCHAR         char_type;
typedef const TCHAR * string;
typedef HWND          component_handle;
typedef WNDPROC       component_callback;
typedef MSG           message_type;
typedef LONG_PTR      id_type;

#define GUIPROP_COMPONENT TEXT("gui::component")

/** windows-specific entry point data. */

struct entry_data
{
   LPCSTR command_args; /**< command line arguments */
   int    show;         /**< how to show the main window */

   inline entry_data( LPCSTR ca, int sw ):
      command_args( ca ), show( sw )
   {
   }
};

}}}

#endif
