// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_MAKE_HANDLER_HPP
#define BOOST_GUI_MAKE_HANDLER_HPP

#include <boost/gui/event.hpp>
#include <boost/gui/event_id.hpp>
#include <boost/signals.hpp>
#include <map>

namespace boost { namespace gui
{

namespace detail
{
   template< class Object >
   class object_handler
   {
      typedef void ( Object::* handler )( event * );
      private:
         Object * object;
         handler  onevent;
      public:
         void operator()( event * ev )
         {
            ( object ->* onevent )( ev );
         }
      public:
         object_handler( Object * o, handler h ):
            object( o ), onevent( h )
         {
         }
   };
}

/** Bind a member function as an event handler so it can be used as a signal
  * slot.
  */

template< typename Object >
inline detail::object_handler< Object >
make_handler( Object * o, void ( Object::* hfn )( event * ))
{
   return detail::object_handler< Object >( o, hfn );
}

}}

#endif
