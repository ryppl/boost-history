// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "frame.hpp"

gui::win::window_type guix::frame::wt( TEXT( "CxxGUIFrame" ), COLOR_BTNFACE );

void guix::frame::EvDestroy( gui::event * ev )
{
   ::PostQuitMessage( 0 );
   ev -> handled = true;
}

void guix::frame::EvPosChanged( gui::event * ev )
{
   if( pane ) pane -> move( client_area );
   ev -> handled = true;
}

guix::frame::frame( gui::string name ):
   gui::component( wt, name, 0, WS_OVERLAPPEDWINDOW ),
   pane( 0 )
{
   set_handler( WM_DESTROY,          &frame::EvDestroy );
   set_handler( WM_WINDOWPOSCHANGED, &frame::EvPosChanged );
}
