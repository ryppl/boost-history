// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_WIN_WINDOW_TYPE_HPP
#define BOOST_GUI_WIN_WINDOW_TYPE_HPP

#include <boost/gui/win/windows.hpp>

namespace boost { namespace gui { namespace win
{

/** [todo]: find a better way to implement this. */

class window_type: public WNDCLASSEX
{
   private:
      void fill
      (
         string, HBRUSH, HCURSOR = 0, HICON = 0, string = 0, HINSTANCE = 0
      );
   public:
      inline operator const string() const;
   public:
      inline window_type
      (
         string, int = COLOR_WINDOW, HCURSOR = 0, HICON = 0, string = 0
      );
      inline window_type
      (
         string, HBRUSH, HCURSOR = 0, HICON = 0, string = 0
      );
};

window_type::operator const string() const
{
   return lpszClassName;
}

window_type::window_type
(
   string name, int bg, HCURSOR cursor, HICON icon, string menu
)
{
   fill( name, reinterpret_cast< HBRUSH >( bg + 1 ), cursor, icon, menu );
   ::RegisterClassEx( this );
}

window_type::window_type
(
   string name, HBRUSH bg, HCURSOR cursor, HICON icon, string menu
)
{
   fill( name, bg, cursor, icon, menu );
   ::RegisterClassEx( this );
}

}}}

#endif
