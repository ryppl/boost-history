// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/gui/event_handler.hpp>
#include <boost/gui/component.hpp>
#include <boost/gui/log.hpp>

bool boost::gui::event_handler::process_filter_chain( event_id et, event * ev )
{
   signal_type * h = events[ et ];
   if( h ) ( *h )( ev );
   return ev -> handled;
}

void boost::gui::event_handler::default_event( event * ev )
{
}

void boost::gui::event_handler::onevent( event * ev )
{
   BOOST_GUI_LOGOB(event) << *ev << std::endl;

   /** Process events along the set of registered handlers for that event until
     * the event is signalled as being "handled".
     */

   event_id et(( *ev )());
   if( !process_filter_chain( et, ev ))
   {
      /** Ordinary processing has not resulted in the event being handled, so
        * try an "extended" handler.
        */

      if( et.id == WM_COMMAND ) et.code = HIWORD( ev -> param1< WPARAM >());
      if( et.id == WM_NOTIFY  ) et.code = ev -> param2< NMHDR * >() -> code;
      if( et.code != 0 ) process_filter_chain( et, ev );
   }

   /** Perform special processing on the event if it originated from a different
     * window. Within the Win32 environment, this is called reflection.
     */

   if( !ev -> handled && ev -> system_event )
   {
      platf::component_handle originator = ev -> get_originator();
      if( originator != ev -> get_component_handle())
      {
         /** Redirect event to the originator of the event if it was not
           * handled by the parent window.
           */

         event e( *ev );
         e.set_component_handle( originator );
         gui::component * pwnd = e.get_component();
         if( pwnd )
         {
            e.system_event = false;
            pwnd -> onevent( &e );
            if( e.handled ) ev -> handled = true;
         }
      }
   }

   if( !ev -> handled ) default_event( ev );
}

boost::gui::event_handler::signal_type &
boost::gui::event_handler::handler_for( event_id id )
{
   signal_type * handler = events[ id ];
   if( handler == 0 ) events[ id ] = handler = new signal_type();
   return( *handler );
}

boost::gui::event_handler::~event_handler()
{
   for( event_map::iterator i = events.begin(); i != events.end(); ++i )
      delete ( *i ).second;
}
