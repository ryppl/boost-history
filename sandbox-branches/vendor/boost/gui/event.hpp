// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_EVENT_HPP
#define BOOST_GUI_EVENT_HPP

#include <boost/gui/platform.hpp>

namespace boost { namespace gui
{

/** platform-independent event.
 * Provides a facility for controling the value returned by the event and the
 * status of its processing by the library/program.
 */

class event: public platf::event
{
   public:
      typedef platf::event::return_type return_type;
      typedef platf::event::id_type     id_type;
      typedef platf::event::handle_type handle_type;
   private:
      return_type  retval;  /**< value to be returned to the system */
   public:
      bool handled;      /**< has the program dealt with the event? */
      bool system_event; /**< is the event a system-generated event? */
   public: // return type interaction
      inline operator return_type() const;
      inline event & operator=( return_type );
      template< typename T > inline event & operator=( T );
   public: // message handling facilities
      /** get the component handle to the component that generated this event. */
      handle_type get_originator() const;
   public:
      inline event( id_type id, handle_type h = handle_type());
};

event::operator return_type() const
{
   return retval;
}

event & event::operator=( return_type rv )
{
   retval = rv;
   return *this;
}

template< typename T >
event & event::operator=( T rv )
{
   retval = reinterpret_cast< return_type >( rv );
   return *this;
}

event::event( id_type id, handle_type h ): platf::event( id, h ),
   handled( false ), system_event( false )
{
}

}}

#endif
