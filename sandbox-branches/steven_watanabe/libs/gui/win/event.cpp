// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/gui/event.hpp>
#include <commctrl.h>

boost::gui::event::handle_type boost::gui::event::get_originator() const
{
   HWND originator = get_component_handle();
   switch(( *this )())
   {
      case WM_COMMAND:
         if( param2< HWND >() != 0 )
            originator = param2< HWND >();
         break;
      case WM_NOTIFY:
         originator = param2< NMHDR * >() -> hwndFrom;
         break;
      case WM_CTLCOLORBTN: case WM_CTLCOLOREDIT:      case WM_CTLCOLORLISTBOX:
      case WM_CTLCOLORDLG: case WM_CTLCOLORSCROLLBAR: case WM_CTLCOLORSTATIC:
      case WM_VKEYTOITEM:  case WM_CHARTOITEM:
      case WM_HSCROLL:     case WM_VSCROLL:
         originator = param2< HWND >();
         break;
      case WM_DRAWITEM: case WM_MEASUREITEM: case WM_COMPAREITEM:
      case WM_DELETEITEM:
      {
         int ctlid = param1< int >();
         if( ctlid != 0 ) originator = ::GetDlgItem( originator, ctlid );
      } break;
   }
   return originator;
}
