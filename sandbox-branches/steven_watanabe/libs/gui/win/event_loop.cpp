// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/gui/event_loop.hpp>

void boost::gui::event_loop::add_filter( event_filter * mf )
{
   if( mf != 0 )
   {
      mf -> next = filters;
      filters    = mf;
   }
}

bool boost::gui::event_loop::preprocess()
{
   if( platf::event::preprocess()) return true;

   for( filter_chain i = filters; i != 0; i = i -> next )
   {
      if( i -> filter( *this )) return true;
   }
   return false;
}

int boost::gui::event_loop::process()
{
   while( get()) if( !preprocess())
   {
      ::TranslateMessage( this );
      ::DispatchMessage(  this );
   }
   return( wParam );
}

int PASCAL WinMain( HINSTANCE, HINSTANCE, LPSTR cmdline, int sw )
{
   boost::gui::entry_data ed( cmdline, sw );
   try
   {
      return gui_main( ed );
   }
   catch( std::exception & e )
   {
      ::MessageBoxA( 0, e.what(), "Program Error", MB_OK );
   }
   return E_FAIL;
}
