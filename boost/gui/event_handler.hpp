// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_EVENT_HANDLER_HPP
#define BOOST_GUI_EVENT_HANDLER_HPP

#include <boost/gui/event.hpp>
#include <list>
#include <map>

namespace boost { namespace gui
{

class event_id
{
   public:
      event::id_type id;
      event::id_type code;
   public:
      inline operator event::id_type() const;

      inline event_id( event::id_type i, event::id_type c = 0 );
};

event_id::operator event::id_type() const
{
   return id;
}

event_id::event_id( event::id_type i, event::id_type c ):
   id( i ), code( c )
{
}

class event_handler
{
   public:
      typedef void ( event_handler::* handler )( event * );
      typedef std::list< handler > handler_chain;
      typedef std::map< event_id, handler_chain > event_map;
   private:
      event_map events;
   public:
      template< typename Handler >
      inline void set_handler( event_id, Handler );
   private:
      bool process_filter_chain( event_id, event * );
   public:
      virtual void default_event( event * );
      void onevent( event * );
      virtual ~event_handler();
};

template< typename Handler >
void event_handler::set_handler( event_id id, Handler h )
{
   events[ id ].push_back( handler( h ));
}

}}

#endif
