// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_EVENT_ID_HPP
#define BOOST_GUI_EVENT_ID_HPP

#include <boost/gui/event.hpp>

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

}}

#endif
