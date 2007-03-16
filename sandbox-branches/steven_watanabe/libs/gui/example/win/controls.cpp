// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/gui/log.hpp>
#include <boost/gui/component.hpp>
#include <boost/gui/event_loop.hpp>

#include "frame.hpp"

class edit: public gui::component
{
   public:
      inline edit( gui::component * p = 0, gui::area a = gui::area()):
         gui::component
         (
            TEXT( "EDIT" ), 0, p,
            gui::frame_style( WS_BORDER | WS_VISIBLE, WS_EX_CLIENTEDGE ), a
         )
      {
      }
};

class button: public gui::component
{
   public:
      virtual void pressed( gui::event * );
   public:
      inline button
      (
         gui::string title,  gui::component * p = 0, gui::area a = gui::area()
      ):
         gui::component( TEXT( "BUTTON" ), title, p, WS_VISIBLE, a )
      {
         hook_events();
         handler_for( gui::event_id( WM_COMMAND, BN_CLICKED ))
            .connect( gui::make_handler( this, &button::pressed ));
      }
};

void button::pressed( gui::event * ev )
{
   ::MessageBox( *parent, TEXT( "You pressed me!" ), TEXT( "button" ), MB_OK );
   ev -> handled = true;
}

int gui_main( const gui::entry_data & ed )
{
   guix::frame wnd( TEXT( "C++ Frame Window" ));
   edit edit(  &wnd );
   button btn( TEXT( "Press Me!" ), &wnd );

   edit.resize( gui::area( 10, 10, 200, 25 ));
   btn.resize(  gui::area( 10, 50, 200, 25 ));

   ::ShowWindow( wnd, ed.show );

   gui::event_loop el;
   return el.process();
}
