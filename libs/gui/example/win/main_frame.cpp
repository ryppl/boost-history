// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/gui/component.hpp>
#include <boost/gui/win/window_type.hpp>
#include <boost/gui/event_loop.hpp>

namespace gui = boost::gui;

// register the window class with windows

gui::win::window_type main_frame_class( TEXT( "MyFrame" ), COLOR_BTNFACE );

// set up a main frame

class main_frame: public gui::component
{
   public:
      virtual void EvDestroy( gui::event * );
   public:
      inline main_frame( gui::string );
};

void main_frame::EvDestroy( gui::event * )
{
   ::PostQuitMessage( 0 ); // terminate event loop
}

main_frame::main_frame( gui::string name ):
   gui::component( main_frame_class, name, 0, WS_OVERLAPPEDWINDOW )
{
   set_handler( WM_DESTROY, &main_frame::EvDestroy );
}

// entry point

int gui_main( const gui::entry_data & ed )
{
   main_frame wnd( TEXT( "C++ Frame Window" ));

   ::ShowWindow( wnd, ed.show );

   gui::event_loop el;
   return el.process();
}
