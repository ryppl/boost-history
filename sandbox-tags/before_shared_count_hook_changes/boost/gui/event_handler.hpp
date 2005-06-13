// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_EVENT_HANDLER_HPP
#define BOOST_GUI_EVENT_HANDLER_HPP

#include <boost/gui/event.hpp>
#include <boost/gui/event_id.hpp>
#include <boost/gui/make_handler.hpp>
#include <boost/signals.hpp>
#include <map>

namespace boost { namespace gui
{

/** Base class for registering and dealing with GUI events sent to a component.
  */

class event_handler
{
   public:
      typedef boost::signal< void ( event * ) > signal_type;
      typedef std::map< event_id, signal_type * > event_map;
   private:
      event_map events;
   public:
      signal_type & handler_for( event_id );
   private:
      bool process_filter_chain( event_id, event * );
   public:
      virtual void default_event( event * );
      void onevent( event * );
      virtual ~event_handler();
};

}}

#endif
