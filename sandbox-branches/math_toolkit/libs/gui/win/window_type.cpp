// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/gui/win/window_type.hpp>

LRESULT CALLBACK EventHandler
(
   HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam
);

void boost::gui::win::window_type::fill
(
   string name, HBRUSH bg, HCURSOR cursor, HICON icon, string menu, HINSTANCE hi
)
{
   ::memset( this, 0, sizeof( WNDCLASSEX ));
   cbSize        = sizeof( WNDCLASSEX );
   style         = 0;
   lpfnWndProc   = ::EventHandler;
   lpszClassName = name;
   hbrBackground = bg;
   lpszMenuName  = menu;
   hCursor       = ( cursor != 0 ) ? cursor : ::LoadCursor( 0, IDC_ARROW );
   hIcon         = ( icon   != 0 ) ? icon   : ::LoadIcon(   0, IDI_APPLICATION );
   hInstance     = ( hi     != 0 ) ? hi     : ::GetModuleHandle( 0 );
}
